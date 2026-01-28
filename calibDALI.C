#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TMarker.h"
#include "TSystem.h"
#include <iostream>
#include "TSpectrum.h"
#include <fstream>

struct GausResult
{
    double mean;
    double sigma;
    double amplitude;
    TF1 *fit;
};
GausResult fitOnePeak(TH1 *h1, double low, double high, double mean, bool draw);
// std::pair<double, double> fitOnePeak(TH1 *h1, double low, double high, double mean, bool draw);
void savePeakDataCSV(std::ofstream &outCSV, int det, const std::vector<double> &mean, const std::vector<double> &sigma);

// Co60 calibration files
const char *Co60calibFile = "/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/Calib/Co60_burn_0077.root";
const char *Co60outFile = "/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/Calib/Co60_0077_v3.root"; // v3 to stop unintended running
const char *Co60outCSV = "/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/Calib/Co60_0077_v3.csv";   // v3 to stop unintended running

void calibDALI_Co60(
    const char *inputFile,
    const char *outputRoot,
    const char *outputCSV)
{
    TFile *fi = new TFile(inputFile, "READ");       // input file
    TFile *fo = new TFile(outputRoot, "RECREATE");  // output ROOT file
    std::ofstream outCSV(outputCSV, std::ios::out); // output CSV file

    TH2F *h2 = (TH2F *)fi->Get("adc_id");
    TCanvas *c1 = new TCanvas("c1", "peaks to fit", 900, 600);

    fo->cd();

    int nbx = h2->GetNbinsX();
    // Loop
    for (int det = 0; det < nbx; det++)
    {
        TH1 *hp = h2->ProjectionY(Form("hp_det%d", det), det + 1, det + 1);
        hp->SetDirectory(0);
        if (hp->Integral() < 100)
        {
            delete hp;
            continue;
        }
        int resolution = 20;
        TSpectrum *sp = new TSpectrum(3, resolution);
        sp->SetResolution(resolution);
        hp->GetXaxis()->SetRangeUser(220, 1000);
        TH1 *h1 = (TH1 *)hp->Clone(Form("h1_det%d", det));
        Int_t nfound = sp->Search(h1, resolution, "nobackgroundoff", 0.6);
        TH1 *hb = sp->Background(h1, 20, "same");
        Double_t *xpeaks = sp->GetPositionX();
        Double_t *ypeaks = sp->GetPositionY();
        // std::cout << "Detector " << det << ": found " << nfound << " Peaks at: " << xpeaks[0] << ", " << xpeaks[1] << std::endl;

        if (abs(xpeaks[0] - xpeaks[1]) > 100)
        {
            int xrangeMin = (int)(std::max(xpeaks[0], xpeaks[1]) - 50);
            h1->GetXaxis()->SetRangeUser(xrangeMin, xrangeMin + 200);
            int resolution = 10;
            TSpectrum *sp = new TSpectrum(2, resolution);
            Int_t nfound = sp->Search(h1, resolution, "nobackgroundoff", 0.4);
            if (nfound != 2)
            {
                h1->Rebin(2);
                h1->GetXaxis()->SetRangeUser(xrangeMin, xrangeMin + 200);
                nfound = sp->Search(h1, resolution, "nobackgroundoff", 0.4);
            }
            TH1 *hb = sp->Background(h1, 20, "same");
            xpeaks = sp->GetPositionX();
            ypeaks = sp->GetPositionY();
            std::cout << "Refined " << nfound << " peaks at: " << xpeaks[0] << ", " << xpeaks[1] << std::endl;
        }

        c1->Clear();
        h1->SetTitle(Form("Detector_%d", det));
        h1->GetXaxis()->SetRangeUser(100, 1000);
        h1->Draw();

        vector<double> mean, sigma;
        // Initial fit
        for (int p = 0; p < nfound; p++)
        {
            auto res = fitOnePeak(h1, xpeaks[p] - 10, xpeaks[p] + 10, xpeaks[p], false);
            mean.push_back(res.mean);
            sigma.push_back(res.sigma);
        }
        // Rebin and do refined fit
        h1->Rebin(2);
        h1->GetXaxis()->SetRangeUser(200, 1000);
        for (int p = 0; p < nfound; p++)
        {
            auto res = fitOnePeak(h1, mean[p] - 1.8 * sigma[p], mean[p] + 1.8 * sigma[p], mean[p], true);
            mean[p] = res.mean;
            sigma[p] = res.sigma;
        }
        // Save to CSV
        savePeakDataCSV(outCSV, det, mean, sigma);

        hb->Draw("same");
        c1->Update();

        // Save this canvas inside the ROOT file
        c1->SetName(Form("Detector_%02d", det));
        c1->Write();

        delete hp;
    }

    // Close output file
    fo->Close();
    delete fo;

    // Close input file
    fi->Close();
    delete fi;
    outCSV.close();
}

GausResult fitOnePeak(TH1 *h1, double low, double high, double mean, bool draw)
{
    GausResult res{0, 0, 0, nullptr};

    TF1 *gausFit = new TF1("gausFit", "gaus", low, high);

    gausFit->SetParameter(1, mean); // initial guess for mean

    int status = h1->Fit(gausFit, "RQN");

    if (status != 0)
    {
        std::cerr << "Fit failed!" << std::endl;
        // return res;
    }

    if (draw)
    {
        gausFit->Draw("same");
    }

    res.mean = gausFit->GetParameter(1);
    res.sigma = gausFit->GetParameter(2);
    res.amplitude = gausFit->GetParameter(0);
    res.fit = gausFit;

    return res;
}

void savePeakDataCSV(std::ofstream &outCSV, int det,
                     const std::vector<double> &mean,
                     const std::vector<double> &sigma)
{
    if (!outCSV.is_open())
    {
        std::cerr << "Error: cannot write to CSV file!" << std::endl;
        return;
    }

    // Write header only once
    static bool header_written = false;
    if (!header_written)
    {
        outCSV << "DetectorID";
        for (size_t i = 0; i < mean.size(); ++i)
        {
            outCSV << "," << Form("Mean%d", static_cast<int>(i + 1))
                   << "," << Form("Sigma%d", static_cast<int>(i + 1));
        }
        outCSV << "\n";
        header_written = true;
    }

    // Write data row
    outCSV << det;
    for (size_t i = 0; i < mean.size(); ++i)
    {
        outCSV << "," << mean[i] << "," << sigma[i];
    }
    outCSV << "\n";
}

// Y88 calibration files
const char *Y88calibFile = "/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/Calib/Y88_burn_0078.root";
const char *Y88outFile = "/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/Calib/Y88_0078_v1.root"; // v3 to stop unintended running
const char *Y88outCSV = "/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/Calib/Y88_0078_v1.csv";   // v3 to stop unintended running

void calibDALI_Y88(
    const char *inputFile = Y88calibFile,
    const char *outputRoot = Y88outFile,
    const char *outputCSV = Y88outCSV)
{
    TFile *fi = new TFile(inputFile, "READ");       // input file
    std::ofstream outCSV(outputCSV, std::ios::out); // output CSV file
    TFile *fo = new TFile(outputRoot, "RECREATE");  // output ROOT file
    // Create dirs
    TDirectory *dirCanvas = fo->mkdir("Canvases");
    TDirectory *dirHist = fo->mkdir("Histograms");

    TH2F *h2 = (TH2F *)fi->Get("adc_id");
    TCanvas *c1 = new TCanvas("c1", "peaks to fit", 900, 600);

    fo->cd();

    int nbx = h2->GetNbinsX();
    std::cout << "Number of det Bins: " << nbx << std::endl;
    // Loop
    for (int det = 0; det < nbx; det++)
    {
        TH1 *hp = h2->ProjectionY(Form("hp_det%d", det), det + 1, det + 1);
        hp->SetDirectory(nullptr);
        hp->GetXaxis()->SetRangeUser(200, 1500); // Set range to avoid low-energy noise
        if (hp->Integral() < 100)
        {
            delete hp;
            continue;
        }
        TH1 *hsub = (TH1 *)hp->Clone(Form("hsub_det%d", det));
        hsub->SetDirectory(nullptr);

        int peakRes = 20;
        int bkgRes = 150;                      // 50 or higher is better, 10 or 20 substract the actual events from peaks and sigma gets smaller
        TSpectrum sp(10, peakRes);             // Max 10 peaks, peakRes for peak resolution, but dosen't affect much
        TH1 *hbkg = sp.Background(hp, bkgRes); // Estimate background; bkgRes larger value for smoother background
        hsub->Add(hbkg, -1.0);                 // Subtract background: hsub = hp − hbkg

        // int sigma = 10;
        // Int_t nfound = sp.Search(hsub, sigma, "nodraw", 0.2); // Adjust sigma for expected peak width
        Int_t nfound = sp.Search(hsub, 0.5);
        Double_t *xpeaks = sp.GetPositionX();
        Double_t *ypeaks = sp.GetPositionY();
        std::cout << "Detector " << det << ": found " << nfound << " peaks at:\t";
        for (int p = 0; p < nfound; p++)
        {
            std::cout << xpeaks[p] << "\t";
        }
        std::cout << std::endl;

        // Ensure first 2 peaks are in ascending order
        if (nfound >= 2 && xpeaks[0] > xpeaks[1])
        {
            std::swap(xpeaks[0], xpeaks[1]);
            std::swap(ypeaks[0], ypeaks[1]);
        }

        // Initial Fit
        std::vector<double> mean, sigma, amp;
        for (int p = 0; p < nfound; p++)
        {
            if (p < 2) // fit only first 2 relevant peaks
            {
                auto res = fitOnePeak(hsub, xpeaks[p] - 10, xpeaks[p] + 10, xpeaks[p], false);
                mean.push_back(res.mean);
                sigma.push_back(res.sigma);
            }
        }

        // Final Fit
        for (int p = 0; p < nfound; p++)
        {
            if (p < 2) // fit only first 2 relevant peaks
            {
                auto res = fitOnePeak(hsub, mean[p] - 2 * sigma[p], mean[p] + 2 * sigma[p], mean[p], false);
                mean[p] = res.mean;
                sigma[p] = res.sigma;
                amp.push_back(res.amplitude);
                hsub->GetListOfFunctions()->Add(res.fit); // attache fit to histogram
            }
        }
        std::cout << "Detector " << det << ": GaussFit Mean at:\t";
        for (int p = 0; p < mean.size(); p++)
        {
            if (p < 2) // print only first 2 relevant peaks
            {
                std::cout << mean[p] << "\t";
            }
        }
        std::cout << std::endl;
        std::cout << std::endl; // extra line for better readability

        c1->Clear();
        hsub->SetTitle(Form("Detector_%d", det));
        hsub->SetLineColor(kBlack);
        hsub->Draw();
        hp->Draw("same");
        hbkg->Draw("same");
        for (int p = 0; p < nfound; p++)
        {
            if (p < 2) // draw only first 2 relevant peaks
            {
                // TMarker *m = new TMarker(xpeaks[p], ypeaks[p], 20); // Marker style 23: inverted triangle; 20: full circle
                TMarker *m = new TMarker(mean[p], amp[p], 20); // Marker style 23: inverted triangle; 20: full circle
                m->SetMarkerColor(kBlue);
                m->SetMarkerSize(1.0);
                m->Draw("same");
            }
        }
        c1->Update();

        // Save background subtracted histogram in the Histograms directory
        dirHist->cd();
        hsub->Write();

        // Save this canvas inside the Canvases directory
        dirCanvas->cd();
        c1->SetName(Form("Detector_%02d", det));
        c1->Write();

        delete hp;
        delete hsub;
        delete hbkg;

        fo->cd();                                  // Ensure to return to the root file
        savePeakDataCSV(outCSV, det, mean, sigma); // Save to CSV
    }

    // Close output file
    fo->Close();
    delete fo;
    outCSV.close();

    // Close input file
    fi->Close();
    delete fi;
}

// Cs137 calibration files
const char *Cs137calibFile = "/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/Calib/Cs137_burn_0079.root";
const char *Cs137outFile = "/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/Calib/Cs137_0079_v2.root"; // v3 to stop unintended running
const char *Cs137outCSV = "/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/Calib/Cs137_0079_v2.csv";   // v3 to stop unintended running

void calibDALI_Cs137(
    const char *inputFile = Cs137calibFile,
    const char *outputRoot = Cs137outFile,
    const char *outputCSV = Cs137outCSV)
{
    TFile *fi = new TFile(inputFile, "READ");       // input file
    std::ofstream outCSV(outputCSV, std::ios::out); // output CSV file
    TFile *fo = new TFile(outputRoot, "RECREATE");  // output ROOT file
    // Create dirs
    TDirectory *dirCanvas = fo->mkdir("Canvases");
    TDirectory *dirHist = fo->mkdir("Histograms");

    TH2F *h2 = (TH2F *)fi->Get("adc_id");
    TCanvas *c1 = new TCanvas("c1", "peaks to fit", 900, 600);

    fo->cd();

    int nbx = h2->GetNbinsX();
    std::cout << "Number of det Bins: " << nbx << std::endl;
    // Loop
    for (int det = 0; det < nbx; det++)
    {
        TH1 *hp = h2->ProjectionY(Form("hp_det%d", det), det + 1, det + 1);
        hp->SetDirectory(nullptr);

        // // get X value at maximum bin
        // hp->GetXaxis()->SetRangeUser(100, 1500);
        // int maxBin = hp->GetMaximumBin();                     // Bin index of maximum
        // double maxContent = hp->GetBinContent(maxBin);        // Maximum bin content
        // double xAtMax = hp->GetXaxis()->GetBinCenter(maxBin); // X value at the center of that bin
        // cout << "Detector " << det << ", X at Max Bin = " << xAtMax << endl;
        // hp->GetXaxis()->SetRangeUser(xAtMax, 1500);  // but this causes issues for detectors with low counts

        hp->GetXaxis()->SetRangeUser(200, 1500); // Set range to avoid low-energy noise
        if (hp->Integral() < 100)
        {
            delete hp;
            continue;
        }

        TH1 *hsub = (TH1 *)hp->Clone(Form("hsub_det%d", det));
        hsub->SetDirectory(nullptr);

        int peakRes = 20;
        int bkgRes = 150;                      // 50 or higher is better, 10 or 20 substract the actual events from peaks and sigma gets smaller
        TSpectrum sp(10, peakRes);             // Max 10 peaks, peakRes for peak resolution, but dosen't affect much
        TH1 *hbkg = sp.Background(hp, bkgRes); // Estimate background; bkgRes larger value for smoother background
        hsub->Add(hbkg, -1.0);                 // Subtract background: hsub = hp − hbkg

        // int sigma = 10;
        // Int_t nfound = sp.Search(hsub, sigma, "nodraw", 0.2); // Adjust sigma for expected peak width
        Int_t nfound = sp.Search(hsub, 0.5);
        Double_t *xpeaks = sp.GetPositionX();
        Double_t *ypeaks = sp.GetPositionY();
        std::cout << "Detector " << det << ": found " << nfound << " peaks at:\t";
        for (int p = 0; p < nfound; p++)
        {
            std::cout << xpeaks[p] << "\t";
        }
        std::cout << std::endl;

        // Ensure first 2 peaks are in ascending order
        if (nfound >= 2 && xpeaks[0] > xpeaks[1] && ypeaks[0] < ypeaks[1])
        {
            std::swap(xpeaks[0], xpeaks[1]);
            std::swap(ypeaks[0], ypeaks[1]);
            cout << "Swapped peaks for Detector " << det << endl;
        }

        // Initial Fit
        std::vector<double> mean, sigma, amp;
        for (int p = 0; p < nfound; p++)
        {
            if (p < 1) // fit only first 1 relevant peaks
            {
                auto res = fitOnePeak(hsub, xpeaks[p] - 10, xpeaks[p] + 10, xpeaks[p], false);
                mean.push_back(res.mean);
                sigma.push_back(res.sigma);
            }
        }

        // Final Fit
        for (int p = 0; p < nfound; p++)
        {
            if (p < 1) // fit only first 1 relevant peaks
            {
                auto res = fitOnePeak(hsub, mean[p] - 2 * sigma[p], mean[p] + 2 * sigma[p], mean[p], false);
                mean[p] = res.mean;
                sigma[p] = res.sigma;
                amp.push_back(res.amplitude);
                hsub->GetListOfFunctions()->Add(res.fit); // attache fit to histogram
            }
        }
        std::cout << "Detector " << det << ": GaussFit Mean at:\t";
        for (int p = 0; p < mean.size(); p++)
        {
            if (p < 1) // print only first 1 relevant peaks
            {
                std::cout << mean[p] << "\t";
            }
        }
        std::cout << std::endl;
        std::cout << std::endl; // extra line for better readability

        c1->Clear();
        hsub->SetTitle(Form("Detector_%d", det));
        hsub->SetLineColor(kBlack);
        hsub->Draw();
        hp->Draw("same");
        hbkg->Draw("same");
        for (int p = 0; p < mean.size(); p++)
        {
            if (p < 1) // draw only first 1 relevant peaks
            {
                // TMarker *m = new TMarker(xpeaks[p], ypeaks[p], 20); // Marker style 23: inverted triangle; 20: full circle
                TMarker *m = new TMarker(mean[p], amp[p], 20); // Marker style 23: inverted triangle; 20: full circle
                m->SetMarkerColor(kBlue);
                m->SetMarkerSize(1.0);
                m->Draw("same");
            }
        }
        c1->Update();

        // Save background subtracted histogram in the Histograms directory
        dirHist->cd();
        hsub->Write();

        // Save this canvas inside the Canvases directory
        dirCanvas->cd();
        c1->SetName(Form("Detector_%02d", det));
        c1->Write();

        delete hp;
        delete hsub;
        delete hbkg;

        fo->cd();                                  // Ensure to return to the root file
        savePeakDataCSV(outCSV, det, mean, sigma); // Save to CSV
    }

    // Close output file
    fo->Close();
    delete fo;
    outCSV.close();

    // Close input file
    fi->Close();
    delete fi;
}
