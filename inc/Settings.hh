#ifndef __SETTINGS_HH
#define __SETTINGS_HH
#include <iostream>
#include <iomanip>
#include <string>

#include "TEnv.h"

using namespace std;

struct AoQCorrections
{
  double X, A, Q, B, Y;      // linear terms
  double X2, A2, Q2, B2, Y2; // quadratic terms
};

/*!
  A container for the analysis settings
*/
class Settings
{
public:
  //! default constructor
  Settings() {};
  //! constructor
  Settings(char *settings);
  //! dummy destructor
  ~Settings() {
  };
  //! Read the settings
  void ReadSettings();
  //! Print the settings
  void PrintSettings();

  //! Get the vorbose level
  int VerboseLevel() { return fverbose; }
  //! Set the vorbose level
  void SetVerboseLevel(int vl) { fverbose = vl; }

  //! Get if DALI is to be used
  bool WithDALI() { return fwithDALI; }

  //! Get the BigRIPS PPAC xml file
  char *PPACFile() { return (char *)fPPACfile.c_str(); }
  //! Get the BigRIPS PPAC Default xml file
  char *PPACDefFile() { return (char *)fPPACdefaultfile.c_str(); }
  //! Get the BigRIPS plastic xml file
  char *PlasticFile() { return (char *)fplasticfile.c_str(); }
  //! Get the BigRIPS IC xml file
  char *ICFile() { return (char *)fICfile.c_str(); }
  //! Get the BigRIPS Focalplane xml file
  char *FocalFile() { return (char *)ffocalfile.c_str(); }
  //! Get the BigRIPS/ZeroDegree matrix file
  char *MatrixFile(int i) { return (char *)fmatrixfile[i].c_str(); }

  //! Get the time of flight offsets for the A/Q
  double TimeOffset(int b) { return ftoffset[b]; }
  //! XML file with the DALI calibrations
  char *DALIFile() { return (char *)fDALIfile.c_str(); }

  //! Get the file with the bad DALI channels
  char *BadChFile() { return (char *)fDALIbadfile.c_str(); }
  //! Get the file with the DALI recalibration parameters
  char *ReCalFile() { return (char *)fDALIrecalfile.c_str(); }
  //! Do the recalibration
  bool DoReCalibration() { return fdorecal; }

  //! The file containing the time dependent corrections
  const char *TimeCorFile() { return fTimeCorFile.c_str(); }
  //! The file containing the events numbers at which each run starts
  const char *EvtNrFile() { return fEvtNrFile.c_str(); }

  //! Get the overflow energy
  double Overflow() { return foverflow; }
  //! Get the underflow energy
  double Underflow() { return funderflow; }
  //! Get the txt file witht the positions of the DALI crystals from the simulation
  char *DALIPosFile() { return (char *)fDALIposfile.c_str(); }
  //! Get the beta for the Doppler correction
  double Beta() { return fbeta; }
  //! Get the gate on the DALI - beam timing
  double TimingGate(int i) { return ftimegate[i]; }

  //! Get the beta in front of MINOS for the Doppler correction
  double BetaBefore() { return fbetaM[0]; }
  //! Get the beta after MINOS for the Doppler correction
  double BetaAfter() { return fbetaM[1]; }
  //! Get the lenght of the MINOS target
  double MINOSlength() { return fminoslength; }

  //! Get the type of addback to be used, 1 distance, 2 angles
  int AddbackType() { return faddbacktype; }
  //! Get the maximum distance between hits to be considered for addback
  double AddbackDistance() { return faddbackdistance; }
  //! Get the maximum angle between hits and target to be considered for addback
  double AddbackAngle() { return faddbackangle; }
  //! Get the software threshold for addback, global
  double AddbackThresh() { return faddbackthreshold; }
  //! Get the time difference between hits to be considered for addback
  double AddbackTimeDiff(int i) { return faddbacktdiff[i]; }

  //! Get the alignment shift (X0) for the PPAC3 at F8 after the target
  double PPAC3PositionX0() { return fppac3align[0]; }
  //! Get the alignment shift (Y0) for the PPAC3 at F8 after the target
  double PPAC3PositionY0() { return fppac3align[1]; }
  //! Get the alignment shift (X1) for the PPAC3 at F8 after the target
  double PPAC3PositionX1() { return fppac3align[2]; }
  //! Get the alignment shift (Y1) for the PPAC3 at F8 after the target
  double PPAC3PositionY1() { return fppac3align[3]; }
  //! Get the target position with respect to nominal focus
  double TargetPosition() { return ftargetposition; }
  //! Get the gate on the F5X position
  double F5XGate(int i) { return ff5xgate[i]; }
  //! Get the gate on the change in delta for charge changes
  double DeltaGate(int i) { return fdeltagate[i]; }

  // ================ BigRIPS (sp=0) ================
  double GetAoQCorrection(int sp, int fp, const char *var); // sp: 0=BigRIPS,1=ZeroDeg; fp: focal plane index; var: "X","A","Q","B","Y","X2","A2","Q2","B2","Y2"

  double GetBRAoQCorrection_F3X() { return faoq_corr[0][0].X; }
  double GetBRAoQCorrection_F3A() { return faoq_corr[0][0].A; }
  double GetBRAoQCorrection_F3Q() { return faoq_corr[0][0].Q; }
  double GetBRAoQCorrection_F3B() { return faoq_corr[0][0].B; }
  double GetBRAoQCorrection_F3Y() { return faoq_corr[0][0].Y; }
  double GetBRAoQCorrection_F3X2() { return faoq_corr[0][0].X2; }
  double GetBRAoQCorrection_F3A2() { return faoq_corr[0][0].A2; }
  double GetBRAoQCorrection_F3Q2() { return faoq_corr[0][0].Q2; }
  double GetBRAoQCorrection_F3B2() { return faoq_corr[0][0].B2; }
  double GetBRAoQCorrection_F3Y2() { return faoq_corr[0][0].Y2; }

  double GetBRAoQCorrection_F5X() { return faoq_corr[0][1].X; }
  double GetBRAoQCorrection_F5A() { return faoq_corr[0][1].A; }
  double GetBRAoQCorrection_F5Q() { return faoq_corr[0][1].Q; }
  double GetBRAoQCorrection_F5B() { return faoq_corr[0][1].B; }
  double GetBRAoQCorrection_F5Y() { return faoq_corr[0][1].Y; }
  double GetBRAoQCorrection_F5X2() { return faoq_corr[0][1].X2; }
  double GetBRAoQCorrection_F5A2() { return faoq_corr[0][1].A2; }
  double GetBRAoQCorrection_F5Q2() { return faoq_corr[0][1].Q2; }
  double GetBRAoQCorrection_F5B2() { return faoq_corr[0][1].B2; }
  double GetBRAoQCorrection_F5Y2() { return faoq_corr[0][1].Y2; }

  double GetBRAoQCorrection_F7X() { return faoq_corr[0][2].X; }
  double GetBRAoQCorrection_F7A() { return faoq_corr[0][2].A; }
  double GetBRAoQCorrection_F7Q() { return faoq_corr[0][2].Q; }
  double GetBRAoQCorrection_F7B() { return faoq_corr[0][2].B; }
  double GetBRAoQCorrection_F7Y() { return faoq_corr[0][2].Y; }
  double GetBRAoQCorrection_F7X2() { return faoq_corr[0][2].X2; }
  double GetBRAoQCorrection_F7A2() { return faoq_corr[0][2].A2; }
  double GetBRAoQCorrection_F7Q2() { return faoq_corr[0][2].Q2; }
  double GetBRAoQCorrection_F7B2() { return faoq_corr[0][2].B2; }
  double GetBRAoQCorrection_F7Y2() { return faoq_corr[0][2].Y2; }

  // ================ ZeroDeg (sp=1) ================
  double GetZDAoQCorrection_F8X() { return faoq_corr[1][0].X; }
  double GetZDAoQCorrection_F8A() { return faoq_corr[1][0].A; }
  double GetZDAoQCorrection_F8Q() { return faoq_corr[1][0].Q; }
  double GetZDAoQCorrection_F8B() { return faoq_corr[1][0].B; }
  double GetZDAoQCorrection_F8Y() { return faoq_corr[1][0].Y; }
  double GetZDAoQCorrection_F8X2() { return faoq_corr[1][0].X2; }
  double GetZDAoQCorrection_F8A2() { return faoq_corr[1][0].A2; }
  double GetZDAoQCorrection_F8Q2() { return faoq_corr[1][0].Q2; }
  double GetZDAoQCorrection_F8B2() { return faoq_corr[1][0].B2; }
  double GetZDAoQCorrection_F8Y2() { return faoq_corr[1][0].Y2; }

  double GetZDAoQCorrection_F9X() { return faoq_corr[1][1].X; }
  double GetZDAoQCorrection_F9A() { return faoq_corr[1][1].A; }
  double GetZDAoQCorrection_F9Q() { return faoq_corr[1][1].Q; }
  double GetZDAoQCorrection_F9B() { return faoq_corr[1][1].B; }
  double GetZDAoQCorrection_F9Y() { return faoq_corr[1][1].Y; }
  double GetZDAoQCorrection_F9X2() { return faoq_corr[1][1].X2; }
  double GetZDAoQCorrection_F9A2() { return faoq_corr[1][1].A2; }
  double GetZDAoQCorrection_F9Q2() { return faoq_corr[1][1].Q2; }
  double GetZDAoQCorrection_F9B2() { return faoq_corr[1][1].B2; }
  double GetZDAoQCorrection_F9Y2() { return faoq_corr[1][1].Y2; }

  double GetZDAoQCorrection_F11X() { return faoq_corr[1][2].X; }
  double GetZDAoQCorrection_F11A() { return faoq_corr[1][2].A; }
  double GetZDAoQCorrection_F11Q() { return faoq_corr[1][2].Q; }
  double GetZDAoQCorrection_F11B() { return faoq_corr[1][2].B; }
  double GetZDAoQCorrection_F11Y() { return faoq_corr[1][2].Y; }
  double GetZDAoQCorrection_F11X2() { return faoq_corr[1][2].X2; }
  double GetZDAoQCorrection_F11A2() { return faoq_corr[1][2].A2; }
  double GetZDAoQCorrection_F11Q2() { return faoq_corr[1][2].Q2; }
  double GetZDAoQCorrection_F11B2() { return faoq_corr[1][2].B2; }
  double GetZDAoQCorrection_F11Y2() { return faoq_corr[1][2].Y2; }

  double GetBRAoQCorrection_gain() { return faoq_lin[0][0]; }
  double GetBRAoQCorrection_offs() { return faoq_lin[0][1]; }
  double GetZDAoQCorrection_gain() { return faoq_lin[1][0]; }
  double GetZDAoQCorrection_offs() { return faoq_lin[1][1]; }

  // ================ Z corrections ================
  double GetBRZCorrection_ripsbeta1() { return fzet_corr[0][0]; }
  double GetBRZCorrection_ripsbeta1sq() { return fzet_corr[0][1]; }
  double GetBRZCorrection_aoq2() { return fzet_corr[0][2]; }
  double GetBRZCorrection_aoq2sq() { return fzet_corr[0][3]; }
  double GetBRZCorrection_constant() { return fzet_corr[0][4]; }

  double GetZDZCorrection_ripsbeta5() { return fzet_corr[1][0]; }
  double GetZDZCorrection_ripsbeta5sq() { return fzet_corr[1][1]; }
  double GetZDZCorrection_aoq5() { return fzet_corr[1][2]; }
  double GetZDZCorrection_aoq5sq() { return fzet_corr[1][3]; }
  double GetZDZCorrection_constant() { return fzet_corr[1][4]; }

private:
  //! filename of the settings file
  string finputfile;
  //! verbose level
  int fverbose;
  //! use DALI
  bool fwithDALI;
  //! BigRIPS PPAC xml file
  string fPPACfile;
  //! BigRIPS PPAC default xml file
  string fPPACdefaultfile;
  //! BigRIPS Plastic xml file
  string fplasticfile;
  //! BigRIPS IC xml file
  string fICfile;
  //! BigRIPS Focalplane xml file
  string ffocalfile;
  //! BigRIPS/ZeroDegree matrix files
  string fmatrixfile[4];
  //! time offsets for A/Q calibration
  double ftoffset[6];
  //! DALI calibration file
  string fDALIfile;

  //! Time dependent corrections for IC
  string fTimeCorFile;
  //! Event numbers
  string fEvtNrFile;

  //! DALI bad channels file
  string fDALIbadfile;
  //! DALI recalibration file
  string fDALIrecalfile;
  //! do recalibration
  bool fdorecal;
  //! Overflow value for gamma energies
  double foverflow;
  //! Underflow value for gamma energies
  double funderflow;

  //! averaged positions from the simulation
  string fDALIposfile;
  //! averge beta for Doppler correction
  double fbeta;
  //! timing gate DALI - beam
  double ftimegate[2];
  //! beta for Doppler correction with MINOS before and after target
  double fbetaM[2];
  //! length of minos
  double fminoslength;

  //! type of addback
  int faddbacktype;
  //! max distance between two hits for addback
  double faddbackdistance;
  //! max angle between two hits for addback
  double faddbackangle;
  //! energy threshold for addback
  double faddbackthreshold;
  //! time difference between two hits for addback
  double faddbacktdiff[2];

  //! alignment of PPAc at F8
  double fppac3align[4];
  //! target position with respect to nominal focus
  double ftargetposition;
  //! gate on the F5X position
  double ff5xgate[2];
  //! gate on the delta change
  double fdeltagate[4];

  // Using struct for better readability and extendability
  AoQCorrections faoq_corr[2][3]; // 2 spectrometers, 3 focal planes

  // double faoq_corr[2][3][3];      // BR/ZD; focal plane; X, angle A, q

  double faoq_lin[2][2]; // BR/ZD; gain; offs

  //! Z correction based on ripsbeta and aoq
  double fzet_corr[2][5]; // BR/ZD; RipsBeta linear & quadratic; aoq linear and quadratic; and constant
};
#endif
