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

// Co60 calibration files
const char *Co60SpecCalibFile = "/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/Calib/Co60_0077_v2.root";       // write full path here
const char *Co60SpecOutFile = "/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/Calib/Co60_0077_v2_specAna.root"; // write full path here
const char *Co60SpecOutCSV = "/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/Calib/Co60_0077_v2_specAna.csv";   // write full path here

void calibSpectra(
    const char *inputFile = Co60SpecCalibFile,
    const char *outputRoot = Co60SpecOutFile,
    const char *outputCSV = Co60SpecOutCSV)
{
    TFile *fin = TFile::Open(inputFile, "READ");
    TFile *fout = new TFile(outputRoot, "RECREATE");
    std::ofstream outCSV(outputCSV, std::ios::out);

    TIter next(fin->GetListOfKeys());
    TKey *key;

    int nExtracted = 0;

    while ((key = (TKey *)next()))
    {
        if (strcmp(key->GetClassName(), "TCanvas") != 0)
            continue;

        TCanvas *c = (TCanvas *)key->ReadObj();
        cout << "\nCanvas: " << c->GetName() << endl;

        TIter nextObj(c->GetListOfPrimitives());
        TObject *obj;

        while ((obj = nextObj()))
        {
            if (!obj->InheritsFrom("TH1"))
                continue;

            TH1 *h = (TH1 *)obj;
            TString name = h->GetName();

            // FILTER
            if (!name.BeginsWith("h1_det"))
                continue;
            if (name.EndsWith("_background"))
                continue;

            std::cout << "Extracted Obj name: " << name << std::endl;
            nExtracted++;

            static TCanvas *c1 = nullptr;
            if (!c1)
                c1 = new TCanvas("c_all", "All spectra", 800, 600);
            c1->cd();
            c1->Clear();

            TH1 *hclone = (TH1 *)h->Clone(Form("%s_clone", name.Data()));
            hclone->SetDirectory(nullptr);

            hclone->GetXaxis()->SetRangeUser(200, 1000);

            int resolution = 20;

            // Clone again so original spectrum stays untouched
            TH1 *hsub = (TH1 *)hclone->Clone(Form("%s_sub", name.Data()));
            hsub->SetDirectory(nullptr);

            // Estimate background
            TSpectrum sp;
            TH1 *hbkg = sp.Background(hclone, resolution);

            // Subtract background: hsub = hclone − hbkg
            hsub->Add(hbkg, -1.0);

            Int_t nfound = sp.Search(hsub, 2, "nodraw", 0.5);
            Double_t *xpeaks = sp.GetPositionX();
            Double_t *ypeaks = sp.GetPositionY();
            std::cout << "Detector " << name << " | peaks found: " << nfound << std::endl;

            // Ensure peaks are in ascending order
            if (nfound == 2 && xpeaks[0] > xpeaks[1])
            {
                std::swap(xpeaks[0], xpeaks[1]);
                std::swap(ypeaks[0], ypeaks[1]);
            }

            // Validate peak count
            if (nfound != 2)
            {
                std::cout << "  Skipping detector " << name << "(bad peak count)\n";
                continue;
            }

            // // Draw
            // Background subtracted spectrum
            Int_t xrangeMin = (int)(std::min(xpeaks[0], xpeaks[1]) - 200);
            Int_t xrangeMax = (int)(std::max(xpeaks[0], xpeaks[1]) + 200);
            hsub->Rebin(2);
            hsub->GetXaxis()->SetRangeUser(xrangeMin, xrangeMax);
            hsub->SetMaximum(hsub->GetMaximum() * 1.2);
            hsub->SetLineColor(kBlack);
            hsub->Draw("hist");
            // hsub->SetLineColor(kRed);
            // hsub->Draw("hist same");

            for (int i = 0; i < nfound; i++)
            {
                TMarker *m = new TMarker(xpeaks[i], ypeaks[i], 20);
                m->SetMarkerColor(kRed);
                m->SetMarkerSize(1.0);
                m->Draw("same");
            }

            vector<double> mean, sigma, amp;
            // Initial fit
            for (int p = 0; p < nfound; p++)
            {
                auto res = fitOnePeak(hsub, xpeaks[p] - 20, xpeaks[p] + 20, xpeaks[p], false);
                mean.push_back(res.mean);
                sigma.push_back(res.sigma);
            }

            for (int p = 0; p < nfound; p++)
            {
                auto res = fitOnePeak(hsub, mean[p] - 1.5 * sigma[p], mean[p] + 1.5 * sigma[p], mean[p], true);
                mean[p] = res.mean;
                sigma[p] = res.sigma;
                amp.push_back(res.amplitude);

                hsub->GetListOfFunctions()->Add(res.fit); // attache fit to histogram
            }

            for (int i = 0; i < nfound; i++)
            {
                TMarker *m = new TMarker(mean[i], amp[i], 23); // Marker style 23: inverted triangle; 20: full circle
                m->SetMarkerColor(kBlue);
                m->SetMarkerSize(1.0);
                m->SetMarkerStyle(23); // Marker style 23: inverted triangle; 20: full circle
                m->Draw("same");

                // 20  // full circle ●   (default, clean)
                // 21  // full square ■
                // 22  // full triangle ▲
                // 23  // full inverted triangle ▼
                // 24  // open circle ○
                // 25  // open square □
                // 26  // open triangle △
                // 27  // open diamond ◇
                // 28  // open cross +
                // 29  // full star ★
                // 30  // open star ☆
            }

            // hclone->SetMaximum(hclone->GetMaximum() * 1.2);
            // hclone->SetLineColor(kBlack);
            // hclone->Draw("hist");

            // hbkg->SetLineWidth(2);
            // hbkg->SetLineColor(kBlue);
            // hbkg->SetLineStyle(2);
            // hbkg->Draw("hist same");

            c1->Update();
            gSystem->ProcessEvents(); // IMPORTANT
            gSystem->Sleep(100);      // milliseconds

            // Save to CSV
            // Extract detector ID from histogram name
            int det = -1;
            if (sscanf(name.Data(), "h1_det%d", &det) != 1)
            {
                std::cerr << "Error: cannot extract detector ID from histogram name " << name << std::endl;
                continue;
            }
            savePeakDataCSV(outCSV, det, mean, sigma);

            // delete hclone;
            // delete hsub;
            // delete hbkg;

            fout->cd();
            hsub->Write();
        }

        if (nExtracted == 280)
            break;
    }
    std::cout << "\nTotal histograms extracted: " << nExtracted << std::endl;

    fout->Close();
    delete fout;
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
