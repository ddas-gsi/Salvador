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
    int fitStatus;
};
GausResult fitOnePeak(TH1 *h1, double low, double high, double mean, bool draw);

struct LandauResult
{
    double mpv;
    double sigma;
    double amplitude;
    TF1 *fit;
    int fitStatus;
};
LandauResult fitLandau(TH1 *h1, double low, double high, double mpv, bool draw);

void timeCalib()
{
    // ofstream fouttxt("./dali_toffset_all_iso_g1000.txt");
    // ofstream fouttxt("./dali_toffset_all_iso_g1000_Au6000.txt");

    // ofstream foutcsv("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/timeOffsets_all_iso_6000_g1000.csv");
    // ofstream foutcsv("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/timeOffsets_all_iso_2000_g1000.csv");
    // ofstream foutcsv("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/timeOffsets.csv");
    // ofstream foutcsv("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/timeOffsets_g700.csv");
    // ofstream foutcsv("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/timeOffsets_le500.csv");

    // TFile *fout = TFile::Open("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/timeCalib_all_iso_6000_g1000.root", "RECREATE");
    // TFile *fout = TFile::Open("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/timeCalib_all_iso_2000_g1000.root", "RECREATE");
    // TFile *fout = TFile::Open("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/timeCalib_49K_2000_g700.root", "RECREATE");
    // TFile *fout = TFile::Open("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/timeCalib_49K_2000_le500.root", "RECREATE");
    // TFile *fin = TFile::Open("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/pers_tr_all_2000.root");
    // TFile *fout = TFile::Open("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/timeCalib_all_2000.root", "RECREATE");

    // TFile *fin = TFile::Open("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/pers_tr_all_iso_6000.root");
    // TFile *fin = TFile::Open("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/pers_tr_all_iso_2000.root");
    // TFile *fin = TFile::Open("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/pers_tr_in49Kout49K_all_2000.root");
    // TFile *fin = TFile::Open("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/pers_tr_in49Kout49K_all_2000_widerTimeGate_gain1_no15ns_apart.root");

    // ofstream fouttxt("/u/ddas/software/Salvador/toffset/dali_toffset_all_iso_g700_Au2000.txt");
    // ofstream foutcsv("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/timeOffsets_all_iso_g700_Au2000.csv");
    // TFile *fout = TFile::Open("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/timeOffsets_all_iso_g700_Au2000.root", "RECREATE");
    // TFile *fin = TFile::Open("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/pers_tr_all_iso_2000.root");

    // ofstream fouttxt("/u/ddas/software/Salvador/toffset/dali_toffset_all_iso_g700_Be3000.txt");
    // ofstream foutcsv("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/timeOffsets_all_iso_g700_Be3000.csv");
    // TFile *fout = TFile::Open("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/timeOffsets_all_iso_g700_Be3000.root", "RECREATE");
    // TFile *fin = TFile::Open("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/pers_tr_all_iso_3000.root");

    // ofstream fouttxt("/u/ddas/software/Salvador/toffset/dali_toffset_all_iso_g700_Be5000.txt");
    // ofstream foutcsv("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/timeOffsets_all_iso_g700_Be5000.csv");
    // TFile *fout = TFile::Open("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/timeOffsets_all_iso_g700_Be5000.root", "RECREATE");
    // TFile *fin = TFile::Open("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/pers_tr_all_iso_5000.root");

    ofstream fouttxt("/u/ddas/software/Salvador/toffset/dali_toffset_all_iso_g700_Au6000.txt");
    ofstream foutcsv("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/timeOffsets_all_iso_g700_Au6000.csv");
    TFile *fout = TFile::Open("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/timeOffsets_all_iso_g700_Au6000.root", "RECREATE");
    TFile *fin = TFile::Open("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/pers_tr_all_iso_6000.root");

    foutcsv << "DetectorID,MPV/Mean,Sigma,TimeOffset,MaxBin,MaxMPVDiff,FitStatus,ReFitStatus\n";
    TCanvas *c1 = new TCanvas("c1", "Time", 1200, 800);

    // TH2F *h2 = (TH2F *)fin->Get("time_id_g1000");
    TH2F *h2 = (TH2F *)fin->Get("time_id_g700");

    h2->GetYaxis()->SetRangeUser(-850, -1300);
    h2->Draw("colz");

    TH2F *h2_timeCalib = new TH2F("h2_timeCalib", "Time Calibration;Detector ID;Time Offset (ns)", 500, 0, 500, 1000, -200, 200);

    for (int i = 0; i < 500; i++)
    {
        TH1F *hp = (TH1F *)h2->ProjectionY(Form("hp%d", i), i + 1, i + 1);
        if (hp->Integral() < 10)
        {
            // cout << i << endl;
            continue;
        }

        // hp->Rebin(2);
        int max = hp->GetMaximumBin();
        double xmax = hp->GetXaxis()->GetBinCenter(max);
        double ymax = hp->GetBinContent(max);
        double totalContent = hp->Integral();
        double meanBinContent = hp->GetBinContent(max);
        cout << "Detector " << i << ": Max at " << xmax << " with content " << ymax << endl;

        hp->GetXaxis()->SetRangeUser(xmax - 150, xmax + 150);

        int fitStatus = 0; // 0 for success, others for failure
        int refitStatus = 0;

        TF1 *landauFit = new TF1("landauFit", "landau", xmax - 20, xmax + 40);
        landauFit->SetParameter(1, xmax); // initial guess for MPV
        landauFit->SetParameter(0, ymax);
        fitStatus = hp->Fit(landauFit, "RQ");
        if (fitStatus != 0)
        {
            delete landauFit; // Clean up the failed fit function
            landauFit = nullptr;

            // refit with a different range
            landauFit = new TF1("landauFit_refit1", "landau", xmax - 20, xmax + 30);
            landauFit->SetParameter(1, xmax); // initial guess for MPV
            landauFit->SetParameter(0, ymax);
            refitStatus = hp->Fit(landauFit, "RQ+");
        }

        if (abs(xmax - landauFit->GetParameter(1)) > 5)
        {
            cout << "  🚩 Fit MPV " << landauFit->GetParameter(1) << " is too far from max " << xmax << endl;

            delete landauFit; // Clean up the failed fit function
            landauFit = nullptr;

            // refit with a different range
            landauFit = new TF1("landauFit_refit2", "landau", xmax - 20, xmax + 20);
            landauFit->SetParameter(1, xmax); // initial guess for MPV
            landauFit->SetParameter(0, ymax);
            refitStatus = hp->Fit(landauFit, "RQ+");

            if (abs(xmax - landauFit->GetParameter(1)) > 5)
            {
                // If the fitted MPV is too far from the initial max, consider it a failure
                cout << "  ❌ Fit MPV " << landauFit->GetParameter(1) << " is too far from max " << xmax << ", marking as failure." << endl;
                refitStatus = -5; // Mark as failure
            }
        }

        double mpv = landauFit->GetParameter(1);
        double sigma = landauFit->GetParameter(2);
        double amplitude = landauFit->GetParameter(0);

        cout << "  Landau Fit result Det" << i << ": MPV = " << mpv << ", sigma = " << sigma << ", amplitude = " << amplitude << ", FitStatus: " << fitStatus << ", ReFitStatus: " << refitStatus << endl;

        foutcsv << i << "," << mpv << "," << sigma << "," << -mpv << "," << xmax << "," << xmax - mpv << "," << fitStatus << "," << refitStatus << "\n";

        fouttxt << "DALI.Toffset." << i << ":\t" << mpv << endl;
        fouttxt << "DALI.TimGain." << i << ":\t" << 1 << endl;

        for (int bin = 1; bin <= hp->GetNbinsX(); bin++)
        {
            double t = hp->GetXaxis()->GetBinCenter(bin);
            double content = hp->GetBinContent(bin);
            // double t_calib = t - res.mean; // Calibrated time using Gaussian mean
            double t_calib = t - mpv; // Calibrated time using Landau MPV
            h2_timeCalib->Fill(i, t_calib, content);
        }

        TCanvas *cp = new TCanvas(Form("c_proj_%d", i), Form("Projection %d", i), 800, 600);
        cp->cd();
        hp->Draw();
        // res.fit->Draw("same");
        fout->cd();
        cp->Write(Form("c_proj_%d", i));

        delete cp;
    }
    foutcsv.close();

    fout->cd();
    h2_timeCalib->Write("h2_timeCalib");

    TCanvas *c2 = new TCanvas("c2", "Time Calibration", 1500, 1000);
    c2->Divide(1, 2);
    c2->cd(1);
    h2->Draw("colz");
    c2->cd(2);
    h2_timeCalib->Draw("colz");
    fout->cd();
    c2->Write("c_time_calib");

    c2->Update();
    // gApplication->Run();

    fout->Close();
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

LandauResult fitLandau(TH1 *h1, double low, double high, double mpv, bool draw)
{
    LandauResult res{0, 0, 0, nullptr};

    TF1 *landauFit = new TF1("landauFit", "landau", low, high);

    landauFit->SetParameter(1, mpv); // initial guess for MPV

    int status = h1->Fit(landauFit, "RQN");

    if (status != 0)
    {
        std::cerr << "Fit failed!" << std::endl;
        // return res;
    }

    if (draw)
    {
        landauFit->Draw("same");
    }

    res.mpv = landauFit->GetParameter(1);
    res.sigma = landauFit->GetParameter(2);
    res.amplitude = landauFit->GetParameter(0);
    res.fit = landauFit;
    res.fitStatus = status;

    return res;
}