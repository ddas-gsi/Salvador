#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sys/time.h>
#include <signal.h>
#include "TArtStoreManager.hh"
#include "TArtEventStore.hh"
// #include "TArtEventInfo.hh"
#include "TArtBigRIPSParameters.hh"
#include "TArtDALIParameters.hh"
#include "TArtCalibPID.hh"
#include "TArtCalibDALI.hh"
#include "TArtCalibPPAC.hh"
#include "TArtCalibPlastic.hh"
#include "TArtCalibIC.hh"
#include "TArtCalibFocalPlane.hh"
#include "TArtEventInfo.hh"
#include "TArtPlastic.hh"
#include "TArtIC.hh"
#include "TArtPPAC.hh"
#include "TArtRecoPID.hh"
#include "TArtRecoRIPS.hh"
#include "TArtRecoTOF.hh"
#include "TArtRecoBeam.hh"
#include "TArtFocalPlane.hh"
#include "TArtTOF.hh"
#include "TArtRIPS.hh"
#include "TArtBeam.hh"
#include "TArtDALINaI.hh"

#include "TFile.h"
#include "TH1F.h"
#include "TTree.h"
#include "TStopwatch.h"

#include "CommandLineInterface.hh"
#include "Settings.hh"

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

bool signal_received = false;
void signalhandler(int sig);
double get_time();
int main(int argc, char *argv[])
{
  double time_start = get_time();
  TStopwatch timer;
  timer.Start();
  signal(SIGINT, signalhandler);
  cout << "\"Metamorphosis of Narcissus\" (1937), Salvador Dali" << endl;
  cout << "Unpacker for BigRIPS/DALI" << endl;
  char *InputFile;
  char *OutputFile = NULL;
  char *SetFile = NULL;
  char *CutFile = NULL;
  int nmax = 0;
  int vl = 0;

  CommandLineInterface *interface = new CommandLineInterface();
  interface->Add("-i", "input file", &InputFile);
  interface->Add("-o", "output file", &OutputFile);
  interface->Add("-s", "settings file", &SetFile);
  interface->Add("-n", "nmax", &nmax);
  interface->Add("-v", "verbose", &vl);
  interface->CheckFlags(argc, argv);

  if (InputFile == NULL || OutputFile == NULL)
  {
    cerr << "You have to provide at least one input file and the output file!" << endl;
    exit(1);
  }
  cout << "input file:" << InputFile << endl;
  cout << "output file: " << OutputFile << endl;
  if (SetFile == NULL)
    cerr << "No settings file! Using standard values" << endl;
  else
    cout << "settings file:" << SetFile << endl;

  cout << "creating outputfile " << endl;
  TFile *outfile = new TFile(OutputFile, "recreate");

  if (outfile->IsZombie())
  {
    return 4;
  }

  // read in the settings
  Settings *set = new Settings(SetFile);

  //////////========= Plastic Cuts Info =========////////////

  int RunID = 1010; // Run Number of RIBF249

  // Define hlist to hold histograms and cuts
  TList *hlist = new TList();

  // Decide which cutType to load
  char load_dT_vs_logQ_cuts = 'y'; // LoadCutID: 1
  char load_logQ_vs_X_cuts = 'y';  // LoadCutID: 2
  char load_dT_vs_X_cuts = 'y';    // LoadCutID: 3

  vector<int> fpIDs = {3, 7, 8, 11}; // default focal planes for Plastics

  // gCUT files for Plastic
  std::map<std::string, std::map<int, TCutG *>> PlasticCuts; // key = focal-plane id (3,7,8,...) value=cut pointer
  std::map<int, TCutG *> dT_vs_logQ_cuts;
  std::map<int, TCutG *> logQ_vs_X_cuts;
  std::map<int, TCutG *> dT_vs_X_cuts;

  if (RunID && RunID != 0)
  {
    // Load cut files for each focal plane id and put into different PlasticCuts
    for (int id : fpIDs)
    {
      if (load_dT_vs_logQ_cuts == 'y')
      {
        // cout << "Loading dT_vs_logQ cuts for FocalPlane " << id << endl;
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
        // cout << "Loading logQ_vs_X cuts for FocalPlane " << id << endl;
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
        // cout << "Loading dT_vs_X cuts for FocalPlane " << id << endl;
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
  }

  //////////--------- Plastic Cuts Info ---------////////////

  // Create StoreManager both for calibration "TArtCalib..." and treatment "TArtReco..."
  //------------------------------------------------------------------------------------
  TArtStoreManager *sman = TArtStoreManager::Instance();

  // Create EventStore to control the loop and get the EventInfo
  //------------------------------------------------------------
  TArtEventStore *estore = new TArtEventStore();
  estore->SetInterrupt(&signal_received);
  estore->Open(InputFile);
  std::cout << "estore ->" << InputFile << std::endl;
  TString *rn = estore->GetRunInfo()->GetRunNumber();
  std::cout << "Runnumber " << rn->Atoi() << std::endl;

  TEnv *evtnumbers = new TEnv(set->EvtNrFile());
  int startevent = evtnumbers->GetValue(Form("Start.Event.Number.%d", rn->Atoi()), 0);
  cout << "run number " << rn->Atoi() << " starts with event nr " << startevent << endl;

  /// load time dependent corrections
  TFile *cor = new TFile(set->TimeCorFile());
  TH1F *f7_cor[2];
  TH1F *f11_cor[2];
  TH1F *br_cor[2];
  TH1F *zd_cor[2];
  if (cor->IsZombie())
  {
    cerr << "ignore previous warning!!" << endl;
    cerr << "File " << set->TimeCorFile() << " not existing, cannot perform time dependent corrections!" << endl;
    f7_cor[0] = NULL;
    f7_cor[1] = NULL;
    f11_cor[0] = NULL;
    f11_cor[1] = NULL;
    br_cor[0] = NULL;
    br_cor[1] = NULL;
    zd_cor[0] = NULL;
    zd_cor[1] = NULL;
  }
  else
  {
    f7_cor[0] = (TH1F *)cor->Get("hoffsF7");
    f7_cor[1] = (TH1F *)cor->Get("hgainF7");
    f11_cor[0] = (TH1F *)cor->Get("hoffsF11");
    f11_cor[1] = (TH1F *)cor->Get("hgainF11");
    br_cor[0] = (TH1F *)cor->Get("hoffsBR");
    br_cor[1] = (TH1F *)cor->Get("hgainBR");
    zd_cor[0] = (TH1F *)cor->Get("hoffsZD");
    zd_cor[1] = (TH1F *)cor->Get("hgainZD");

    outfile->cd();
    for (int i = 0; i < 2; i++)
    {
      f7_cor[i]->Write();
      f11_cor[i]->Write();
      br_cor[i]->Write();
      zd_cor[i]->Write();
    }
  }
  outfile->cd();

  int event = startevent;

  // Create BigRIPSParameters to get Plastics, PPACs, ICs and FocalPlanes parameters from ".xml" files
  //--------------------------------------------------------------------------------------------------
  TArtBigRIPSParameters *para = TArtBigRIPSParameters::Instance();
  para->LoadParameter(set->PPACFile());
  if (!set->WithDALI())
    para->LoadParameter(set->PPACDefFile());
  para->LoadParameter(set->PlasticFile());
  para->LoadParameter(set->ICFile());
  para->LoadParameter(set->FocalFile());
  if (set->WithDALI())
    para->SetFocusPosOffset(8, 0);

  // Create CalibPID to get and calibrate raw data ( CalibPID ->
  //[CalibPPAC , CalibIC, CalibPlastic , CalibFocalPlane]
  TArtCalibPID *brcalib = new TArtCalibPID();
  TArtCalibPPAC *ppaccalib = brcalib->GetCalibPPAC();
  TArtCalibPlastic *plasticcalib = brcalib->GetCalibPlastic();
  TArtCalibIC *iccalib = brcalib->GetCalibIC();
  TArtCalibFocalPlane *cfpl = brcalib->GetCalibFocalPlane();

  // Create RecoPID to get calibrated data and to reconstruct TOF, AoQ, Z, ... (RecoPID ->
  //[ RecoTOF , RecoRIPS , RecoBeam] )
  TArtRecoPID *recopid = new TArtRecoPID();

  // para->PrintListOfPPACPara();

  // Definition of observables we want to reconstruct
  cout << "Defining bigrips parameters" << endl;

  TArtRIPS *recorips[4];
  recorips[0] = recopid->DefineNewRIPS(3, 5, set->MatrixFile(0), "D3");  // F3 - F5
  recorips[1] = recopid->DefineNewRIPS(5, 7, set->MatrixFile(1), "D5");  // F5 - F7
  recorips[2] = recopid->DefineNewRIPS(8, 9, set->MatrixFile(2), "D7");  // F8 - F9
  recorips[3] = recopid->DefineNewRIPS(9, 11, set->MatrixFile(3), "D8"); // F9 - F11

  // Reconstruction of TOF DefineNewTOF(first plane, second plane, time offset)
  TArtTOF *recotof[6];
  recotof[0] = recopid->DefineNewTOF("F3pl", "F7pl", set->TimeOffset(0), 5); // F3 - F5
  recotof[1] = recopid->DefineNewTOF("F3pl", "F7pl", set->TimeOffset(1), 5); // F5 - F7
  recotof[2] = recopid->DefineNewTOF("F3pl", "F7pl", set->TimeOffset(2), 5); // F3 - F7

  if (set->WithDALI())
  {
    recotof[3] = recopid->DefineNewTOF("F8pl", "F11long", set->TimeOffset(3), 9); // F8 - F9
    recotof[4] = recopid->DefineNewTOF("F8pl", "F11long", set->TimeOffset(4), 9); // F9 - F11
    recotof[5] = recopid->DefineNewTOF("F8pl", "F11long", set->TimeOffset(5), 9); // F8 - F11
  }
  else
  {
    recotof[3] = recopid->DefineNewTOF("F7pl", "F11long", set->TimeOffset(3), 9); // F7 - F9
    recotof[4] = recopid->DefineNewTOF("F7pl", "F11long", set->TimeOffset(4), 9); // F9 - F11
    recotof[5] = recopid->DefineNewTOF("F7pl", "F11long", set->TimeOffset(5), 9); // F7 - F11
  }

  TArtTOF *tof7to8 = recopid->DefineNewTOF("F7pl", "F8pl"); // F7 - F8

  // Reconstruction of IC observables for ID
  TArtBeam *recobeam[6];
  recobeam[0] = recopid->DefineNewBeam(recorips[0], recotof[0], "F7IC");
  recobeam[1] = recopid->DefineNewBeam(recorips[1], recotof[1], "F7IC");
  recobeam[2] = recopid->DefineNewBeam(recorips[0], recorips[1], recotof[2], "F7IC");

  recobeam[3] = recopid->DefineNewBeam(recorips[2], recotof[3], "F11IC");
  recobeam[4] = recopid->DefineNewBeam(recorips[3], recotof[4], "F11IC");
  recobeam[5] = recopid->DefineNewBeam(recorips[2], recorips[3], recotof[5], "F11IC");

  // Create DALIParameters to get ".xml"
  TArtDALIParameters *dpara = TArtDALIParameters::Instance();
  if (set->WithDALI())
    dpara->LoadParameter(set->DALIFile());

  // Create CalibDALI to get and calibrate raw data
  TArtCalibDALI *dalicalib = NULL;
  if (set->WithDALI())
    dalicalib = new TArtCalibDALI();

  // output tree
  TTree *tr = new TTree("tr", "Data Tree");
  // branch for trig bit
  int trigbit = 0;
  tr->Branch("trigbit", &trigbit, "trigbit/I");
  // branch for original event number
  int eventnumber = 0;
  tr->Branch("eventnumber", &eventnumber, "eventnumber/I");
  int toteventnumber = 0;
  tr->Branch("toteventnumber", &toteventnumber, "toteventnumber/I");
  // branch for timestamp
  unsigned long long int timestamp = 0;
  // if(!set->WithDALI())
  tr->Branch("timestamp", &timestamp, "timestamp/l");
  // branches for each focal plane
  FocalPlane *fp[NFPLANES];
  for (unsigned short f = 0; f < NFPLANES; f++)
  {
    fp[f] = new FocalPlane;
    tr->Branch(Form("fp%d", fpID[f]), &fp[f], 320000);
  }
  // branch for the beam, beta, a/q, z
  Beam *beam = new Beam;
  tr->Branch("beam", &beam, 320000);
  // branch for the PPACs
  PPAC *ppacs = new PPAC;
  tr->Branch("ppacs", &ppacs, 320000);
  DALI *dali = new DALI;
  if (set->WithDALI())
  {
    // branch for DALI
    tr->Branch("dali", &dali, 320000);
  }
  tr->SetAutoSave(10000);
  tr->SetAutoFlush(10000);

  // -----------------------------------------------------------------------------
  //  Plastic Scintillator Cut Evaluation
  //
  //  This block evaluates three independent selection cuts for each focal plane
  //  plastic (FP3, FP7, FP8, FP11):
  //
  //      (1) dT vs logQ     cut  → bit 0
  //      (2) logQ vs X      cut  → bit 1
  //      (3) dT vs X        cut  → bit 2
  //
  //  The results are stored in two different types of bit masks:
  //
  //  ---------------------------------------------------------------------------
  //  (A) fpCutMask[0] : 1-bit-per-FP mask for BigRIPS only
  //  ---------------------------------------------------------------------------
  //    fpCutMask[0] tracks whether *all 3 cuts* passed for FP3 and FP7 only.
  //    These are the FPs used for BigRIPS tracking quality.
  //
  //      bit 0 → FP3 all cuts passed
  //      bit 1 → FP7 all cuts passed
  //
  //    Values range 0–3.
  //
  //  ---------------------------------------------------------------------------
  //  (B) fpCutMask[1] : 1-bit-per-FP mask for BigRIPS + ZeroDegree
  //  ---------------------------------------------------------------------------
  //    fpCutMask[1] tracks whether *all 3 cuts* passed for all four FPs
  //    (FP3, FP7, FP8, FP11).
  //
  //      bit 0 → FP3 all cuts passed
  //      bit 1 → FP7 all cuts passed
  //      bit 2 → FP8 all cuts passed
  //      bit 3 → FP11 all cuts passed
  //
  //    Values range 0–15.
  //
  //  ---------------------------------------------------------------------------
  //  (C) plCutMask : 3-bit-per-FP mask (total 12 bits for 4 FPs)
  //  ---------------------------------------------------------------------------
  //    plCutMask encodes *individual cut results* for every FP and every cut type.
  //
  //    Bit layout (fidx = 0..3 → FP3, FP7, FP8, FP11):
  //
  //        fidx=0 (FP3):
  //            bit 0 → dT vs logQ passed
  //            bit 1 → logQ vs X passed
  //            bit 2 → dT vs X passed
  //
  //        fidx=1 (FP7):
  //            bit 3 → dT vs logQ passed
  //            bit 4 → logQ vs X passed
  //            bit 5 → dT vs X passed
  //
  //        fidx=2 (FP8):
  //            bit 6 → dT vs logQ passed
  //            bit 7 → logQ vs X passed
  //            bit 8 → dT vs X passed
  //
  //        fidx=3 (FP11):
  //            bit  9 → dT vs logQ passed
  //            bit 10 → logQ vs X passed
  //            bit 11 → dT vs X passed
  //
  //     Example: plCutMask = 0b000_001_111 means:
  //           - FP3: all 3 cuts passed
  //           - FP7: only first cut passed
  //           - FP8, FP11: none passed
  //
  //  ---------------------------------------------------------------------------
  //  Logic overview:
  //  ---------------------------------------------------------------------------
  //    • For each FP, we check the 3 TCutG gates if they exist.
  //    • For each cut passed, a specific bit inside `plCutMask` is set.
  //    • If all 3 cuts pass:
  //          → corresponding bit in fpCutMask[1] is set.
  //    • If FP is 3 or 7 and all 3 cuts pass:
  //          → corresponding bit in fpCutMask[0] is set.
  //
  //  This structure enables fast post-analysis AND/OR logic on FP cut combinations
  //  and provides diagnostic access to individual cut performance per FP.
  //
  // -----------------------------------------------------------------------------

  int fpCutMask[2] = {0, 0}; // 4-bit FP-pass mask (FP3, FP7, FP8, FP11); fpCutMask[0] for BigRIPS, fpCutMask[1] for BigRIPS && ZeroDegree
  tr->Branch("fpCutMask[2]", &fpCutMask, "fpCutMask[2]/I");

  // branch for individual plastic cut combination information
  int plCutMask = 0; // 12-bit cut-by-cut mask
  tr->Branch("plCutMask", &plCutMask, "plCutMask/I");

  unsigned long long int last_timestamp = 0;
  int ctr = 0;

  // start the event loop
  while (estore->GetNextEvent() && !signal_received)
  {
    // clearing
    trigbit = 0;
    timestamp = 0;
    eventnumber++;
    toteventnumber = event;
    for (int f = 0; f < NFPLANES; f++)
    {
      fp[f]->Clear();
    }
    ppacs->Clear();
    beam->Clear();
    if (set->WithDALI())
      dali->Clear();

    // Making the BigRIPS tree calibration
    brcalib->ClearData();
    brcalib->ReconstructData();

    // trigger bit information
    TArtRawEventObject *rawevent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");
    // for (int i = 0; i < rawevent->GetNumSeg(); i++)
    // {
    //   TArtRawSegmentObject *seg = rawevent->GetSegment(i);
    //   Int_t fpl = seg->GetFP();
    //   Int_t detector = seg->GetDetector();
    //   if (fpl == 0 && detector == 10)
    //   {
    //     for (int j = 0; j < seg->GetNumData(); j++)
    //     {
    //       TArtRawDataObject *d = seg->GetData(j);
    //       trigbit = d->GetVal();
    //     }
    //   }
    // }

    //////////// From Chen's MakeOnlineBigRIPSTree.cc ////////////
    for (int i = 0; i < rawevent->GetNumSeg(); i++)
    {
      TArtRawSegmentObject *seg = rawevent->GetSegment(i);
      Int_t fpl = seg->GetFP();
      Int_t detector = seg->GetDetector();
      if (fpl == 0 && detector == 10)
      {
        for (int j = 0; j < seg->GetNumData(); j++)
        {
          TArtRawDataObject *d = seg->GetData(j);
          int channel = d->GetCh();
          if (channel == 0)
            trigbit = d->GetVal();
        }
      }
    }
    //--------- From Chen's MakeOnlineBigRIPSTree.cc ------------

    // if(!set->WithDALI()){
    // timestamp information
    TClonesArray *info_a = (TClonesArray *)sman->FindDataContainer("EventInfo");
    TArtEventInfo *info = (TArtEventInfo *)info_a->At(0);
    unsigned int bit = info->GetTriggerBit();
    timestamp = info->GetTimeStamp();
    if (timestamp < last_timestamp)
    {
      cout << "timestamp was reset, this TS = " << timestamp << ", last one was " << last_timestamp << " difference " << timestamp - last_timestamp << endl;
    }
    //
    if (vl > 0)
      cout << "TrigBit: " << trigbit << "\t" << bit << "\t" << timestamp << "\t" << timestamp - last_timestamp << endl;
    last_timestamp = timestamp;
    // timestamp = info->GetEventNumber();
    // }

    TArtPPAC *tppac;
    for (unsigned short p = 0; p < NPPACS; p++)
    {
      SinglePPAC *dppac = new SinglePPAC;
      tppac = ppaccalib->GetPPAC(p);
      if (tppac)
      {
        if (set->WithDALI())
          dppac->Set(tppac->GetID(), tppac->GetX(), tppac->GetY(), tppac->GetXZPos(), tppac->GetTSumX(), tppac->GetTSumY());
        else
          dppac->Set(tppac->GetID(), tppac->GetX(), tppac->GetY(), tppac->GetXZPos(), tppac->GetYZPos(), tppac->GetTSumX(), tppac->GetTSumY());
        if (tppac->IsFiredX() || tppac->IsFiredY())
          ppacs->AddPPAC(dppac);
      }
    }

    // focal plane detector information
    TArtFocalPlane *tfpl;
    TArtPlastic *tpla;
    TArtIC *tic;
    TVectorD *vec;
    Track track;
    Plastic plastic;
    MUSIC music;

    // Define Variables for Plastic Cuts
    float dT[NFPLANES];
    float logQ[NFPLANES];

    // loop over focal planes
    for (unsigned short f = 0; f < NFPLANES; f++)
    {
      fp[f]->Clear();
      track.Clear();
      tfpl = cfpl->FindFocalPlane(fpID[f]);

      if (!set->WithDALI())
      {
        TMatrixD xvec(2, 1);
        xvec.Zero();
        TMatrixD yvec(2, 1);
        yvec.Zero();
        TMatrixD xmat(2, 2);
        xmat.Zero();
        TMatrixD ymat(2, 2);
        ymat.Zero();
        int first = firstPPAC(fpID[f]);
        if (first < 0)
          continue;
        double zpos = tfpl->GetZoffset();
        int nfiredx[3] = {0, 0, 0}; // total, upstream, downstream
        int nfiredy[3] = {0, 0, 0};
        for (unsigned short p = 0; p < 4; p++)
        {
          SinglePPAC *dppac = ppacs->GetPPACID(first + p);
          double x = dppac->GetX();
          double y = dppac->GetY();
          double zx = dppac->GetXZ() - zpos;
          double zy = dppac->GetYZ() - zpos;
          if (dppac->FiredX())
          {
            xvec(0, 0) += zx * x;
            xvec(1, 0) += x;
            xmat(0, 1) += zx;
            xmat(1, 0) += zx;
            xmat(0, 0) += zx * zx;
            xmat(1, 1)++;
            nfiredx[0]++;
            if (p < 2)
              nfiredx[1]++;
            else
              nfiredx[2]++;
          }
          if (dppac->FiredY())
          {
            yvec(0, 0) += zy * y;
            yvec(1, 0) += y;
            ymat(0, 1) += zy;
            ymat(1, 0) += zy;
            ymat(0, 0) += zy * zy;
            ymat(1, 1)++;
            nfiredy[0]++;
            if (p < 2)
              nfiredy[1]++;
            else
              nfiredy[2]++;
          }
        }
        if (nfiredx[1] > 0 && nfiredx[2] > 0)
        {
          TMatrixD rxvec = xmat.Invert() * xvec;
          tfpl->SetOptVector(0, rxvec(1, 0));
          tfpl->SetOptVector(1, TMath::ATan(rxvec(0, 0)) * 1000);
        }
        else
        {
          tfpl->SetOptVector(0, -99999);
          tfpl->SetOptVector(1, -99999);
        }
        if (nfiredy[1] > 0 && nfiredy[2] > 0)
        {
          TMatrixD ryvec = ymat.Invert() * yvec;
          tfpl->SetOptVector(2, ryvec(1, 0));
          tfpl->SetOptVector(3, TMath::ATan(ryvec(0, 0)) * 1000);
        }
        else
        {
          tfpl->SetOptVector(2, -99999);
          tfpl->SetOptVector(3, -99999);
        }
        tfpl->SetNumFiredPPACX(nfiredx[0]);
        tfpl->SetNumFiredPPACY(nfiredy[0]);
        tfpl->CopyPos();
      }

      if (vl > 2)
        cout << "FP " << fpID[f];
      if (tfpl)
      {
        if (vl > 2)
          cout << "\tnfiredx  = " << tfpl->GetNumFiredPPACX() << ", nfiredy  = " << tfpl->GetNumFiredPPACY();
        vec = tfpl->GetOptVector();
        if (vl > 2)
          cout << "\tx = " << (*vec)(0) << ", y = " << (*vec)(2) << ", a = " << (*vec)(1) << ",b = " << (*vec)(3);
        track.Set((*vec)(0), (*vec)(2), (*vec)(1), (*vec)(3));
      }
      if (vl > 2)
        cout << endl;
      plastic.Clear();
      tpla = plasticcalib->FindPlastic(Form("F%dpl", fpID[f]));
      if (fpID[f] == 11)
        tpla = plasticcalib->FindPlastic(Form("F%dpl-1", fpID[f]));
      if (tpla)
      {
        // set charge and time
        plastic.SetTime(tpla->GetTimeL(), tpla->GetTimeR());
        plastic.SetCharge(tpla->GetQLRaw(), tpla->GetQRRaw());

        // --- Multihit: Set hit-count and per-hit values from TArtPlastic -> Plastic ---
        // counts
        Int_t nHitL = tpla->GetNHitL();
        Int_t nHitR = tpla->GetNHitR();
        plastic.SetMultihit(nHitL, nHitR);

        // per-hit left
        for (Int_t ih = 0; ih < nHitL; ++ih)
        {
          // GetMulHitTDCL returns Int_t (channel/time)
          plastic.AddMulHitTDCL(static_cast<Int_t>(tpla->GetMulHitTDCL(ih)));
        }

        // per-hit right
        for (Int_t ih = 0; ih < nHitR; ++ih)
        {
          plastic.AddMulHitTDCR(static_cast<Int_t>(tpla->GetMulHitTDCR(ih)));
        }
      }

      // MUSIC info
      music.Clear();
      tic = iccalib->FindIC(Form("F%dIC", fpID[f]));
      if (tic)
      {
        music.SetNHits(tic->GetNumHit());
        music.SetEnergy(tic->GetEnergyAvSum(), tic->GetEnergySqSum());
      }

      // fill the Focal Plane class for every event
      fp[f]->SetTrack(track);
      fp[f]->SetPlastic(plastic);
      fp[f]->SetMUSIC(music);

      // load info into the Variables for Plastic Cut calculations
      if (tpla)
      {
        // protect dT calculation against non-positive times
        float ftimeL = tpla->GetTimeL();
        float ftimeR = tpla->GetTimeR();
        if (ftimeL > 0.0 && ftimeR > 0.0)
          dT[f] = ftimeL - ftimeR;
        else
          dT[f] = NAN; // mark invalid

        // protect log calculation against non-positive charges
        float qL = tpla->GetQLRaw();
        float qR = tpla->GetQRRaw();
        if (0.0 < qL && qL < 10000 && 0.0 < qR && qR < 10000) // < 10000 to protect against unphysical large charges
          logQ[f] = log(qL / qR);
        else
          logQ[f] = NAN; // mark invalid
      }
    }

    // -----------------------------
    // FP-Level Plastic Cut Pass Mask Evaluation
    // -----------------------------
    // 1-bit-per-FP mask

    fpCutMask[0] = 0; // BigRIPS
    fpCutMask[1] = 0; // BigRIPS && ZeroDegree

    fidx = 0; // reset FP index
    for (auto &id : fpIDs)
    {
      bool passBR = true;   // all cuts for BigRIPS
      bool passBRZD = true; // all cuts for BigRIPS+ZeroDegree

      Track *trackPtr = nullptr;
      if (fp[fpNr(id)])
        trackPtr = fp[fpNr(id)]->GetTrack();

      // -----------------------------
      // 1) dT vs logQ cut
      // -----------------------------
      if (PlasticCuts["dT_vs_logQ_cuts"][id])
      {
        TCutG *cut = PlasticCuts["dT_vs_logQ_cuts"][id];
        if (!(cut && !std::isnan(dT[fpNr(id)]) && !std::isnan(logQ[fpNr(id)]) &&
              cut->IsInside(logQ[fpNr(id)], dT[fpNr(id)])))
        {
          passBRZD = false;
          if (id == 3 || id == 7)
            passBR = false;
          // short-circuit: break for FP-level mask
          continue;
        }
      }

      // -----------------------------
      // 2) logQ vs X cut
      // -----------------------------
      if (PlasticCuts["logQ_vs_X_cuts"][id])
      {
        TCutG *cut = PlasticCuts["logQ_vs_X_cuts"][id];
        if (!(cut && !std::isnan(logQ[fpNr(id)]) && trackPtr && trackPtr->GetX() != -99999 &&
              cut->IsInside(trackPtr->GetX(), logQ[fpNr(id)])))
        {
          passBRZD = false;
          if (id == 3 || id == 7)
            passBR = false;
          continue;
        }
      }

      // -----------------------------
      // 3) dT vs X cut
      // -----------------------------
      if (PlasticCuts["dT_vs_X_cuts"][id])
      {
        TCutG *cut = PlasticCuts["dT_vs_X_cuts"][id];
        if (!(cut && !std::isnan(dT[fpNr(id)]) && trackPtr && trackPtr->GetX() != -99999 &&
              cut->IsInside(trackPtr->GetX(), dT[fpNr(id)])))
        {
          passBRZD = false;
          if (id == 3 || id == 7)
            passBR = false;
          continue;
        }
      }

      // Final FP mask
      if (passBRZD)
        fpCutMask[1] |= (1 << fidx);
      if ((id == 3 || id == 7) && passBR)
        fpCutMask[0] |= (1 << fidx);

      fidx++;
    }

    // -----------------------------
    // Individual Plastic Cut Evaluation
    // -----------------------------
    // 3-bit-per-FP mask: each FP has 3 bits for the three cuts
    // Bit 0: dT vs logQ
    // Bit 1: logQ vs X
    // Bit 2: dT vs X
    // Example: FP3 -> bits 0-2, FP7 -> bits 3-5, etc.
    plCutMask = 0; // initialize mask

    int fidx = 0;          // focal plane index
    for (auto &id : fpIDs) // e.g., fpIDs = {3,7,8,11}
    {
      int bit_shift = fidx * 3; // 3 bits per FP
      Track *trackPtr = nullptr;
      if (fp[fpNr(id)])
        trackPtr = fp[fpNr(id)]->GetTrack();

      // -----------------------------
      // 1) dT vs logQ cut
      // -----------------------------
      if (PlasticCuts["dT_vs_logQ_cuts"][id])
      {
        TCutG *cut = PlasticCuts["dT_vs_logQ_cuts"][id];
        if (cut && !std::isnan(dT[fpNr(id)]) && !std::isnan(logQ[fpNr(id)]) &&
            cut->IsInside(logQ[fpNr(id)], dT[fpNr(id)]))
          plCutMask |= (1 << bit_shift);
      }

      // -----------------------------
      // 2) logQ vs X cut
      // -----------------------------
      if (PlasticCuts["logQ_vs_X_cuts"][id])
      {
        TCutG *cut = PlasticCuts["logQ_vs_X_cuts"][id];
        if (cut && !std::isnan(logQ[fpNr(id)]) && trackPtr && trackPtr->GetX() != -99999 &&
            cut->IsInside(trackPtr->GetX(), logQ[fpNr(id)]))
          plCutMask |= (1 << (bit_shift + 1));
      }

      // -----------------------------
      // 3) dT vs X cut
      // -----------------------------
      if (PlasticCuts["dT_vs_X_cuts"][id])
      {
        TCutG *cut = PlasticCuts["dT_vs_X_cuts"][id];
        if (cut && !std::isnan(dT[fpNr(id)]) && trackPtr && trackPtr->GetX() != -99999 &&
            cut->IsInside(trackPtr->GetX(), dT[fpNr(id)]))
          plCutMask |= (1 << (bit_shift + 2));
      }

      fidx++;
    }

    // Reconstructiong the PID
    recopid->ClearData();
    recopid->ReconstructData();

    // beam parameters and PID
    beam->SetTOFBeta(0, recotof[2]->GetTOF(), recotof[2]->GetBeta());
    beam->SetTOFBeta(1, recotof[5]->GetTOF(), recotof[5]->GetBeta());
    beam->SetTOFBeta(2, tof7to8->GetTOF(), tof7to8->GetBeta());

    for (unsigned short b = 0; b < 6; b++)
    {
      double z = recobeam[b]->GetZet();
      double gz = 1;
      double oz = 0;
      if (b < 3 && f7_cor[0] != NULL && f7_cor[1] != NULL)
      {
        gz = f7_cor[1]->GetBinContent(event / 10000 + 1);
        oz = f7_cor[0]->GetBinContent(event / 10000 + 1);
      }
      if (b > 2 && f11_cor[0] != NULL && f11_cor[1] != NULL)
      {
        gz = f11_cor[1]->GetBinContent(event / 10000 + 1);
        oz = f11_cor[0]->GetBinContent(event / 10000 + 1);
      }
      double a = recobeam[b]->GetAoQ();
      double ga = 1;
      double oa = 0;
      if (b < 3 && br_cor[0] != NULL && br_cor[1] != NULL)
      {
        ga = br_cor[1]->GetBinContent(event / 10000 + 1);
        oa = br_cor[0]->GetBinContent(event / 10000 + 1);
      }
      if (b > 2 && zd_cor[0] != NULL && zd_cor[1] != NULL)
      {
        ga = zd_cor[1]->GetBinContent(event / 10000 + 1);
        oa = zd_cor[0]->GetBinContent(event / 10000 + 1);
      }
      z = z * gz + oz;
      a = a * ga + oa;

      // cout << z << "\t" << event << "\t" << o << "\t" << g << "\t" << z*g+o << endl;

      beam->SetAQZ(b, a, z);
      beam->SetRIPSBeta(b, recobeam[b]->GetBeta());
    }

    // AoQ Higher order Abberation correction
    if (set->WithDALI())
    {
      for (unsigned short b = 0; b < 3; b++)
      {
        double corr = 0;

        // ========= BR AoQ correction =========
        corr = set->GetBRAoQCorrection_F3X() * fp[fpNr(3)]->GetTrack()->GetX() +
               set->GetBRAoQCorrection_F3A() * fp[fpNr(3)]->GetTrack()->GetA() +
               set->GetBRAoQCorrection_F3Q() * sqrt(fp[fpNr(3)]->GetPlastic()->GetChargeL() * fp[fpNr(3)]->GetPlastic()->GetChargeR()) +
               set->GetBRAoQCorrection_F5X() * fp[fpNr(5)]->GetTrack()->GetX() +
               set->GetBRAoQCorrection_F5A() * fp[fpNr(5)]->GetTrack()->GetA() +
               set->GetBRAoQCorrection_F7X() * fp[fpNr(7)]->GetTrack()->GetX() +
               set->GetBRAoQCorrection_F7A() * fp[fpNr(7)]->GetTrack()->GetA() +
               set->GetBRAoQCorrection_F7Q() * sqrt(fp[fpNr(7)]->GetPlastic()->GetChargeL() * fp[fpNr(7)]->GetPlastic()->GetChargeR());

        beam->CorrectAQ(b, corr);
        beam->ScaleAQ(b, set->GetBRAoQCorrection_gain(), set->GetBRAoQCorrection_offs());

        // ========= ZD AoQ correction =========
        corr = set->GetZDAoQCorrection_F8X() * fp[fpNr(8)]->GetTrack()->GetX() +
               set->GetZDAoQCorrection_F8A() * fp[fpNr(8)]->GetTrack()->GetA() +
               set->GetZDAoQCorrection_F8Q() * sqrt(fp[fpNr(8)]->GetPlastic()->GetChargeL() * fp[fpNr(8)]->GetPlastic()->GetChargeR()) +
               set->GetZDAoQCorrection_F9X() * fp[fpNr(9)]->GetTrack()->GetX() +
               set->GetZDAoQCorrection_F9X2() * pow(fp[fpNr(9)]->GetTrack()->GetX(), 2) +
               set->GetZDAoQCorrection_F9A() * fp[fpNr(9)]->GetTrack()->GetA() +
               set->GetZDAoQCorrection_F9Y() * fp[fpNr(9)]->GetTrack()->GetY() +
               set->GetZDAoQCorrection_F9Y2() * pow(fp[fpNr(9)]->GetTrack()->GetY(), 2) +
               set->GetZDAoQCorrection_F11X() * fp[fpNr(11)]->GetTrack()->GetX() +
               set->GetZDAoQCorrection_F11A() * fp[fpNr(11)]->GetTrack()->GetA() +
               set->GetZDAoQCorrection_F11Q() * sqrt(fp[fpNr(11)]->GetPlastic()->GetChargeL() * fp[fpNr(11)]->GetPlastic()->GetChargeR()) +
               set->GetZDAoQCorrection_F11B() * fp[fpNr(11)]->GetTrack()->GetB() +
               set->GetZDAoQCorrection_F11B2() * pow(fp[fpNr(11)]->GetTrack()->GetB(), 2) +
               set->GetZDAoQCorrection_F11Y() * fp[fpNr(11)]->GetTrack()->GetY() +
               set->GetZDAoQCorrection_F11Y2() * pow(fp[fpNr(11)]->GetTrack()->GetY(), 2);

        beam->CorrectAQ(b + 3, corr);
        beam->ScaleAQ(b + 3, set->GetZDAoQCorrection_gain(), set->GetZDAoQCorrection_offs());
      }
    }

    // Zet for RipsBeta and AoQ correction
    if (set->WithDALI())
    {
      for (unsigned short b = 0; b < 3; b++)
      {
        double corr = 0;

        // ========= BR Zet correction =========
        corr = set->GetBRZCorrection_ripsbeta1() * beam->GetRIPSBeta(1) +
               set->GetBRZCorrection_ripsbeta1sq() * pow(beam->GetRIPSBeta(1), 2) +
               set->GetBRZCorrection_aoq2() * beam->GetAQ(2) +
               set->GetBRZCorrection_aoq2sq() * pow(beam->GetAQ(2), 2) +
               set->GetBRZCorrection_constant();

        beam->CorrectZ(b, corr);

        // ========= ZD Zet correction =========
        corr = set->GetZDZCorrection_ripsbeta5() * beam->GetRIPSBeta(5) +
               set->GetZDZCorrection_ripsbeta5sq() * pow(beam->GetRIPSBeta(5), 2) +
               set->GetZDZCorrection_aoq5() * beam->GetAQ(5) +
               set->GetZDZCorrection_aoq5sq() * pow(beam->GetAQ(5), 2) +
               set->GetZDZCorrection_constant();

        beam->CorrectZ(b + 3, corr);
      }
    }

    for (unsigned short b = 0; b < 4; b++)
      beam->SetDelta(b, recorips[b]->GetDelta());

    if (set->WithDALI() && dalicalib != NULL)
    {
      // dali
      dalicalib->ClearData();
      // removed plastic time for online analysis
      // dalicalib->SetPlTime(beam->GetTOF(2));
      // dalicalib->SetVertex(z_vertex-3.9);
      dalicalib->SetVertex(0.0);
      // Add above to remove F8plastic tof.
      dalicalib->ReconstructData();

      // cout << "dalicalib->GetNumNaI() " << dalicalib->GetNumNaI() << endl;
      for (unsigned short g = 0; g < dalicalib->GetNumNaI() - 2; g++)
      { // last two are junk?
        TArtDALINaI *hit = (TArtDALINaI *)dalicalib->GetNaIArray()->At(g);
        DALIHit *dhit = new DALIHit();
        dhit->SetID(hit->GetID());
        dhit->SetADC(hit->GetRawADC());
        dhit->SetTDC(hit->GetRawTDC());
        dhit->SetEnergy(hit->GetEnergy());
        dhit->SetPos(hit->GetXPos(), hit->GetYPos(), hit->GetZPos());
        dhit->SetDCEnergy(hit->GetDoppCorEnergy());
        dhit->SetTime(hit->GetTime());
        dhit->SetTOffset(hit->GetTimeOffseted());
        //      if(dhit->GetEnergy()>0)
        dali->AddHit(dhit);
      }
    }

    // fill the tree
    tr->Fill();

    // output
    if (ctr % 10000 == 0)
    {
      double time_end = get_time();
      cout << setw(5) << ctr << " events done " << setiosflags(ios::fixed) << setprecision(1) << (Float_t)ctr / (time_end - time_start) << " events/s \r" << flush;
      tr->AutoSave();
      tr->FlushBaskets();
      outfile->Flush();
    }
    ctr++;
    event++;

    if (nmax > 0 && ctr > nmax - 1)
      break;
  }

  double time_end = get_time();
  cout << "Program Run time: " << time_end - time_start << " s." << endl;
  cout << "Total of " << ctr << " events processed, " << ctr / (time_end - time_start) << " events/s." << endl;
  cout << BLUE << tr->GetEntries() << DEFCOLOR << " entries written to tree (" << BLUE << tr->GetZipBytes() / (1024 * 1024) << DEFCOLOR << " MB)" << endl;
  tr->Write("", TObject::kOverwrite);
  outfile->Close();
  timer.Stop();
  cout << "CPU time: " << timer.CpuTime() << "\tReal time: " << timer.RealTime() << endl;

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
