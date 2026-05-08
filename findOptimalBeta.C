#include "TGraph.h"
#include "TCanvas.h"

void findOptimalBeta()
{
    TFile *fout = new TFile("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/rms_vs_beta.root", "RECREATE");

    TFile *fin = TFile::Open("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/merged/pers_tr_in49Kout49K_2000_merged_beta51416.root");
    if (!fin || fin->IsZombie())
    {
        Error("findOptimalBeta", "Cannot open ROOT file");
        return;
    }

    TH2F *h2 = (TH2F *)fin->Get("egamABdc_beta");
    if (!h2)
    {
        Error("findOptimalBeta", "Histogram not found");
        return;
    }

    double Epeak = 771.0;
    double fit_min = Epeak - 50;
    double fit_max = Epeak + 80;

    std::vector<double> beta_vals;
    std::vector<double> rms_vals;

    TCanvas *c1 = new TCanvas("c1", "RMS Scan", 800, 600);

    for (double ebeta = 0.4; ebeta < 0.6; ebeta += 0.001)
    {
        int binx = h2->GetXaxis()->FindBin(ebeta);

        TH1D *proj = h2->ProjectionY(
            Form("proj_%0.4f", ebeta), binx, binx);

        proj->Rebin(10);                           // Optional: rebin for smoother RMS
        proj->GetXaxis()->SetRangeUser(600, 1000); // Adjust as needed

        double mean = proj->GetMean();
        // double fit_min = mean - 50;
        // double fit_max = mean + 50;

        // --- Gaussian fit ---
        TF1 *g = new TF1("g", "gaus(0)+pol1(3)", fit_min, fit_max);
        g->SetParameter(1, Epeak); // Initial guess for mean
        g->SetParameter(2, 20);    // Initial guess for sigma
        proj->Fit(g, "RQ");        // Quiet + Range
        double sigma = g->GetParameter(2);

        double rms = proj->GetRMS();

        cout << "Beta: " << ebeta << " | RMS: " << rms << " | Mean: " << mean << " | Sigma: " << sigma << endl;

        beta_vals.push_back(ebeta);
        rms_vals.push_back(rms);

        // delete proj; // avoid memory pile-up

        // Optional: plot each projection
        proj->Draw();
        c1->Update();
    }

    // Create graph
    TGraph *gr = new TGraph(beta_vals.size(), &beta_vals[0], &rms_vals[0]);

    gr->SetTitle("RMS vs Beta;Beta;RMS");
    gr->SetMarkerStyle(20);
    gr->SetMarkerColor(kBlue);

    gr->Draw("AP");

    c1->Update();

    fout->cd();
    gr->Write("rms_graph");
    c1->Write("canvas");
    fout->Close();

    int min_i = std::min_element(rms_vals.begin(), rms_vals.end()) - rms_vals.begin();
    std::cout << "Best beta = " << beta_vals[min_i]
              << " | RMS = " << rms_vals[min_i] << std::endl;
}