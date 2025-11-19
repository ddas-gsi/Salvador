#include <iostream>
#include <iomanip>
#include <string>
#include <sys/time.h>
#include <signal.h>
#include "TMath.h"
#include "TFile.h"
#include "TChain.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TKey.h"
#include "TStopwatch.h"
#include "CommandLineInterface.hh"
#include "FocalPlane.hh"
#include "PPAC.hh"
#include "Beam.hh"
#include "DALI.hh"
#include "Globaldefs.h"

#include "TGraph.h"
#include "TROOT.h"
#include "TCutG.h"
#include <vector>
#include <map>

using namespace TMath;
using namespace std;
int DALIIDS = 500;
bool signal_received = false;
void signalhandler(int sig);
double get_time();
int main(int argc, char *argv[])
{
  int RunID = 1010; // Run Number of RIBF249

  int total_50Ca20_pid2 = 0;
  int total_49K19_pid2 = 0;
  int inCut_50Ca20_pid2 = 0;
  int inCut_49K19_pid2 = 0;
  int total_50Ca20_pid5 = 0;
  int total_49K19_pid5 = 0;
  int inCut_50Ca20_pid5 = 0;
  int inCut_49K19_pid5 = 0;

  double time_start = get_time();
  TStopwatch timer;
  timer.Start();
  signal(SIGINT, signalhandler);
  cout << "\"The Burning Giraffe\" (1937), Salvador Dali" << endl;
  cout << "Histogramms for DALI" << endl;
  int LastEvent = -1;
  int Verbose = 0;
  vector<char *> InputFiles;
  char *OutFile = NULL;
  char *TreeName = (char *)"tr";

  int LoadCutID = 0;    // default 0 means load NO cuts
  int FocalPlaneID = 0; // default -1 means load all focal planes

  // Read in the command line arguments
  CommandLineInterface *interface = new CommandLineInterface();
  interface->Add("-i", "input files", &InputFiles);
  interface->Add("-o", "output file", &OutFile);
  interface->Add("-tn", "name of the tree", &TreeName);
  interface->Add("-le", "last event to be read", &LastEvent);
  interface->Add("-v", "verbose level", &Verbose);
  interface->Add("-lcx", "load cut id 0, 1, 2, 3, -1", &LoadCutID);
  interface->Add("-fid", "focal plane id 3,5,7,8,11", &FocalPlaneID);
  interface->Add("-r", "Batch number", &RunID);
  interface->CheckFlags(argc, argv);
  // Complain about missing mandatory arguments
  if (InputFiles.size() == 0)
  {
    cout << "No input file given " << endl;
    return 1;
  }
  if (OutFile == NULL)
  {
    cout << "No output ROOT file given " << endl;
    OutFile = (char *)"test.root";
  }
  cout << "input file(s):" << endl;
  for (unsigned int i = 0; i < InputFiles.size(); i++)
  {
    cout << InputFiles[i] << endl;
  }
  cout << "output file: " << OutFile << endl;

  cout << "Batch Number: " << RunID << endl;

  ////////// Decide which cuts to load ////////////
  char load_dT_vs_logQ_cuts; // LoadCutID: 1
  char load_logQ_vs_X_cuts;  // LoadCutID: 2
  char load_dT_vs_X_cuts;    // LoadCutID: 3

  if (LoadCutID == 0)
  {
    // Use defaults: load no cuts
    load_dT_vs_logQ_cuts = 'n';
    load_logQ_vs_X_cuts = 'n';
    load_dT_vs_X_cuts = 'n';
    cout << "loading no cuts" << endl;
  }
  else if (LoadCutID == 1)
  {
    load_dT_vs_logQ_cuts = 'y';
    load_logQ_vs_X_cuts = 'n';
    load_dT_vs_X_cuts = 'n';
    cout << "loading dT_vs_logQ cuts only" << endl;
  }
  else if (LoadCutID == 2)
  {
    load_dT_vs_logQ_cuts = 'n';
    load_logQ_vs_X_cuts = 'y';
    load_dT_vs_X_cuts = 'n';
    cout << "loading logQ_vs_X cuts only" << endl;
  }
  else if (LoadCutID == 3)
  {
    load_dT_vs_logQ_cuts = 'n';
    load_logQ_vs_X_cuts = 'n';
    load_dT_vs_X_cuts = 'y';
    cout << "loading dT_vs_X cuts only" << endl;
  }
  else if (LoadCutID == -1)
  {
    load_dT_vs_logQ_cuts = 'y';
    load_logQ_vs_X_cuts = 'y';
    load_dT_vs_X_cuts = 'y';
    cout << "loading all cuts" << endl;
  }
  else
  {
    cout << "Error: load cut id must be 0, 1, 2, 3 or -1" << endl;
    return 2;
  }

  //////////// Mapping for focal plane ids ////////////
  vector<int> fpIDs = {};

  if (FocalPlaneID == 0)
  {
    fpIDs = {};
    cout << "loading cuts for focal planes: None" << endl;
  }
  else if (FocalPlaneID == -1)
  {
    fpIDs = {3, 7, 8, 11}; // defaults
    cout << "loading cuts for focal planes: All" << endl;
  }
  else
  {
    string fid_str = to_string(abs(FocalPlaneID));

    for (size_t i = 0; i < fid_str.size(); ++i)
    {
      if (fid_str[i] == '1' && i + 1 < fid_str.size() && fid_str[i + 1] == '1')
      {
        fpIDs.push_back(11);
        ++i; // skip next '1'
      }
      else if (isdigit(fid_str[i]))
      {
        int id = fid_str[i] - '0';
        if (id == 3 || id == 5 || id == 7 || id == 8)
          fpIDs.push_back(id);
        else
          cout << "Warning: ignoring invalid focal plane digit " << id << endl;
      }
    }

    if (fpIDs.empty())
    {
      cout << "Error: no valid focal planes found in -fid " << FocalPlaneID << endl;
      return 2;
    }

    cout << "loading cuts for focal planes: ";
    for (auto id : fpIDs)
      cout << id << " ";
    cout << endl;
  }
  //////////// --------------------------- ////////////

  // Define hlist to hold histograms and cuts
  TList *hlist = new TList();

  /////////// gCUT files for Plastic ////////////
  // std::map<const char *, std::map<int, TCutG *>> PlasticCuts; // key = focal-plane id (3,5,7,...) value=cut pointer
  std::map<std::string, std::map<int, TCutG *>> PlasticCuts; // key = focal-plane id (3,5,7,...) value=cut pointer
  std::map<int, TCutG *> dT_vs_logQ_cuts;
  std::map<int, TCutG *> logQ_vs_X_cuts;
  std::map<int, TCutG *> dT_vs_X_cuts;

  // Load cut files for each focal plane id and put into PlasticCuts
  for (int id : fpIDs)
  {
    if (load_dT_vs_logQ_cuts == 'y')
    {
      cout << "Loading dT_vs_logQ cuts for FocalPlane " << id << endl;
      TString file = Form("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/salva/cuts/plastic/%d/%d_dT_vs_logQ_%d.cxx", RunID, RunID, id);
      gROOT->ProcessLine(Form(".L %s", file.Data()));
      TObject *obj = gROOT->FindObject(Form("dT_vs_logQ_%d", id));
      if (obj && obj->InheritsFrom("TCutG"))
      {
        dT_vs_logQ_cuts[id] = (TCutG *)obj;
        if (dT_vs_logQ_cuts[id]) // hlist the cut only if it was loaded
        {
          TCutG *clone = (TCutG *)dT_vs_logQ_cuts[id]->Clone(Form("%s_cut", dT_vs_logQ_cuts[id]->GetName()));
          hlist->Add(clone);
        }
        if (Verbose)
          cout << "Loaded dT_vs_logQ cut for FocalPlane " << id << endl;
      }
      else
      {
        dT_vs_logQ_cuts[id] = nullptr;
        cout << "Warning: no dT_vs_logQ CUTG found for FocalPlane " << id << " (file: " << file.Data() << ")" << endl;
      }
      PlasticCuts["dT_vs_logQ_cuts"] = dT_vs_logQ_cuts;
    }
    if (load_logQ_vs_X_cuts == 'y')
    {
      cout << "Loading logQ_vs_X cuts for FocalPlane " << id << endl;
      TString file = Form("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/salva/cuts/plastic/%d/%d_logQ_vs_X_%d.cxx", RunID, RunID, id);
      gROOT->ProcessLine(Form(".L %s", file.Data()));
      TObject *obj = gROOT->FindObject(Form("logQ_vs_X_%d", id));
      if (obj && obj->InheritsFrom("TCutG"))
      {
        logQ_vs_X_cuts[id] = (TCutG *)obj;
        if (logQ_vs_X_cuts[id]) // hlist the cut only if it was loaded
        {
          TCutG *clone = (TCutG *)logQ_vs_X_cuts[id]->Clone(Form("%s_cut", logQ_vs_X_cuts[id]->GetName()));
          hlist->Add(clone);
        }
        if (Verbose)
          cout << "Loaded logQ_vs_X cut for FocalPlane " << id << endl;
      }
      else
      {
        logQ_vs_X_cuts[id] = nullptr;
        cout << "Warning: no logQ_vs_X CUTG found for FocalPlane " << id << " (file: " << file.Data() << ")" << endl;
      }
      PlasticCuts["logQ_vs_X_cuts"] = logQ_vs_X_cuts;
    }
    if (load_dT_vs_X_cuts == 'y')
    {
      cout << "Loading dT_vs_X cuts for FocalPlane " << id << endl;
      TString file = Form("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/salva/cuts/plastic/%d/%d_dT_vs_X_%d.cxx", RunID, RunID, id);
      gROOT->ProcessLine(Form(".L %s", file.Data()));
      TObject *obj = gROOT->FindObject(Form("dT_vs_X_%d", id));
      if (obj && obj->InheritsFrom("TCutG"))
      {
        dT_vs_X_cuts[id] = (TCutG *)obj;
        if (dT_vs_X_cuts[id]) // hlist the cut only if it was loaded
        {
          TCutG *clone = (TCutG *)dT_vs_X_cuts[id]->Clone(Form("%s_cut", dT_vs_X_cuts[id]->GetName()));
          hlist->Add(clone);
        }
        if (Verbose)
          cout << "Loaded dT_vs_X cut for FocalPlane " << id << endl;
      }
      else
      {
        dT_vs_X_cuts[id] = nullptr;
        cout << "Warning: no dT_vs_X CUTG found for FocalPlane " << id << " (file: " << file.Data() << ")" << endl;
      }
      PlasticCuts["dT_vs_X_cuts"] = dT_vs_X_cuts;
    }
  }

  //////// load pid cuts from file for ***PID 2*** ////////
  TString pid50Ca20_pid2f1 = Form("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/salva/cuts/pid/%d_50Ca20_pid2.cxx", RunID);
  gROOT->ProcessLine(Form(".L %s", pid50Ca20_pid2f1.Data()));
  TCutG *pid50Ca20_pid2 = (TCutG *)gROOT->FindObject("50Ca20_pid2");
  if (pid50Ca20_pid2)
  {
    TCutG *clone = (TCutG *)pid50Ca20_pid2->Clone(Form("%s_cut", pid50Ca20_pid2->GetName()));
    hlist->Add(clone);
    cout << "Loaded 50Ca20 PID 2 cut for Run Number: " << RunID << endl;
  }
  else if (!pid50Ca20_pid2)
  {
    cerr << "Error: could not find PID 2 cut 50Ca20" << endl;
    return 2;
  }
  TString pid49K19_pid2f2 = Form("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/salva/cuts/pid/%d_49K19_pid2.cxx", RunID);
  gROOT->ProcessLine(Form(".L %s", pid49K19_pid2f2.Data()));
  TCutG *pid49K19_pid2 = (TCutG *)gROOT->FindObject("49K19_pid2");
  if (pid49K19_pid2)
  {
    TCutG *clone = (TCutG *)pid49K19_pid2->Clone(Form("%s_cut", pid49K19_pid2->GetName()));
    hlist->Add(clone);
    cout << "Loaded 49K19 PID 2 cut for Run Number: " << RunID << endl;
  }
  else if (!pid49K19_pid2)
  {
    cerr << "Error: could not find PID 2 cut 49K19" << endl;
    return 2;
  }

  //////// load pid cuts from file for ***PID 5*** ////////
  TString pid50Ca20_pid5f1 = Form("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/salva/cuts/pid/%d_50Ca20_pid5.cxx", RunID);
  gROOT->ProcessLine(Form(".L %s", pid50Ca20_pid5f1.Data()));
  TCutG *pid50Ca20_pid5 = (TCutG *)gROOT->FindObject("50Ca20_pid5");
  if (pid50Ca20_pid5)
  {
    TCutG *clone = (TCutG *)pid50Ca20_pid5->Clone(Form("%s_cut", pid50Ca20_pid5->GetName()));
    hlist->Add(clone);
    cout << "Loaded 50Ca20 PID 5 cut for Run Number: " << RunID << endl;
  }
  else if (!pid50Ca20_pid5)
  {
    cerr << "Error: could not find PID 5 cut 50Ca20" << endl;
    return 2;
  }
  TString pid49K19_pid5f2 = Form("/u/ddas/Lustre/gamma/ddas/RIBF249/rootfiles/ddas/salva/cuts/pid/%d_49K19_pid5.cxx", RunID);
  gROOT->ProcessLine(Form(".L %s", pid49K19_pid5f2.Data()));
  TCutG *pid49K19_pid5 = (TCutG *)gROOT->FindObject("49K19_pid5");
  if (pid49K19_pid5)
  {
    TCutG *clone = (TCutG *)pid49K19_pid5->Clone(Form("%s_cut", pid49K19_pid5->GetName()));
    hlist->Add(clone);
    cout << "Loaded 49K19 PID 5 cut for Run Number: " << RunID << endl;
  }
  else if (!pid49K19_pid5)
  {
    cerr << "Error: could not find PID 5 cut 49K19" << endl;
    return 2;
  }

  //////////// Define Histograms ////////////
  // ppacs
  TH2F *tsumx_id = new TH2F("tsumx_id", "tsumx_id", NPPACS, 0, NPPACS, 2500, 0, 250);
  hlist->Add(tsumx_id);
  TH2F *tsumy_id = new TH2F("tsumy_id", "tsumy_id", NPPACS, 0, NPPACS, 2500, 0, 250);
  hlist->Add(tsumy_id);
  TH1F *tsumx[NPPACS];
  TH1F *tsumy[NPPACS];
  TH2F *ppacx_vs_time[NPPACS];
  TH2F *ppacy_vs_time[NPPACS];

  for (unsigned short p = 0; p < NPPACS; p++)
  {
    tsumx[p] = new TH1F(Form("tsumx_%d", p), Form("tsumx_%d", p), 1000, -200, 800);
    hlist->Add(tsumx[p]);
    tsumy[p] = new TH1F(Form("tsumy_%d", p), Form("tsumy_%d", p), 1000, -200, 800);
    hlist->Add(tsumy[p]);
    ppacx_vs_time[p] = new TH2F(Form("ppacx_vs_time_%d", p), Form("ppacx_vs_time_%d", p), 10000, 0, 100e6, 500, -100, 100);
    hlist->Add(ppacx_vs_time[p]);
    ppacy_vs_time[p] = new TH2F(Form("ppacy_vs_time_%d", p), Form("ppacy_vs_time_%d", p), 10000, 0, 100e6, 500, -100, 100);
    hlist->Add(ppacy_vs_time[p]);
  }
  // focal planes
  TH2F *dT_vs_logQ[NFPLANES];
  TH2F *logQ_vs_X[NFPLANES];
  TH2F *dT_vs_X[NFPLANES];
  TH2F *dT_vs_logQ_clean[NFPLANES];
  for (unsigned short f = 0; f < NFPLANES; f++)
  {
    // dT_vs_logQ[f] = new TH2F(Form("dT_vs_logQ_%d", fpID[f]), Form("dT_vs_logQ_%d", fpID[f]), 1000, -5, 5, 300, -3, 3);
    dT_vs_logQ[f] = new TH2F(Form("dT_vs_logQ_%d", fpID[f]), Form("dT_vs_logQ_%d", fpID[f]), 1000, -10, 10, 300, -10, 10);
    hlist->Add(dT_vs_logQ[f]);
    dT_vs_logQ_clean[f] = new TH2F(Form("dT_vs_logQ_clean_%d", fpID[f]), Form("dT_vs_logQ_clean_%d", fpID[f]), 1000, -10, 10, 300, -10, 10); // without the GetTimeL or GetTimeR == -10000
    hlist->Add(dT_vs_logQ_clean[f]);
    logQ_vs_X[f] = new TH2F(Form("logQ_vs_X_%d", fpID[f]), Form("logQ_vs_X_%d", fpID[f]), 1000, -100, 100, 300, -3, 3);
    hlist->Add(logQ_vs_X[f]);
    dT_vs_X[f] = new TH2F(Form("dT_vs_X_%d", fpID[f]), Form("dT_vs_X_%d", fpID[f]), 1000, -100, 100, 1000, -10, 10);
    hlist->Add(dT_vs_X[f]);
  }
  // beam
  TH1F *beta[3];
  for (unsigned short b = 0; b < 3; b++)
  {
    beta[b] = new TH1F(Form("beta_%d", b), Form("beta_%d", b), 1000, 0, 1);
    hlist->Add(beta[b]);
  }
  TH1F *delta[4];
  for (unsigned short b = 0; b < 4; b++)
  {
    delta[b] = new TH1F(Form("delta_%d", b), Form("delta_%d", b), 1000, -10, 10);
    hlist->Add(delta[b]);
  }

  // dali
  TH2F *adc_id = new TH2F("adc_id", "adc_id", DALIIDS, 0, DALIIDS, 5000, 0, 5000);
  hlist->Add(adc_id);
  TH2F *en_id = new TH2F("en_id", "en_id", DALIIDS, 0, DALIIDS, 500, 0, 2000);
  hlist->Add(en_id);
  TH2F *enF_id = new TH2F("enF_id", "enF_id", DALIIDS, 0, DALIIDS, 2000, 0, 2000);
  hlist->Add(enF_id);
  TH2F *enDC_id = new TH2F("enDC_id", "enDC_id", DALIIDS, 0, DALIIDS, 500, 0, 2000);
  hlist->Add(enDC_id);
  TH2F *time_id = new TH2F("time_id", "time_id", DALIIDS, 0, DALIIDS, 2000, -2000, 0);
  hlist->Add(time_id);
  TH2F *time_id_g = new TH2F("time_id_g", "time_id_g", DALIIDS, 0, DALIIDS, 2000, -2000, 0);
  hlist->Add(time_id_g);
  TH2F *toffset_id = new TH2F("toffset_id", "toffset_id", DALIIDS, 0, DALIIDS, 1000, -200, 800);
  hlist->Add(toffset_id);

  // PID
  TH2F *z_vs_aoq[6];
  TH2F *z_vs_aoqc[6];
  for (unsigned short f = 0; f < 6; f++)
  {
    z_vs_aoq[f] = new TH2F(Form("z_vs_aoq_%d", f), Form("z_vs_aoq_%d", f), 1000, 2.2, 2.8, 1000, 0, 40);
    hlist->Add(z_vs_aoq[f]);
    z_vs_aoqc[f] = new TH2F(Form("z_vs_aoqc_%d", f), Form("z_vs_aoqc_%d", f), 1000, 2.2, 2.8, 1000, 0, 40);
    hlist->Add(z_vs_aoqc[f]);
  }

  // AoQ Corrections plots for different focal planes
  TH2F *aoq2_vs_x_fp[NFPLANES];
  TH2F *aoq2_vs_y_fp[NFPLANES];
  TH2F *aoq2_vs_a_fp[NFPLANES];
  TH2F *aoq2_vs_b_fp[NFPLANES];
  TH2F *aoq5_vs_x_fp[NFPLANES];
  TH2F *aoq5_vs_y_fp[NFPLANES];
  TH2F *aoq5_vs_a_fp[NFPLANES];
  TH2F *aoq5_vs_b_fp[NFPLANES];

  for (unsigned short f = 0; f < NFPLANES; f++)
  {
    aoq2_vs_x_fp[f] = new TH2F(Form("aoq2_vs_x_fp%d", fpID[f]), Form("aoq2_vs_x_fp%d", fpID[f]), 1000, -100, 100, 1000, 2.2, 2.8);
    hlist->Add(aoq2_vs_x_fp[f]);
    aoq2_vs_y_fp[f] = new TH2F(Form("aoq2_vs_y_fp%d", fpID[f]), Form("aoq2_vs_y_fp%d", fpID[f]), 1000, -50, 50, 1000, 2.2, 2.8);
    hlist->Add(aoq2_vs_y_fp[f]);
    aoq2_vs_a_fp[f] = new TH2F(Form("aoq2_vs_a_fp%d", fpID[f]), Form("aoq2_vs_a_fp%d", fpID[f]), 1000, -50, 50, 1000, 2.2, 2.8);
    hlist->Add(aoq2_vs_a_fp[f]);
    aoq2_vs_b_fp[f] = new TH2F(Form("aoq2_vs_b_fp%d", fpID[f]), Form("aoq2_vs_b_fp%d", fpID[f]), 1000, -50, 50, 1000, 2.2, 2.8);
    hlist->Add(aoq2_vs_b_fp[f]);

    aoq5_vs_x_fp[f] = new TH2F(Form("aoq5_vs_x_fp%d", fpID[f]), Form("aoq5_vs_x_fp%d", fpID[f]), 1000, -100, 100, 1000, 2.2, 2.8);
    hlist->Add(aoq5_vs_x_fp[f]);
    aoq5_vs_y_fp[f] = new TH2F(Form("aoq5_vs_y_fp%d", fpID[f]), Form("aoq5_vs_y_fp%d", fpID[f]), 1000, -50, 50, 1000, 2.2, 2.8);
    hlist->Add(aoq5_vs_y_fp[f]);
    aoq5_vs_a_fp[f] = new TH2F(Form("aoq5_vs_a_fp%d", fpID[f]), Form("aoq5_vs_a_fp%d", fpID[f]), 1000, -50, 50, 1000, 2.2, 2.8);
    hlist->Add(aoq5_vs_a_fp[f]);
    aoq5_vs_b_fp[f] = new TH2F(Form("aoq5_vs_b_fp%d", fpID[f]), Form("aoq5_vs_b_fp%d", fpID[f]), 1000, -50, 50, 1000, 2.2, 2.8);
    hlist->Add(aoq5_vs_b_fp[f]);
  }

  // AoQ Drift corrections against time
  TH2F *aoq2_vs_time = new TH2F("aoq2_vs_time", "aoq2_vs_time", 10000, 0, 100e6, 1000, 2.2, 2.8);
  hlist->Add(aoq2_vs_time);
  TH2F *aoq5_vs_time = new TH2F("aoq5_vs_time", "aoq5_vs_time", 10000, 0, 100e6, 1000, 2.2, 2.8);
  hlist->Add(aoq5_vs_time);

  // AoQ Abberation corrections
  TH2F *aoq2_vs_y_fp3_corr = new TH2F("aoq2_vs_y_fp3_corr", "aoq2_vs_y_fp3_corr", 1000, -25, 20, 1000, 2.2, 2.8);
  hlist->Add(aoq2_vs_y_fp3_corr);
  TH2F *z_vs_aoq2_corr = new TH2F("z_vs_aoq2_corr", "z_vs_aoq2_corr", 1000, 2.2, 2.8, 1000, 0, 40);
  hlist->Add(z_vs_aoq2_corr);
  TH1F *aoq2_h1 = new TH1F("aoq2", "aoq2", 1000, 2.46, 2.57);
  hlist->Add(aoq2_h1);
  TH1F *aoq2_corr_h1 = new TH1F("aoq2_corr", "aoq2_corr", 1000, 2.46, 2.57);
  hlist->Add(aoq2_corr_h1);
  TH2F *z_vs_aoq5_corr = new TH2F("z_vs_aoq5_corr", "z_vs_aoq5_corr", 1000, 2.2, 2.8, 1000, 0, 40);
  hlist->Add(z_vs_aoq5_corr);
  TH1F *aoq5_h1 = new TH1F("aoq5", "aoq5", 1000, 2.46, 2.57);
  hlist->Add(aoq5_h1);
  TH1F *aoq5_corr_h1 = new TH1F("aoq5_corr", "aoq5_corr", 1000, 2.46, 2.57);
  hlist->Add(aoq5_corr_h1);

  TChain *tr;
  tr = new TChain(TreeName);
  for (unsigned int i = 0; i < InputFiles.size(); i++)
  {
    tr->Add(InputFiles[i]);
  }
  if (tr == NULL)
  {
    cout << "could not find tree " << TreeName << " in file " << endl;
    for (unsigned int i = 0; i < InputFiles.size(); i++)
    {
      cout << InputFiles[i] << endl;
    }
    return 3;
  }
  PPAC *ppac = new PPAC;
  tr->SetBranchAddress("ppacs", &ppac);
  FocalPlane *fp[NFPLANES];
  for (unsigned short f = 0; f < NFPLANES; f++)
  {
    fp[f] = new FocalPlane;
    tr->SetBranchAddress(Form("fp%d", fpID[f]), &fp[f]);
  }
  Beam *beam = new Beam;
  tr->SetBranchAddress("beam", &beam);
  DALI *dali = new DALI;
  tr->SetBranchAddress("dali", &dali);
  int eventnumber = 0;
  tr->SetBranchAddress("eventnumber", &eventnumber);
  int toteventnumber = 0;
  tr->SetBranchAddress("toteventnumber", &toteventnumber);

  Double_t nentries = tr->GetEntries();
  cout << nentries << " entries in tree" << endl;
  if (nentries < 1)
    return 4;
  if (LastEvent > 0)
    nentries = LastEvent;

  ////////////////// Loop over events //////////////////
  Int_t nbytes = 0;
  Int_t status;
  for (int i = 0; i < nentries; i++)
  {
    if (signal_received)
    {
      break;
    }
    ppac->Clear();
    dali->Clear();
    beam->Clear();
    if (Verbose > 2)
      cout << "getting entry " << i << endl;
    status = tr->GetEvent(i);
    if (Verbose > 2)
      cout << "status " << status << endl;
    if (status == -1)
    {
      cerr << "Error occured, couldn't read entry " << i << " from tree " << tr->GetName() << " in file " << tr->GetFile()->GetName() << endl;
      return 5;
    }
    else if (status == 0)
    {
      cerr << "Error occured, entry " << i << " in tree " << tr->GetName() << " in file " << tr->GetFile()->GetName() << " doesn't exist" << endl;
      return 6;
    }
    nbytes += status;

    //++++++++++++++++++++++++++++++++++++++++++++++++//
    ////////////////// start analysis //////////////////
    //++++++++++++++++++++++++++++++++++++++++++++++++//

    /////////// Define Variables for Plastic ///////////
    float dT[NFPLANES];
    float logQ[NFPLANES];
    for (unsigned short f = 0; f < NFPLANES; f++)
    {
      Plastic *pl = fp[f]->GetPlastic();

      // protect dT calculation against non-positive times
      float ftimeL = pl->GetTimeL();
      float ftimeR = pl->GetTimeR();
      if (ftimeL > 0.0 && ftimeR > 0.0)
        dT[f] = ftimeL - ftimeR;
      else
        dT[f] = NAN; // mark invalid

      // protect log calculation against non-positive charges
      float qL = pl->GetChargeL();
      float qR = pl->GetChargeR();
      if (0.0 < qL && qL < 10000 && 0.0 < qR && qR < 10000) // < 10000 to protect against unphysical large charges
        logQ[f] = log(qL / qR);
      else
        logQ[f] = NAN; // mark invalid
    }

    /////////// Load conditions for Plastics ////////////
    bool passAllCuts = true;
    bool passBRCuts = true;
    bool passZDCuts = true;

    if (load_dT_vs_logQ_cuts == 'y')
    {
      for (auto &kv : dT_vs_logQ_cuts)
      {
        int id = kv.first;      // focal plane id i.e.: 3,7,8,11
        TCutG *cut = kv.second; // cut for this focal plane
        if (cut)                // only if cut is defined
        {
          if (!cut->IsInside(logQ[fpNr(id)], dT[fpNr(id)]) || !isfinite(logQ[fpNr(id)]) || !isfinite(dT[fpNr(id)]))
          {
            passAllCuts = false;
            if (id == 3 || id == 7)
            {
              passBRCuts = false;
            }
            else if (id == 8 || id == 11)
            {
              passZDCuts = false;
            }
            break; // fail early
          }
        }
      }
    }
    if (load_logQ_vs_X_cuts == 'y')
    {
      for (auto &kv : logQ_vs_X_cuts)
      {
        int id = kv.first;      // focal plane id
        TCutG *cut = kv.second; // cut for this focal plane
        if (cut)                // only if cut is defined
        {
          if (!cut->IsInside(fp[fpNr(id)]->GetTrack()->GetX(), logQ[fpNr(id)]) || !isfinite(logQ[fpNr(id)]))
          {
            passAllCuts = false;
            if (id == 3 || id == 7)
            {
              passBRCuts = false;
            }
            else if (id == 8 || id == 11)
            {
              passZDCuts = false;
            }
            break; // fail early
          }
        }
      }
    }
    if (load_dT_vs_X_cuts == 'y')
    {
      for (auto &kv : dT_vs_X_cuts)
      {
        int id = kv.first;      // focal plane id
        TCutG *cut = kv.second; // cut for this focal plane
        if (cut)                // only if cut is defined
        {
          if (!cut->IsInside(fp[fpNr(id)]->GetTrack()->GetX(), dT[fpNr(id)]) || !isfinite(dT[fpNr(id)]))
          {
            passAllCuts = false;
            if (id == 3 || id == 7)
            {
              passBRCuts = false;
            }
            else if (id == 8 || id == 11)
            {
              passZDCuts = false;
            }
            break; // fail early
          }
        }
      }
    }

    //////////// Fill histograms that pass all cuts ////////////
    // if (passZDCuts && passBRCuts)
    if (passAllCuts)
    {

      // ppacs
      for (unsigned short p = 0; p < ppac->GetN(); p++)
      {
        SinglePPAC *sp = ppac->GetPPAC(p);
        tsumx_id->Fill(sp->GetID(), sp->GetTsumX());
        tsumy_id->Fill(sp->GetID(), sp->GetTsumY());
        if (sp->GetID() > -1 && sp->GetID() < NPPACS)
        {
          tsumx[sp->GetID()]->Fill(sp->GetTsumX());
          tsumy[sp->GetID()]->Fill(sp->GetTsumY());
        }
        ppacx_vs_time[sp->GetID()]->Fill(i, sp->GetX());
        ppacy_vs_time[sp->GetID()]->Fill(i, sp->GetY());
      }
      // dali
      for (unsigned short g = 0; g < dali->GetMult(); g++)
      {
        DALIHit *hit = dali->GetHit(g);
        short id = hit->GetID();
        adc_id->Fill(id, hit->GetADC());
        en_id->Fill(id, hit->GetEnergy());
        enF_id->Fill(id, hit->GetEnergy());
        enDC_id->Fill(id, hit->GetDCEnergy());
        time_id->Fill(id, hit->GetTime());
        if (hit->GetEnergy() > 500)
        {
          time_id_g->Fill(id, hit->GetTime());
        }
        toffset_id->Fill(id, hit->GetTOffset());
      }

      // cout << "No of FOCAL Planes: " << NFPLANES << endl;

      // focal planes
      for (unsigned short f = 0; f < NFPLANES; f++)
      {
        Plastic *pl = fp[f]->GetPlastic();
        logQ_vs_X[f]->Fill(fp[f]->GetTrack()->GetX(), log(pl->GetChargeL() / pl->GetChargeR()));
        dT_vs_X[f]->Fill(fp[f]->GetTrack()->GetX(), pl->GetTimeL() - pl->GetTimeR());
        // dT_vs_logQ[f]->Fill(log(pl->GetChargeL() / pl->GetChargeR()), pl->GetTimeL() - pl->GetTimeR());
        dT_vs_logQ[f]->Fill(logQ[f], dT[f]);
      }

      // AoQ Corrections plots    // For the time being commented out
      // for (unsigned short f = 0; f < NFPLANES; f++)
      // {
      //   Track *track = fp[f]->GetTrack();
      //   aoq2_vs_x_fp[f]->Fill(track->GetX(), beam->GetAQ(2));
      //   aoq2_vs_y_fp[f]->Fill(track->GetY(), beam->GetAQ(2));
      //   aoq2_vs_a_fp[f]->Fill(track->GetA(), beam->GetAQ(2));
      //   aoq2_vs_b_fp[f]->Fill(track->GetB(), beam->GetAQ(2));

      //   aoq5_vs_x_fp[f]->Fill(track->GetX(), beam->GetAQ(5));
      //   aoq5_vs_y_fp[f]->Fill(track->GetY(), beam->GetAQ(5));
      //   aoq5_vs_a_fp[f]->Fill(track->GetA(), beam->GetAQ(5));
      //   aoq5_vs_b_fp[f]->Fill(track->GetB(), beam->GetAQ(5));
      // }

      // // ***** AoQ Abberation corrections *****
      // // Modified at line number ~820 to handle BigRIPS and ZeroDegree separately
      // if (fp[fpNr(3)]->GetPlastic()->GetMultihitL() == 1 &&
      //     fp[fpNr(3)]->GetPlastic()->GetMultihitR() == 1 &&
      //     fp[fpNr(7)]->GetPlastic()->GetMultihitL() == 1 &&
      //     fp[fpNr(7)]->GetPlastic()->GetMultihitR() == 1)
      // // (fp[fpNr(3)]->GetPlastic()->GetChargeR() < 5000 &&  // this conditions were already applied when calculating logQ
      // // fp[fpNr(3)]->GetPlastic()->GetChargeL() < 5000 &&)
      // {
      //   // double corr = (2.5 - (2.49978 + 9.76153e-05 * fp[fpNr(3)]->GetTrack()->GetY() + 3.22614e-05 * pow(fp[fpNr(3)]->GetTrack()->GetY(), 2))) +  // corrections using different method. Just for safekeeping kept here.
      //   //               (2.5 - (2.49968 + 3.19712e-05 * fp[fpNr(3)]->GetTrack()->GetX())) +
      //   //               (2.5 - (2.50014 - 1.29684e-05 * fp[fpNr(3)]->GetTrack()->GetA())) +
      //   //               (2.5 - (2.49957 - 1.60998e-05 * fp[fpNr(3)]->GetTrack()->GetB() + 2.85131e-06 * pow(fp[fpNr(3)]->GetTrack()->GetB(), 2))) +
      //   //               (2.5 - (2.49926 - 3.91023e-05 * fp[fpNr(5)]->GetTrack()->GetX())) +
      //   //               (2.5 - (2.49955 + 1.18671e-05 * fp[fpNr(5)]->GetTrack()->GetY() + 1.39958e-05 * pow(fp[fpNr(5)]->GetTrack()->GetY(), 2))) +
      //   //               (2.5 - (2.49975 + 7.47592e-05 * fp[fpNr(5)]->GetTrack()->GetA()));

      //   double aoq2 = beam->GetAQ(2);
      //   double corr2 = 0.00004 * fp[fpNr(5)]->GetTrack()->GetX() +
      //                  (-0.00005 * fp[fpNr(5)]->GetTrack()->GetA()) +
      //                  (+0.0001 * fp[fpNr(7)]->GetTrack()->GetX()) +
      //                  (-0.0001 * fp[fpNr(7)]->GetTrack()->GetA()) +
      //                  (+0.0001 * fp[fpNr(3)]->GetTrack()->GetA()) +
      //                  (-0.000003 * sqrt(fp[fpNr(3)]->GetPlastic()->GetChargeR() * fp[fpNr(3)]->GetPlastic()->GetChargeL())); // use Plastic multihit incase of charge corrections
      //   //  (+0.000025 * sqrt(fp[fpNr(7)]->GetPlastic()->GetChargeR() * fp[fpNr(7)]->GetPlastic()->GetChargeL())) + // use Plastic multihit incase of charge corrections

      //   double aoq2_corr = aoq2 + corr2;

      //   aoq2_h1->Fill(aoq2);
      //   aoq2_corr_h1->Fill(aoq2_corr);
      //   z_vs_aoq2_corr->Fill(aoq2_corr, beam->GetZ(2));
      // }
      // // Modified at line number ~820 to handle BigRIPS and ZeroDegree separately
      // if (fp[fpNr(8)]->GetPlastic()->GetMultihitL() == 1 &&
      //     fp[fpNr(8)]->GetPlastic()->GetMultihitR() == 1 &&
      //     fp[fpNr(11)]->GetPlastic()->GetMultihitL() == 1 &&
      //     fp[fpNr(11)]->GetPlastic()->GetMultihitR() == 1)
      // {
      //   double aoq5 = beam->GetAQ(5);
      //   double corr5 = 0.0002 * fp[fpNr(8)]->GetTrack()->GetX() +
      //                  (+1.75202e-05 * fp[fpNr(9)]->GetTrack()->GetX() + 7.45377e-07 * pow(fp[fpNr(9)]->GetTrack()->GetX(), 2)) +
      //                  (+9.34926e-06 * fp[fpNr(9)]->GetTrack()->GetY() - 0.5238e-06 * pow(fp[fpNr(9)]->GetTrack()->GetY(), 2)) +
      //                  (+0.00002 * fp[fpNr(11)]->GetTrack()->GetA()) +
      //                  (-7.31561e-06 * fp[fpNr(11)]->GetTrack()->GetB() + 4.25615e-06 * pow(fp[fpNr(11)]->GetTrack()->GetB(), 2)) +
      //                  (-3.35538e-07 * fp[fpNr(11)]->GetTrack()->GetY() - 3.78872e-06 * pow(fp[fpNr(11)]->GetTrack()->GetY(), 2));

      //   double aoq5_corr = aoq5 + corr5;

      //   aoq5_h1->Fill(aoq5);
      //   aoq5_corr_h1->Fill(aoq5_corr);
      //   z_vs_aoq5_corr->Fill(aoq5_corr, beam->GetZ(5));
      // }

      // AoQ Drift corrections against time
      if (abs(beam->GetZ(2) - 20.0) < 0.1) // only for Z~20 in PID 2
      {
        aoq2_vs_time->Fill(i, beam->GetAQ(2));
      }
      if (abs(beam->GetZ(5) - 20.0) < 0.1) // only for Z~20 in PID 5
      {
        aoq5_vs_time->Fill(i, beam->GetAQ(5));
      }

      // beam
      for (unsigned short b = 0; b < 3; b++)
        beta[b]->Fill(beam->GetBeta(b));
      for (unsigned short b = 0; b < 4; b++)
        delta[b]->Fill(beam->GetDelta(b));
      for (unsigned short f = 0; f < 6; f++)
      {
        z_vs_aoq[f]->Fill(beam->GetAQ(f), beam->GetZ(f));
        // z_vs_aoqc[f]->Fill(beam->GetCorrAQ(f), beam->GetZ(f));

        if (f == 0 || f == 1 || f == 2) // PID 0,1,2 in BigRIPS
        {
          if (fp[fpNr(3)]->GetPlastic()->GetMultihitL() == 1 &&
              fp[fpNr(3)]->GetPlastic()->GetMultihitR() == 1 &&
              fp[fpNr(7)]->GetPlastic()->GetMultihitL() == 1 &&
              fp[fpNr(7)]->GetPlastic()->GetMultihitR() == 1)
          {
            z_vs_aoqc[f]->Fill(beam->GetCorrAQ(f), beam->GetZ(f));
          }
        }
        if (f == 3 || f == 4 || f == 5) // PID 3,4,5 in ZeroDegree
        {
          if (fp[fpNr(8)]->GetPlastic()->GetMultihitL() == 1 &&
              fp[fpNr(8)]->GetPlastic()->GetMultihitR() == 1 &&
              fp[fpNr(11)]->GetPlastic()->GetMultihitL() == 1 &&
              fp[fpNr(11)]->GetPlastic()->GetMultihitR() == 1)
          {
            z_vs_aoqc[f]->Fill(beam->GetCorrAQ(f), beam->GetZ(f));
          }
        }
      }
    }

    // Separately handle BigRIPS PID
    if (passBRCuts)
    {
      // AoQ Abberation corrections
      if (fp[fpNr(3)]->GetPlastic()->GetMultihitL() == 1 &&
          fp[fpNr(3)]->GetPlastic()->GetMultihitR() == 1 &&
          fp[fpNr(7)]->GetPlastic()->GetMultihitL() == 1 &&
          fp[fpNr(7)]->GetPlastic()->GetMultihitR() == 1)
      {
        double aoq2 = beam->GetAQ(2);
        double corr2 = 0.00004 * fp[fpNr(5)]->GetTrack()->GetX() +
                       (-0.00005 * fp[fpNr(5)]->GetTrack()->GetA()) +
                       (+0.0001 * fp[fpNr(7)]->GetTrack()->GetX()) +
                       (-0.0001 * fp[fpNr(7)]->GetTrack()->GetA()) +
                       (+0.0001 * fp[fpNr(3)]->GetTrack()->GetA()) +
                       (-0.000003 * sqrt(fp[fpNr(3)]->GetPlastic()->GetChargeR() * fp[fpNr(3)]->GetPlastic()->GetChargeL())); // use Plastic multihit incase of charge corrections
        //  (+0.000025 * sqrt(fp[fpNr(7)]->GetPlastic()->GetChargeR() * fp[fpNr(7)]->GetPlastic()->GetChargeL())) + // use Plastic multihit incase of charge corrections

        double aoq2_corr = aoq2 + corr2;

        aoq2_h1->Fill(aoq2);
        aoq2_corr_h1->Fill(aoq2_corr);
        z_vs_aoq2_corr->Fill(aoq2_corr, beam->GetZ(2));

        // count nuclei of interest in PID 2 that pass all plastic cuts
        if (pid50Ca20_pid2->IsInside(beam->GetAQ(2), beam->GetZ(2)))
          inCut_50Ca20_pid2++;
        if (pid49K19_pid2->IsInside(beam->GetAQ(2), beam->GetZ(2)))
          inCut_49K19_pid2++;
      }
    }

    // for Hicari, we have BigRIPS and ZeroDegree plastic cuts together. So in that case use like this. Or use passAllCuts directly.
    if (passZDCuts && passBRCuts)
    // Separately handle ZeroDegree PID
    // if (passZDCuts)
    {
      // AoQ Abberation corrections
      if (fp[fpNr(8)]->GetPlastic()->GetMultihitL() == 1 &&
          fp[fpNr(8)]->GetPlastic()->GetMultihitR() == 1 &&
          fp[fpNr(11)]->GetPlastic()->GetMultihitL() == 1 &&
          fp[fpNr(11)]->GetPlastic()->GetMultihitR() == 1)
      {
        double aoq5 = beam->GetAQ(5);
        double corr5 = 0.0002 * fp[fpNr(8)]->GetTrack()->GetX() +
                       (+1.75202e-05 * fp[fpNr(9)]->GetTrack()->GetX() + 7.45377e-07 * pow(fp[fpNr(9)]->GetTrack()->GetX(), 2)) +
                       (+9.34926e-06 * fp[fpNr(9)]->GetTrack()->GetY() - 0.5238e-06 * pow(fp[fpNr(9)]->GetTrack()->GetY(), 2)) +
                       (+0.00002 * fp[fpNr(11)]->GetTrack()->GetA()) +
                       (-7.31561e-06 * fp[fpNr(11)]->GetTrack()->GetB() + 4.25615e-06 * pow(fp[fpNr(11)]->GetTrack()->GetB(), 2)) +
                       (-3.35538e-07 * fp[fpNr(11)]->GetTrack()->GetY() - 3.78872e-06 * pow(fp[fpNr(11)]->GetTrack()->GetY(), 2));

        double aoq5_corr = aoq5 + corr5;

        aoq5_h1->Fill(aoq5);
        aoq5_corr_h1->Fill(aoq5_corr);
        z_vs_aoq5_corr->Fill(aoq5_corr, beam->GetZ(5));

        // count nuclei of interest in PID 5 that pass all plastic cuts
        if (pid50Ca20_pid5->IsInside(beam->GetAQ(5), beam->GetZ(5)))
          inCut_50Ca20_pid5++;
        if (pid49K19_pid5->IsInside(beam->GetAQ(5), beam->GetZ(5)))
          inCut_49K19_pid5++;
      }
    }

    // Count total nuclei of interest in PID 2 and PID 5 (regardless of plastic cuts)
    for (unsigned short f = 0; f < 6; f++)
    {
      if (f == 2) // PID 2
      {
        if (pid50Ca20_pid2->IsInside(beam->GetAQ(f), beam->GetZ(f)))
          total_50Ca20_pid2++;
        if (pid49K19_pid2->IsInside(beam->GetAQ(f), beam->GetZ(f)))
          total_49K19_pid2++;
      }
      if (f == 5) // PID 5
      {
        if (pid50Ca20_pid5->IsInside(beam->GetAQ(f), beam->GetZ(f)))
          total_50Ca20_pid5++;
        if (pid49K19_pid5->IsInside(beam->GetAQ(f), beam->GetZ(f)))
          total_49K19_pid5++;
      }
    }

    //++++++++++++++++++++++++++++++++++++++++++++++++//
    ///////////////// end of analysis //////////////////
    //++++++++++++++++++++++++++++++++++++++++++++++++//

    if (i % 10000 == 0)
    {
      double time_end = get_time();
      cout << setw(5) << setiosflags(ios::fixed) << setprecision(1) << (100. * i) / nentries << " % done\t" << (Float_t)i / (time_end - time_start) << " events/s " << (nentries - i) * (time_end - time_start) / (Float_t)i << "s to go \r" << flush;
    }
  }
  cout << endl;
  cout << "creating outputfile " << endl;
  TFile *ofile = new TFile(OutFile, "recreate");
  TH1F *h1;
  TH2F *h2;
  TCutG *cut;
  TIter next(hlist);
  TObject *obj;
  while ((obj = next()))
  {
    if (obj->InheritsFrom("TH1"))
    {
      TH1 *h = (TH1 *)obj;
      if (h->GetEntries() > 0)
        h->Write("", TObject::kOverwrite);
    }
    else if (obj->InheritsFrom("TH2"))
    {
      TH2 *h2 = (TH2 *)obj;
      if (h2->GetEntries() > 0)
        h2->Write("", TObject::kOverwrite);
    }
    else if (obj->InheritsFrom("TCutG"))
    {
      TCutG *c = (TCutG *)obj;
      // TCutG doesn't have GetEntries — write it directly
      c->Write("", TObject::kOverwrite);
    }
    else
    {
      // Optional: write other object types or skip them
      // obj->Write("", TObject::kOverwrite);
    }
  }

  ofile->Close();
  double time_end = get_time();
  cout << "Program Run time: " << time_end - time_start << " s." << endl;
  timer.Stop();
  cout << "CPU time: " << timer.CpuTime() << "\tReal time: " << timer.RealTime() << endl;

  if (Verbose)
  {
    cout << "Cut Applied 50Ca20 in PID 2: " << inCut_50Ca20_pid2 << endl;
    cout << "Cut Applied 49K19 in PID 2: " << inCut_49K19_pid2 << endl;
    cout << "Total 50Ca20 in PID 2: " << total_50Ca20_pid2 << endl;
    cout << "Total 49K19 in PID 2: " << total_49K19_pid2 << endl;
    cout << "Cut Applied 50Ca20 in PID 5: " << inCut_50Ca20_pid5 << endl;
    cout << "Cut Applied 49K19 in PID 5: " << inCut_49K19_pid5 << endl;
    cout << "Total 50Ca20 in PID 5: " << total_50Ca20_pid5 << endl;
    cout << "Total 49K19 in PID 5: " << total_49K19_pid5 << endl;
  }

  return 0;
}
void signalhandler(int sig)
{
  if (sig == SIGINT)
  {
    signal_received = true;
  }
}

double get_time()
{
  struct timeval t;
  gettimeofday(&t, NULL);
  double d = t.tv_sec + (double)t.tv_usec / 1000000;
  return d;
}
