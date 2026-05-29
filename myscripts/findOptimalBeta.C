#include "TGraph.h"
#include "TCanvas.h"
#include "TMath.h"

void findOptimalBeta()
{
    TFile *fout = new TFile("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/rms_vs_beta.root", "RECREATE");

    std::ofstream foutCSV("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/rms_vs_beta.csv", std::ios::out);
    // foutCSV << "Beta,RMS,Mean1,Mean2,Sigma1,Sigma2\n";

    // TFile *fin = TFile::Open("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/merged/pers_tr_in49Kout49K_2000_merged_beta51416.root");
    // TFile *fin = TFile::Open("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/merged/pers_tr_in49Kout49K_3000_merged_Abeta487.root");
    // TFile *fin = TFile::Open("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/merged/pers_tr_in49Kout49K_1000_merged_Abeta5166.root");
    TFile *fin = TFile::Open("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/merged/pers_tr_in49Kout49K_1000_merged_Abeta53067.root");
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

    double w1 = 0.5; // Weight for sigma1
    double w2 = 0.5; // Weight for sigma2

    double Epeak1 = 771.0;
    double fit_min1 = 610;
    double fit_max1 = 900;

    double Epeak2 = 1241.0;
    double fit_min2 = 1050;
    double fit_max2 = 1500;

    std::vector<double> beta_vals;
    std::vector<double> rms_vals;
    std::vector<double> sigma_vals1;
    std::vector<double> sigma_vals2;
    std::vector<double> weighted_sigma_vals;
    std::vector<double> mean_vals1;
    std::vector<double> mean_vals2;
    std::vector<double> yield_vals1;
    std::vector<double> yield_vals2;
    std::vector<double> weighted_yield_vals;

    TCanvas *c1 = new TCanvas("c1", "Sigma Scan", 800, 600);

    for (double ebeta = 0.44; ebeta < 0.6; ebeta += 0.001)
    {
        int binx = h2->GetXaxis()->FindBin(ebeta);

        TH1D *proj = h2->ProjectionY(
            Form("proj_%0.4f", ebeta), binx, binx);

        if (proj->GetEntries() < 10)
            continue;

        proj->Rebin(10);                           // Optional: rebin for smoother RMS
        proj->GetXaxis()->SetRangeUser(500, 2000); // Adjust as needed

        double rms = proj->GetRMS();

        // --- Gaussian fit ---
        TF1 *gp1 = new TF1("gp1", "gaus(0)+expo(3)", fit_min1, fit_max1);
        // TF1 *gp1 = new TF1("gp1", "gaus(0)+pol1(3)", fit_min1, fit_max1);
        gp1->SetParameter(1, Epeak1);
        gp1->SetParameter(2, 30);
        proj->Fit(gp1, "RQ+");
        double sigma1 = gp1->GetParameter(2);
        double mean1 = gp1->GetParameter(1);
        double amp1 = gp1->GetParameter(0);
        double yield1 = amp1 * sigma1 * sqrt(2 * TMath::Pi());

        // cout << "Beta: " << ebeta << endl;
        // cout << "amp1: " << amp1 << ", sigma1: " << sigma1 << ", yield1: " << yield1 << endl;

        TF1 *gp2 = new TF1("gp2", "gaus(0)+expo(3)", fit_min2, fit_max2);
        // TF1 *gp2 = new TF1("gp2", "gaus(0)+pol1(3)", fit_min2, fit_max2);
        gp2->SetParameter(1, Epeak2);
        gp2->SetParameter(2, 50);
        proj->Fit(gp2, "RQ+");
        double sigma2 = gp2->GetParameter(2);
        double mean2 = gp2->GetParameter(1);
        double amp2 = gp2->GetParameter(0);
        double yield2 = amp2 * sigma2 * sqrt(2 * TMath::Pi());

        // cout << "amp2: " << amp2 << ", sigma2: " << sigma2 << ", yield2: " << yield2 << endl;

        double weighted_sigma = (w1 * sigma1 + w2 * sigma2) / (w1 + w2);
        double weighted_yield = (w1 * yield1 + w2 * yield2) / (w1 + w2);

        cout << "Beta: " << ebeta << " | RMS: " << rms
             << " | Mean1: " << mean1 << " | Mean2: " << mean2 << " | Sigma1: " << sigma1
             << " | Sigma2: " << sigma2
             << " | Weighted Sigma: " << weighted_sigma << endl;

        foutCSV << "Beta: " << ebeta << " | RMS: " << rms
                << " | Mean1: " << mean1 << " | Mean2: " << mean2 << " | Sigma1: " << sigma1
                << " | Sigma2: " << sigma2
                << " | Weighted Sigma: " << weighted_sigma << endl;

        if (rms > 0)
        {
            beta_vals.push_back(ebeta);
            sigma_vals1.push_back(sigma1);
            sigma_vals2.push_back(sigma2);
            weighted_sigma_vals.push_back(weighted_sigma);

            mean_vals1.push_back(mean1);
            mean_vals2.push_back(mean2);

            yield_vals1.push_back(yield1);
            yield_vals2.push_back(yield2);
            weighted_yield_vals.push_back(weighted_yield);

            rms_vals.push_back(rms);
        }

        proj->SetTitle(Form("egamABdc | beta: %.4f | sig1: %.3f | sig2: %.3f | wsig: %.3f", ebeta, sigma1, sigma2, weighted_sigma));
        // proj->Draw();
        // c1->Update();

        fout->cd();
        proj->Write(Form("proj_%0.4f", ebeta));
    }

    TCanvas *c2 = new TCanvas("c2", "Sigma vs Beta", 1800, 600);
    c2->Divide(3, 1);
    // c2->Divide(3, 2);

    c2->cd(1);
    // Create graph
    TGraph *gr1 = new TGraph(beta_vals.size(), &beta_vals[0], &sigma_vals1[0]);
    gr1->SetTitle("Sigma1 vs Beta;Beta;Sigma1");
    gr1->SetMarkerStyle(20);
    gr1->SetMarkerColor(kBlue);
    // gr1->Draw("AP");
    gr1->Draw("");

    c2->cd(2);
    TGraph *gr2 = new TGraph(beta_vals.size(), &beta_vals[0], &sigma_vals2[0]);
    gr2->SetTitle("Sigma2 vs Beta;Beta;Sigma2");
    gr2->SetMarkerStyle(20);
    gr2->SetMarkerColor(kRed);
    gr2->Draw("");

    c2->cd(3);
    TGraph *gr3 = new TGraph(beta_vals.size(), &beta_vals[0], &weighted_sigma_vals[0]);
    gr3->SetTitle(Form("Weighted Sigma (w1: %.2f, w2: %.2f) vs Beta;Beta;Weighted Sigma", w1, w2));
    gr3->SetMarkerStyle(20);
    gr3->SetMarkerColor(kViolet);
    gr3->Draw("");

    // c2->cd(4);
    TGraph *grMean1 = new TGraph(beta_vals.size(), &mean_vals1[0], &beta_vals[0]);
    grMean1->SetTitle("Mean1 vs Beta; Mean1; Beta");
    grMean1->SetMarkerStyle(20);
    grMean1->SetMarkerColor(kGreen);
    // grMean1->Draw("");

    // c2->cd(5);
    TGraph *grMean2 = new TGraph(beta_vals.size(), &mean_vals2[0], &beta_vals[0]);
    grMean2->SetTitle("Mean2 vs Beta; Mean2; Beta");
    grMean2->SetMarkerStyle(20);
    grMean2->SetMarkerColor(kOrange);
    // grMean2->Draw("");

    // c2->cd(4);
    TGraph *grYield1 = new TGraph(beta_vals.size(), &beta_vals[0], &yield_vals1[0]);
    grYield1->SetTitle("Yield1 vs Beta;Beta;Yield1");
    grYield1->SetMarkerStyle(20);
    grYield1->SetMarkerColor(kGreen);
    // grYield1->Draw("");

    // c2->cd(5);
    TGraph *grYield2 = new TGraph(beta_vals.size(), &beta_vals[0], &yield_vals2[0]);
    grYield2->SetTitle("Yield2 vs Beta;Beta;Yield2");
    grYield2->SetMarkerStyle(20);
    grYield2->SetMarkerColor(kOrange);
    // grYield2->Draw("");

    // c2->cd(6);
    TGraph *grWeightedYield = new TGraph(beta_vals.size(), &beta_vals[0], &weighted_yield_vals[0]);
    grWeightedYield->SetTitle(Form("Weighted Yield (w1: %.2f, w2: %.2f) vs Beta;Beta;Weighted Yield", w1, w2));
    grWeightedYield->SetMarkerStyle(20);
    grWeightedYield->SetMarkerColor(kCyan);
    // grWeightedYield->Draw("");

    c2->Update();
    fout->cd();
    gr1->Write("sigma1_graph");
    gr2->Write("sigma2_graph");
    gr3->Write("weighted_sigma_graph");
    grMean1->Write("mean1_graph");
    grMean2->Write("mean2_graph");
    grYield1->Write("yield1_graph");
    grYield2->Write("yield2_graph");
    grWeightedYield->Write("weighted_yield_graph");
    c2->Write("canvas");
    fout->Close();

    // int min_i = std::min_element(rms_vals.begin(), rms_vals.end()) - rms_vals.begin();
    int min_i1 = std::min_element(sigma_vals1.begin(), sigma_vals1.end()) - sigma_vals1.begin();
    std::cout << "Best beta = " << beta_vals[min_i1]
              << " | RMS = " << rms_vals[min_i1]
              << " | SIGMA1 = " << sigma_vals1[min_i1] << std::endl;

    int min_i2 = std::min_element(sigma_vals2.begin(), sigma_vals2.end()) - sigma_vals2.begin();
    std::cout << "Best beta = " << beta_vals[min_i2]
              << " | RMS = " << rms_vals[min_i2]
              << " | SIGMA2 = " << sigma_vals2[min_i2] << std::endl;

    int min_i_weighted = std::min_element(weighted_sigma_vals.begin(), weighted_sigma_vals.end()) - weighted_sigma_vals.begin();
    std::cout << "Best beta = " << beta_vals[min_i_weighted]
              << " | RMS = " << rms_vals[min_i_weighted]
              << " | Weighted Sigma = " << weighted_sigma_vals[min_i_weighted] << std::endl;

    foutCSV << "Best beta = " << beta_vals[min_i1]
            << " | SIGMA1 = " << sigma_vals1[min_i1] << std::endl;

    foutCSV << "Best beta = " << beta_vals[min_i2]
            << " | SIGMA2 = " << sigma_vals2[min_i2] << std::endl;

    foutCSV << "Best beta = " << beta_vals[min_i_weighted]
            << " | Weighted Sigma = " << weighted_sigma_vals[min_i_weighted] << std::endl;
}