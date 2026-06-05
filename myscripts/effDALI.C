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

#include <TH1.h>
#include <TF1.h>
#include <iostream>

double IntegratePhotopeak(TH1 *h, double mean, double low, double high, bool useFit = true)
{
    if (!h)
    {
        std::cerr << "Error: histogram pointer is null!" << std::endl;
        return 0;
    }

    // Simple integration with background subtraction
    if (!useFit)
    {
        int bin_low = h->FindBin(low);
        int bin_high = h->FindBin(high);

        // Estimate background as average of edges (first and last 2 bins)
        double bg_low = (h->GetBinContent(bin_low) + h->GetBinContent(bin_low + 1)) / 2.0;
        double bg_high = (h->GetBinContent(bin_high) + h->GetBinContent(bin_high - 1)) / 2.0;

        // Linear background slope
        double slope = (bg_high - bg_low) / (high - low);
        double bg_total = 0;

        double counts = 0;
        for (int i = bin_low; i <= bin_high; ++i)
        {
            double x = h->GetBinCenter(i);
            double bg = bg_low + slope * (x - low); // linear background
            double c = h->GetBinContent(i) - bg;
            if (c < 0)
                c = 0;
            counts += c;
        }
        return counts;
    }

    // Gaussian fit method
    double fitLow = low;
    double fitHigh = high;

    // TF1 *gaussFit = new TF1("gaussFit", "[0]*exp(-0.5*((x-[1])/[2])^2) + [3] + [4]*x", fitLow, fitHigh); // this and below both method works well
    TF1 *gaussFit = new TF1("gaussFit", "gaus(0)+pol1(3)", fitLow, fitHigh);
    gaussFit->SetParameters(h->GetMaximum(), mean, (high - low) / 6.0, 0, 0); // initial guesses

    h->Fit(gaussFit, "R"); // quiet fit

    // Area under Gaussian
    double A = gaussFit->GetParameter(0);
    double sigma = gaussFit->GetParameter(2);

    double area = A * sigma * sqrt(2 * 3.14159265); // Gaussian integral

    delete gaussFit;
    return area;
}

void efficiency(const char *iso, double peakEn, double low, double high, bool useFit = true)
{
    const char *Co60effFile = "/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/Calib/Co60_burn_0077_eff.root";
    const char *Y88effFile = "/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/Calib/Y88_burn_0078_eff.root";
    const char *Cs137effFile = "/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/Calib/Cs137_burn_0079_eff.root";

    TFile *file = nullptr;

    if (strcmp(iso, "Co60") == 0)
    {
        file = new TFile(Co60effFile, "READ");
    }
    else if (strcmp(iso, "Y88") == 0)
    {
        file = new TFile(Y88effFile, "READ");
    }
    else if (strcmp(iso, "Cs137") == 0)
    {
        file = new TFile(Cs137effFile, "READ");
    }
    else
    {
        std::cerr << "Unknown isotope: " << iso << std::endl;
        return;
    }

    if (!file || file->IsZombie())
    {
        std::cerr << "Failed to open ROOT file for " << iso << std::endl;
        return;
    }

    TH1F *h1 = (TH1F *)file->Get("egamma");
    if (!h1)
    {
        std::cerr << "Histogram egamma not found!" << std::endl;
        return;
    }

    double counts = IntegratePhotopeak(h1, peakEn, low, high, useFit);
    std::cout << iso << " peak @ " << peakEn
              << " keV → counts = " << counts << std::endl;
}
