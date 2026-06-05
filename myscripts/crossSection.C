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

void crossSection(double mean = 770, double xlow = 650, double xhigh = 880)
{
    const char *inputFile = "/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/pers_tr_in49Kout49K_all_1000.root";
    TFile *fi = new TFile(inputFile, "READ");
    TH1F *h1 = (TH1F *)fi->Get("egamABdc");
    TCanvas *c1 = new TCanvas("c1", "cross section", 900, 600);

    int scale = 10;
    h1->Rebin(scale);

    h1->GetXaxis()->SetRangeUser(0, 2000);
    h1->GetYaxis()->SetRangeUser(0, 1000);
    double sigma = (xhigh - xlow) / 6.0; // rough estimate of sigma

    double ylow = h1->GetBinContent(h1->FindBin(xlow));
    double yhigh = h1->GetBinContent(h1->FindBin(xhigh));
    double slope = (yhigh - ylow) / (xhigh - xlow);
    double Amp = (h1->GetBinContent(mean) + h1->GetBinContent(mean + 1)) / 2.0;
    cout << "Amp: " << Amp << ", Slope: " << slope << endl;

    // Pol1 + Gaussian fits -----------------------------------------------------
    TF1 *fitFunc = new TF1("fitFunc", "gaus(0)+pol1(3)", xlow, xhigh);
    fitFunc->SetParameters(
        Amp, mean, sigma, // Gaussian parameters: amplitude, mean, sigma
        0, 0              // Linear background parameters: constant term, slope
    );
    h1->Fit(fitFunc, "R");

    // parameters from fit
    double Ampf = fitFunc->GetParameter(0);
    double meanf = fitFunc->GetParameter(1);
    double sigmaf = fitFunc->GetParameter(2);
    double bgConst = fitFunc->GetParameter(3);
    double bgSlope = fitFunc->GetParameter(4);

    h1->Draw();

    // extract and draw ONLY the straight line background
    TF1 *bgLine = new TF1("bgLine", "pol1", xlow - 50, xhigh + 50); // extend a bit beyond fit range for better visualization
    bgLine->SetParameters(bgConst, bgSlope);                        // p0 + p1*x
    bgLine->SetLineColor(kBlue);
    bgLine->SetLineStyle(2); // dashed
    bgLine->SetLineWidth(2);
    bgLine->Draw("same");

    double area = Ampf * sigmaf * sqrt(2 * 3.14159265); // Gaussian integral
    cout << "Area under Gaussian: " << area << endl;
    cout << "Fit Mean: " << meanf << " , downscale by: " << scale << " , Area under Gaussian: " << area / scale << endl;

    // Pol1 + Gaussian fits -----------------------------------------------------

    // Exp + Gaussian fits -----------------------------------------------------
    // vector<double> xExpFit = {434.7, 483.6, 668.6, 891.7, 1144.6, 1533, 1805};
    // vector<double> yExpFit = {};
    // for (size_t i = 0; i < xExpFit.size(); ++i)
    // {
    //     yExpFit.push_back(h1->GetBinContent(h1->FindBin(xExpFit[i])));
    // }

    // // Create graph
    // TGraph *gr = new TGraph(xExpFit.size(), &xExpFit[0], &yExpFit[0]);

    // // Fit exponential: y = p0 * exp(p1 * x)
    // TF1 *fExp = new TF1("fExp", "[0]*exp([1]*x)", xExpFit.front(), xExpFit.back());
    // fExp->SetParameters(1000, 0.001); // Initial guess for parameters
    // gr->Fit(fExp, "R");               // R = fit in the given range

    // TF1 *fExpGaus = new TF1("fExpGaus", "[0]*exp([1]*x) + [2]*exp(-0.5*((x-[3])/[4])**2)", xExpFit.front(), xExpFit.back());

    // // Initialize parameters based on previous exponential fit
    // fExpGaus->SetParameter(0, fExp->GetParameter(0)); // exp amplitude
    // fExpGaus->SetParameter(1, fExp->GetParameter(1)); // exp slope
    // fExpGaus->SetParameter(2, Amp);                   // gaussian amplitude
    // fExpGaus->SetParameter(3, mean);                  // gaussian mean
    // fExpGaus->SetParameter(4, sigma);                 // gaussian sigma guess

    // h1->Fit(fExpGaus, "R");

    // h1->Draw();

    // for (size_t i = 0; i < xExpFit.size(); ++i)
    // {

    //     TMarker *marker = new TMarker(xExpFit[i], h1->GetBinContent(h1->FindBin(xExpFit[i])), 20);
    //     marker->SetMarkerColor(kRed);
    //     marker->SetMarkerSize(1.5);
    //     marker->Draw("same");
    // }
    // Exp + Gaussian fits -----------------------------------------------------

    c1->Update();

    // fi->Close();
    // delete fi;
}

// crossSection(730,652,900)
// crossSection(1282,1100,1480)
// crossSection(547,470,670)