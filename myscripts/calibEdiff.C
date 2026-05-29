/* This script is to check the calibrated energy difference
for different sources and different peaks */

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

void savePeakDataCSV(std::ofstream &foutCSV, int det,
                     const std::vector<double> &mean,
                     const std::vector<double> &sigma,
                     const std::vector<double> &energyDiffs,
                     const std::vector<int> &fitStatuses,
                     const std::vector<int> &refitStatuses);

void calibCoEdiff();

void calibEndiff()
{
    // vector<double> Epeaks = {898.047, 1836.090}; // Y88 898 keV, 1836 keV
    // vector<double> fit_mins = {700, 1600};
    // vector<double> fit_mins_mod = {720, 1620};
    // vector<double> fit_maxs = {1100, 2000};
    // TFile *finp = new TFile("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/Calib/Y88_burn_0078_eff.root", "READ");
    // TFile *fout = new TFile("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/Calib/Y88_burn_0078_Ediff.root", "RECREATE");
    // std::ofstream foutCSV("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/Calib/Y88_burn_0078_Ediff.csv", std::ios::out);

    vector<double> Epeaks = {661.657}; // Cs137 661.657 keV
    vector<double> fit_mins = {450};
    vector<double> fit_mins_mod = {500};
    vector<double> fit_maxs = {800};
    TFile *finp = new TFile("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/Calib/Cs137_burn_0079_eff.root", "READ");
    TFile *fout = new TFile("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/Calib/Cs137_burn_0079_Ediff.root", "RECREATE");
    std::ofstream foutCSV("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/Calib/Cs137_burn_0079_Ediff.csv", std::ios::out);

    TH2F *enF_id = (TH2F *)finp->Get("enF_id");
    TCanvas *c1 = new TCanvas("c1", "Calibrated Energy", 900, 600);
    int nbx = enF_id->GetNbinsX();

    cout << "Number of det Bins: " << nbx << endl;

    for (int det = 0; det < nbx; det++)
    {
        TH1 *hp = enF_id->ProjectionY(Form("hp_det%d", det), det + 1, det + 1);
        hp->SetDirectory(0); // disconnects the histogram hp from the opened ROOT file
        if (hp->Integral() < 100)
        {
            delete hp;
            continue;
        }
        hp->Rebin(10);

        vector<double> means;
        vector<double> sigmas;
        vector<double> EnergyDiffs;
        vector<double> EnergyDiffsPercent;
        vector<int> FitStatuses;
        vector<int> ReFitStatuses;

        cout << "Detector " << det << ": " << endl;

        for (size_t i = 0; i < Epeaks.size(); i++)
        {
            TF1 *g1 = new TF1(Form("g%d", static_cast<int>(i + 1)), "gaus(0)", fit_mins[i], fit_maxs[i]);
            g1->SetParameter(1, Epeaks[i]);
            g1->SetParameter(2, 50);
            hp->Fit(g1, "RQN"); // "N" : to not draw the fit result on the histogram
            double rmean = g1->GetParameter(1);
            double rsigma = g1->GetParameter(2);
            delete g1;

            TF1 *ge = new TF1(Form("gp%d", static_cast<int>(i + 1)), "gaus(0)+expo(3)", fit_mins[i], fit_maxs[i]);
            // ge->SetParameter(1, Epeaks[i]);
            ge->SetParameter(1, rmean);
            ge->SetParameter(2, 50);
            TFitResultPtr res = hp->Fit(ge, "RQ+"); // "+" : to add multiple fits on the same histogram
            int fitStatus = res;

            int refitStatus = 0;
            if (fitStatus != 0)
            {
                cout << "Warning: Fit for detector " << det << " peak " << i + 1 << " did not converge! Fit status: " << fitStatus << endl;
                cout << "Refitting again with slightly different range" << endl;

                delete ge;

                TF1 *ge = new TF1(Form("gp%d", static_cast<int>(i + 1)), "gaus(0)+expo(3)", fit_mins_mod[i], fit_maxs[i]);
                // ge->SetParameter(1, Epeaks[i]);
                ge->SetParameter(1, rmean);
                ge->SetParameter(2, 50);
                TFitResultPtr res = hp->Fit(ge, "RQ+"); // "+" : to add multiple fits on the same histogram
                refitStatus = res;
                cout << "Refit status: " << refitStatus << endl;
            }

            double sigma = ge->GetParameter(2);
            double mean = ge->GetParameter(1);
            double energyDiff = Epeaks[i] - mean;
            double energyDiffPercent = energyDiff / Epeaks[i] * 100.0;

            means.push_back(mean);
            sigmas.push_back(sigma);
            EnergyDiffs.push_back(energyDiff);
            EnergyDiffsPercent.push_back(energyDiffPercent);
            FitStatuses.push_back(fitStatus);
            ReFitStatuses.push_back(refitStatus);

            delete ge;
        }

        for (size_t i = 0; i < means.size(); i++)
        {
            cout << "  Peak " << i + 1 << ": Mean Energy = " << means[i]
                 << " keV, Sigma = " << sigmas[i] << " keV"
                 << ", Energy Diff = " << EnergyDiffs[i] << " keV, "
                 << "EnergyDiff percent = " << EnergyDiffsPercent[i] << " %" << endl;
        }

        // Save data to CSV
        savePeakDataCSV(foutCSV, det, means, sigmas, EnergyDiffs, FitStatuses, ReFitStatuses);

        // Draw the histogram with fit results
        hp->Draw();
        c1->Update();

        // save the fitted histogram in root file
        fout->cd();
        hp->Write(Form("calib_energy_fit_det%d", det));

        delete hp;
    }
}

// this function is particularly for Co60
void calibCoEdiff()
{
    vector<double> Epeaks = {1173.240, 1332.508}; // Co60 1173.240 keV, 1332.508 keV
    vector<double> fit_mins = {1000};
    vector<double> fit_maxs = {1450};

    TFile *finp = new TFile("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/Calib/Co60_burn_0077_eff.root", "READ");
    TFile *fout = new TFile("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/Calib/Co60_burn_0077_Ediff.root", "RECREATE");
    std::ofstream foutCSV("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/Calib/Co60_burn_0077_Ediff.csv", std::ios::out);

    TH2F *enF_id = (TH2F *)finp->Get("enF_id");
    TCanvas *c1 = new TCanvas("c1", "Calibrated Energy", 900, 600);
    int nbx = enF_id->GetNbinsX();

    cout << "Number of det Bins: " << nbx << endl;

    for (int det = 0; det < nbx; det++)
    {
        TH1 *hp = enF_id->ProjectionY(Form("hp_det%d", det), det + 1, det + 1);
        hp->SetDirectory(0); // disconnects the histogram hp from the opened ROOT file
        if (hp->Integral() < 100)
        {
            delete hp;
            continue;
        }
        hp->Rebin(10); // rebin to reduce statistical fluctuation

        vector<double> means;
        vector<double> sigmas;
        vector<double> EnergyDiffs;
        vector<double> EnergyDiffsPercent;
        vector<int> FitStatuses;
        vector<int> ReFitStatuses;

        TF1 *dGausExpo = new TF1(
            "doubleGausExpo",
            "[0]*exp(-0.5*((x-[1])/[2])^2) + \
             [3]*exp(-0.5*((x-[4])/[5])^2) + \
             [6]*exp([7]*x)",
            fit_mins[0], fit_maxs[0]);

        dGausExpo->SetParameter(1, Epeaks[0]);
        dGausExpo->SetParameter(2, 50);
        dGausExpo->SetParameter(4, Epeaks[1]);
        dGausExpo->SetParameter(5, 50);
        dGausExpo->SetParameter(6, hp->GetBinContent(hp->FindBin(fit_mins[0]))); // initial guess for the exponential background normalization
        dGausExpo->SetParameter(7, -0.001);                                      // initial guess for the exponential background slope

        cout << "Detector " << det << ": " << endl;

        TFitResultPtr res = hp->Fit(dGausExpo, "R");
        int fitStatus = res;
        FitStatuses.push_back(fitStatus);
        int refitStatus = 0;
        ReFitStatuses.push_back(refitStatus);

        for (size_t i = 0; i < Epeaks.size(); i++)
        {
            double mean = dGausExpo->GetParameter(1 + i * 3);
            double sigma = dGausExpo->GetParameter(2 + i * 3);
            double energyDiff = Epeaks[i] - mean;
            double energyDiffPercent = energyDiff / Epeaks[i] * 100.0;

            means.push_back(mean);
            sigmas.push_back(sigma);
            EnergyDiffs.push_back(energyDiff);
            EnergyDiffsPercent.push_back(energyDiffPercent);
        }

        // Print results
        for (size_t i = 0; i < means.size(); i++)
        {
            cout << "  Peak " << i + 1 << ": Mean Energy = " << means[i]
                 << " keV, Sigma = " << sigmas[i] << " keV"
                 << ", Energy Diff = " << EnergyDiffs[i] << " keV, "
                 << "EnergyDiff percent = " << EnergyDiffsPercent[i] << " %"
                 << "\nFit Status: " << fitStatus << endl;
        }

        // Save data to CSV
        savePeakDataCSV(foutCSV, det, means, sigmas, EnergyDiffs, FitStatuses, ReFitStatuses);

        hp->Draw();
        c1->Update();

        // save the fitted histogram in root file
        fout->cd();
        hp->Write(Form("calib_energy_fit_det%d", det));

        delete hp;
    }
}

void savePeakDataCSV(std::ofstream &foutCSV, int det,
                     const std::vector<double> &mean,
                     const std::vector<double> &sigma,
                     const std::vector<double> &energyDiffs,
                     const std::vector<int> &fitStatuses,
                     const std::vector<int> &refitStatuses)
{
    if (!foutCSV.is_open())
    {
        std::cerr << "Error: cannot write to CSV file!" << std::endl;
        return;
    }

    // Write header only once
    static bool header_written = false;
    if (!header_written)
    {
        foutCSV << "DetectorID";
        for (size_t i = 0; i < mean.size(); ++i)
        {
            foutCSV << "," << Form("Mean%d", static_cast<int>(i + 1))
                    << "," << Form("Sigma%d", static_cast<int>(i + 1));
        }
        for (size_t i = 0; i < energyDiffs.size(); ++i)
        {
            foutCSV << "," << Form("EnergyDiff%d", static_cast<int>(i + 1));
        }
        for (size_t i = 0; i < fitStatuses.size(); ++i)
        {
            foutCSV << "," << Form("FitStatus%d", static_cast<int>(i + 1));
        }
        for (size_t i = 0; i < refitStatuses.size(); ++i)
        {
            foutCSV << "," << Form("ReFitStatus%d", static_cast<int>(i + 1));
        }
        foutCSV << "\n";
        header_written = true;
    }

    // Write data row
    foutCSV << det;
    for (size_t i = 0; i < mean.size(); ++i)
    {
        foutCSV << "," << mean[i] << "," << sigma[i];
    }
    for (size_t i = 0; i < energyDiffs.size(); ++i)
    {
        foutCSV << "," << energyDiffs[i];
    }
    for (size_t i = 0; i < fitStatuses.size(); ++i)
    {
        foutCSV << "," << fitStatuses[i];
    }
    for (size_t i = 0; i < refitStatuses.size(); i++)
    {
        foutCSV << "," << refitStatuses[i];
    }

    foutCSV << "\n";
}