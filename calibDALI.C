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
void savePeakDataCSV(std::ofstream &outCSV, int det, const std::vector<double> &mean, const std::vector<double> &sigma);

// std::pair<double, double> fitOnePeak(TH1 *h1, double low, double high, double mean, bool draw);
std::pair<double, double> fitOnePeakWithBkgSub(TH1 *hSignal, double low, double high, double mean, bool draw);
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

std::pair<double, double> fitOnePeakWithBkgSub(TH1 *hSignal, double low, double high, double mean, bool draw)
{
    TF1 *gausFit = new TF1("gausFit", "gaus", low, high);
    gausFit->SetParameter(1, mean); // initial guess for mean
    hSignal->Fit(gausFit, "RQN");

    if (draw)
    {
        gausFit->Draw("same");
    }
    double Mean = gausFit->GetParameter(1);
    double Sigma = gausFit->GetParameter(2);

    return std::make_pair(Mean, Sigma);
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
    TFile *fo = new TFile(outputRoot, "RECREATE");  // output ROOT file
    std::ofstream outCSV(outputCSV, std::ios::out); // output CSV file

    TH2F *h2 = (TH2F *)fi->Get("adc_id");
    TCanvas *c1 = new TCanvas("c1", "peaks to fit", 900, 600);

    fo->cd();

    int nbx = h2->GetNbinsX();
    // Loop
    for (int det = 0; det < 5; det++)
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

        // delete hp;
    }

    // Close output file
    fo->Close();
    delete fo;

    // Close input file
    fi->Close();
    delete fi;
    outCSV.close();
}