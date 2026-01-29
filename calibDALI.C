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

// Calibrate and Fit the peaks for each detector

struct PeakData
{
    Int_t detID;
    Bool_t hasPeak1;
    Bool_t hasPeak2;
    Double_t mean1;
    Double_t sigma1;
    Double_t mean2;
    Double_t sigma2;
};

void ReadCSV_2Peaks(const std::string &filename,
                    std::map<int, PeakData> &db)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "ERROR: Cannot open " << filename << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // skip header

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string token;

        // --- read DetectorID ---
        std::getline(ss, token, ',');
        int det = std::stoi(token);

        // --- get or create entry ---
        PeakData &d = db[det];

        // --- infuse detID into struct ---
        d.detID = det;

        // --- read peak 1 ---
        std::getline(ss, token, ',');
        d.mean1 = std::stod(token);

        std::getline(ss, token, ',');
        d.sigma1 = std::stod(token);

        // --- read peak 2 ---
        std::getline(ss, token, ',');
        d.mean2 = std::stod(token);

        std::getline(ss, token, ',');
        d.sigma2 = std::stod(token);

        d.hasPeak1 = true;
        d.hasPeak2 = true;
    }
}

void ReadCSV_1Peak(const std::string &filename,
                   std::map<int, PeakData> &db)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "ERROR: Cannot open " << filename << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // skip header

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string token;

        // --- read DetectorID ---
        std::getline(ss, token, ',');
        int det = std::stoi(token);

        // --- get or create entry ---
        PeakData &d = db[det];

        // --- infuse detID ---
        d.detID = det;

        // --- read peak 1 ---
        std::getline(ss, token, ',');
        d.mean1 = std::stod(token);

        std::getline(ss, token, ',');
        d.sigma1 = std::stod(token);

        d.hasPeak1 = true;
        // hasPeak2 intentionally untouched
    }
}

void energyCalib(bool drawErr = false)
{
    std::map<int, PeakData> calibDB_Co60, calibDB_Y88, calibDB_Cs137;
    ReadCSV_2Peaks("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/Calib/Co60_0077_v_corr.csv", calibDB_Co60);
    ReadCSV_2Peaks("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/Calib/Y88_0078_v_corr.csv", calibDB_Y88);
    ReadCSV_1Peak("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/Calib/Cs137_0079_v_corr.csv", calibDB_Cs137);

    // calibration parameters ROOT file
    TFile *fo = TFile::Open(
        "/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/Calib/DALI_energyCalib_007X.root",
        "RECREATE");

    if (!fo || fo->IsZombie())
    {
        std::cerr << "ERROR: Cannot create output ROOT file" << std::endl;
        return;
    }
    // Create dirs
    TDirectory *dirFits = fo->mkdir("Fits");
    TDirectory *dirRes = fo->mkdir("Res");

    // calibration parameters CSV file
    std::ofstream outCSV("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/Calib/DALI_energyCalib_007X.csv");
    outCSV << "DetectorID,p0,p1,p0_err,p1_err,Chi2,NDf,redChi2\n"; // header

    if (calibDB_Co60.size() == calibDB_Y88.size() && calibDB_Y88.size() == calibDB_Cs137.size())
    {
        std::cout << "Calibration DBs loaded successfully and have consistent detector counts." << std::endl;
        std::cout << "Loaded detectors from source Co60  : " << calibDB_Co60.size() << std::endl;
        std::cout << "Loaded detectors from source Y88   : " << calibDB_Y88.size() << std::endl;
        std::cout << "Loaded detectors from source Cs137 : " << calibDB_Cs137.size() << std::endl;
    }
    else
    {
        std::cerr << "Warning: Calibration DBs have inconsistent detector counts!" << std::endl;
    }

    int ndet = calibDB_Co60.size(); // calibDB_Co60.size(); // For testing, limit to first 10 detectors
    int idet = 0;
    TCanvas *c1 = new TCanvas("c1", "Calibrate Detector", 900, 600);

    if (calibDB_Co60.size() == calibDB_Y88.size() && calibDB_Y88.size() == calibDB_Cs137.size())
    {
        for (const auto &[det, d] : calibDB_Co60)
        {
            std::cout << "Detector " << calibDB_Co60[det].detID << " : " << calibDB_Co60[det].mean1
                      << ", " << calibDB_Co60[det].mean2
                      << ", " << calibDB_Y88[det].mean1
                      << ", " << calibDB_Y88[det].mean2
                      << ", " << calibDB_Cs137[det].mean1 << std::endl;

            if (calibDB_Co60[det].detID == calibDB_Y88[det].detID && calibDB_Y88[det].detID == calibDB_Cs137[det].detID)
            {
                const int npeaks = 5;
                double energies[npeaks] = {1173.240, 1332.508, 898.047, 1836.090, 661.657}; // in keV for Co60(2), Y88(2), Cs137(1)
                double errEnergies[npeaks] = {0.003, 0.004, 0.011, 0.008, 0.003};           // in keV

                double adcPeakCentroids[npeaks] = {calibDB_Co60[det].mean1, calibDB_Co60[det].mean2,
                                                   calibDB_Y88[det].mean1, calibDB_Y88[det].mean2,
                                                   calibDB_Cs137[det].mean1};
                double adcPeakSigmas[npeaks] = {calibDB_Co60[det].sigma1, calibDB_Co60[det].sigma2,
                                                calibDB_Y88[det].sigma1, calibDB_Y88[det].sigma2,
                                                calibDB_Cs137[det].sigma1};

                c1->cd();
                c1->Clear();

                // Unique names per detector
                TString gname = Form("Detector_%d_calib", det);
                TString fname = Form("fit_det_%d", det);
                TString rname = Form("residuals_det_%d", det);

                TGraphErrors *gr = new TGraphErrors(npeaks, adcPeakCentroids, energies, adcPeakSigmas, errEnergies);
                gr->SetName(gname);
                gr->SetTitle(Form("Energy Calibration Detector %d", det));
                gr->GetXaxis()->SetTitle("ADC Channel (Centroids)");
                gr->GetYaxis()->SetTitle("Energy (keV)");

                // TF1 *linFit = new TF1("linFit", "pol1", 0, 2000);
                TF1 *linFit = new TF1(fname, "pol1", 0, 2000);
                gr->Fit(linFit, "W"); // "W" to ignore errors in X

                // Get Calibration parameters
                double p0 = linFit->GetParameter(0);    // intercept
                double p1 = linFit->GetParameter(1);    // slope
                double p0_err = linFit->GetParError(0); // uncertainty in intercept
                double p1_err = linFit->GetParError(1); // uncertainty in slope
                double chi2 = linFit->GetChisquare();
                int ndf = linFit->GetNDF();
                double redChi2 = (ndf > 0) ? chi2 / ndf : 0.0; // condition ? value_if_true : value_if_false
                std::cout << "Linear fit: y = " << p0 << " + " << p1 << " * x" << std::endl;
                linFit->SetTitle(Form("Detector %d:   E = %.3f + %.6f * ADC", det, p0, p1));
                gr->SetTitle(Form("Detector %d:    E = %.3f + %.6f * ADC", det, p0, p1)); // overwrite graph title with fit info

                // Save calibration parameters to CSV
                outCSV << det << "," << p0 << "," << p1 << "," << p0_err << "," << p1_err << "," << chi2 << "," << ndf << "," << redChi2 << "\n";

                // Create and fill residuals histogram
                TH1D *hres = new TH1D(rname, Form("Residuals detector %d;Energy residual (keV);Counts", det), 50, -100, 100);
                for (int i = 0; i < npeaks; i++)
                {
                    double efit = linFit->Eval(adcPeakCentroids[i]);
                    hres->Fill(energies[i] - efit);
                }

                if (drawErr)
                {
                    gr->SetLineColor(kGreen);
                    gr->SetLineStyle(1); // must be non-zero to draw X errors
                    gr->SetLineWidth(1); // must be non-zero to draw X errors
                }
                else
                {
                    gr->SetLineStyle(0); // no line
                    gr->SetLineWidth(0); // no line
                }

                gr->SetMarkerStyle(20);
                gr->SetMarkerSize(1.2);
                gr->Draw("APE"); // points + X errors

                linFit->SetLineColor(kRed);
                linFit->Draw("Same"); // overlay fit on the same canvas

                c1->Update();

                // Save to output file
                fo->cd();
                dirFits->cd();
                gr->Write();
                dirRes->cd();
                linFit->Write();
                hres->Write();

                delete gr;
                delete linFit;
                delete hres;
            }
            else
            {
                std::cerr << "Detector ID mismatch among calibration DBs for detector : " << det << "!" << std::endl;
            }

            idet++;
            if (idet >= ndet)
                break;
        }
        fo->Close();
        outCSV.close();
        delete fo;
    }
}
