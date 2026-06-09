#include "TFile.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

struct DetectorData
{
    int id;
    double mpv;
};

void timeAlign()
{
    ifstream finpcsv("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/timeOffsets_all_iso_6000_g1000.csv");
    ofstream fouttxt("/u/ddas/software/Salvador/dali_toffset_all_iso_g1000_Au6000.txt");
    TFile *fin = TFile::Open("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/pers_tr_all_iso_6000.root");

    // ifstream finpcsv("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/timeOffsets_fix_by_hand.csv");
    // ifstream finpcsv("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/timeOffsets.csv");
    // ofstream fouttxt("/u/ddas/software/Salvador/dali_toffset.dat");
    // TFile *fin = TFile::Open("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/pers_tr_all_2000.root");
    // TFile *fin = TFile::Open("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/pers_tr_in49Kout49K_all_2000.root");
    if (!fin || fin->IsZombie())
    {
        Error("timeCalib", "Cannot open ROOT file");
        return;
    }

    TFile *fout = TFile::Open("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/timeAligned_all_6000_fix_check.root", "RECREATE");
    // TFile *fout = TFile::Open("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/pers/timeAligned_all_2000_fix_check.root", "RECREATE");

    vector<DetectorData> data;
    map<int, double> mpvMap;

    string line; // declare line

    getline(finpcsv, line); // Skip header

    while (getline(finpcsv, line))
    {
        stringstream ss(line);
        string token;

        // Read DetectorID
        getline(ss, token, ',');
        int id = stoi(token);
        // Read MPV/Mean
        getline(ss, token, ',');
        double mpv = stod(token);
        // Read Sigma
        getline(ss, token, ',');
        double sigma = stod(token);
        // Read TimeOffset
        getline(ss, token, ',');
        double timeOffset = stod(token);

        data.push_back({id, mpv}); // cleaner storage

        if (-mpv != timeOffset)
        {
            cout << "Warning: Time offset for DetectorID " << id << " does not match -MPV. Check data." << endl;
        }
    }
    // print
    for (const auto &d : data)
    {
        cout << "DetectorID: " << d.id << ", MPV/Mean: " << d.mpv << endl;
        mpvMap[d.id] = d.mpv;
        fouttxt << "DALI.Toffset." << d.id << ":\t" << d.mpv << endl;
        // fouttxt << "Dali.Toffset." << d.id << ":\t" << 0 << endl;
        fouttxt << "DALI.TimGain." << d.id << ":\t" << 1.000 << endl;
    }
    // cout << "ID=415: " << mpvMap[415] << endl;

    TList *hlist = new TList();
    TH2F *h2 = (TH2F *)fin->Get("time_id_g1000");
    // TH2F *h2 = (TH2F *)fin->Get("time_id_g700");
    TH2F *h2_le500 = (TH2F *)fin->Get("time_id_le500");
    h2->GetYaxis()->SetRangeUser(-850, -1300);
    TH2F *h2_timeCalib = new TH2F("h2_timeCalib", "Time Calibration (E>500keV) ;Detector ID;Time Offset (ns)", 500, 0, 500, 1000, -200, 200);
    hlist->Add(h2_timeCalib);
    TH2F *h2_timeCalib_le500 = new TH2F("h2_timeCalib_le500", "Time Calibration (E<500keV);Detector ID;Time Offset (ns)", 500, 0, 500, 1000, -200, 200);
    hlist->Add(h2_timeCalib_le500);

    for (int i = 0; i < 500; i++)
    {
        TH1F *hp = (TH1F *)h2->ProjectionY(Form("hp%d", i), i + 1, i + 1);
        if (hp->Integral() < 10)
        {
            // cout << "No Data for Det: " << i << endl;
            continue;
        }

        for (int bin = 1; bin <= hp->GetNbinsX(); bin++)
        {
            double t = hp->GetXaxis()->GetBinCenter(bin);
            double content = hp->GetBinContent(bin);
            double t_calib = t - mpvMap[i]; // Calibrated time
            h2_timeCalib->Fill(i, t_calib, content);
        }

        // Calculate for E < 500 keV
        TH1F *hp_le500 = (TH1F *)h2_le500->ProjectionY(Form("hp_le500_%d", i), i + 1, i + 1);
        for (int bin = 1; bin <= hp_le500->GetNbinsX(); bin++)
        {
            double t = hp_le500->GetXaxis()->GetBinCenter(bin);
            double content = hp_le500->GetBinContent(bin);
            double t_calib = t - mpvMap[i]; // Calibrated time
            h2_timeCalib_le500->Fill(i, t_calib, content);
        }
    }

    TCanvas *c1 = new TCanvas("c1", "Time Calibration", 1200, 800);
    h2_timeCalib->Draw("colz");

    fout->cd();
    hlist->Write();
}