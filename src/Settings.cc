#include "Settings.hh"
using namespace std;

/*!
  Constructor, setup the settings
  \param settings the settings file
*/
Settings::Settings(char *settings)
{
  finputfile = settings;
  ReadSettings();
}
/*!
  Read in the settings from the fie
*/
void Settings::ReadSettings()
{

  TEnv *set = new TEnv(finputfile.c_str());
  fverbose = set->GetValue("VerboseLevel", 0);

  fwithDALI = set->GetValue("WithDALI", true);

  for (int i = 0; i < 6; i++)
    ftoffset[i] = set->GetValue(Form("TOF.Offset.%d", i), 300.0);
  fDALIfile = set->GetValue("DALI.File", "/home/wimmer/ribf94/db/DALI.xml");

  fPPACfile = set->GetValue("BigRIPS.PPAC.File", "/home/wimmer/ribf94/db/BigRIPSPPAC.xml");
  fPPACdefaultfile = set->GetValue("BigRIPS.PPAC.Def.File", "/home/wimmer/ribf94/db/BigRIPSPPAC.xml");
  fplasticfile = set->GetValue("BigRIPS.Plastic.File", "/home/wimmer/ribf94/db/BigRIPSPlastic.xml");
  fICfile = set->GetValue("BigRIPS.IC.File", "/home/wimmer/ribf94/db/BigRIPSIC.xml");
  ffocalfile = set->GetValue("BigRIPS.Focal.File", "/home/wimmer/ribf94/db/FocalPlane.xml");
  fmatrixfile[0] = set->GetValue("Matrix.35.File", "/home/wimmer/ribf94/matrix/mat1.mat");
  fmatrixfile[1] = set->GetValue("Matrix.57.File", "/home/wimmer/ribf94/matrix/mat2.mat");
  fmatrixfile[2] = set->GetValue("Matrix.89.File", "/home/wimmer/ribf94/matrix/F8F9_LargeAccAchr.mat");
  fmatrixfile[3] = set->GetValue("Matrix.911.File", "/home/wimmer/ribf94/matrix/F9F11_LargeAccAchr.mat");

  fTimeCorFile = set->GetValue("Time.Corrections.File", "settings.dat");
  fEvtNrFile = set->GetValue("Event.Number.File", "settings.dat");

  fdorecal = false;
  if (set->GetValue("Do.ReCalibration", 0) > 0)
  {
    fdorecal = true;
    fDALIrecalfile = set->GetValue("ReCalibration.File", (char *)"settings/recal.dat");
  }

  fbeta = set->GetValue("AverageBeta", 0.5);
  fbetaM[0] = set->GetValue("BeforeBeta", 0.5);
  fbetaM[1] = set->GetValue("AfterBeta", 0.5);
  fminoslength = set->GetValue("LengthMINOS", 5.0);
  foverflow = set->GetValue("Overflow.Threshold", 8000);
  funderflow = set->GetValue("Underflow.Threshold", 0);
  faddbacktype = set->GetValue("Addback.Type", 1);
  faddbackdistance = set->GetValue("Addback.Distance", 20.);
  faddbackangle = set->GetValue("Addback.Angle", 20.);
  faddbackthreshold = set->GetValue("Addback.Threshold", 200.);
  faddbacktdiff[0] = set->GetValue("Addback.TimeDiff.Low", -50.);
  faddbacktdiff[1] = set->GetValue("Addback.TimeDiff.High", 20.);

  ftimegate[0] = set->GetValue("Timing.Gate.Low", -1000.);
  ftimegate[1] = set->GetValue("Timing.Gate.High", 1000.);

  fDALIposfile = set->GetValue("InteractionPoints", (char *)"settings/iponts.dat");
  fDALIbadfile = set->GetValue("Bad.Channels", (char *)"settings/baddali.dat");

  fppac3align[0] = set->GetValue("PPAC3.Align.X0", 0.0);
  fppac3align[1] = set->GetValue("PPAC3.Align.Y0", 0.0);
  fppac3align[2] = set->GetValue("PPAC3.Align.X1", 0.0);
  fppac3align[3] = set->GetValue("PPAC3.Align.Y1", 0.0);
  ftargetposition = set->GetValue("Target.Position", 129.5);

  ff5xgate[0] = set->GetValue("F5X.Gate.Low", -200.);
  ff5xgate[1] = set->GetValue("F5X.Gate.High", 200.);
  fdeltagate[2] = set->GetValue("Delta.Gate.Low", -999.);
  fdeltagate[3] = set->GetValue("Delta.Gate.High", 999.);
  fdeltagate[0] = set->GetValue("Delta.Gate.BR.Low", -999.);
  fdeltagate[1] = set->GetValue("Delta.Gate.BR.High", 999.);

  // AoQ corrections
  // BigRIPS F3 (sp=0, fp=0) =============
  faoq_corr[0][0].X = set->GetValue("BigRIPS.AoQCorr_F3X", 0.0);
  faoq_corr[0][0].A = set->GetValue("BigRIPS.AoQCorr_F3A", 0.0);
  faoq_corr[0][0].Q = set->GetValue("BigRIPS.AoQCorr_F3Q", 0.0);
  faoq_corr[0][0].B = set->GetValue("BigRIPS.AoQCorr_F3B", 0.0);
  faoq_corr[0][0].Y = set->GetValue("BigRIPS.AoQCorr_F3Y", 0.0);

  faoq_corr[0][0].X2 = set->GetValue("BigRIPS.AoQCorr_F3X2", 0.0);
  faoq_corr[0][0].A2 = set->GetValue("BigRIPS.AoQCorr_F3A2", 0.0);
  faoq_corr[0][0].Q2 = set->GetValue("BigRIPS.AoQCorr_F3Q2", 0.0);
  faoq_corr[0][0].B2 = set->GetValue("BigRIPS.AoQCorr_F3B2", 0.0);
  faoq_corr[0][0].Y2 = set->GetValue("BigRIPS.AoQCorr_F3Y2", 0.0);

  // BigRIPS F5 (sp=0, fp=1)
  faoq_corr[0][1].X = set->GetValue("BigRIPS.AoQCorr_F5X", 0.0);
  faoq_corr[0][1].A = set->GetValue("BigRIPS.AoQCorr_F5A", 0.0);
  faoq_corr[0][1].Q = set->GetValue("BigRIPS.AoQCorr_F5Q", 0.0);
  faoq_corr[0][1].B = set->GetValue("BigRIPS.AoQCorr_F5B", 0.0);
  faoq_corr[0][1].Y = set->GetValue("BigRIPS.AoQCorr_F5Y", 0.0);

  faoq_corr[0][1].X2 = set->GetValue("BigRIPS.AoQCorr_F5X2", 0.0);
  faoq_corr[0][1].A2 = set->GetValue("BigRIPS.AoQCorr_F5A2", 0.0);
  faoq_corr[0][1].Q2 = set->GetValue("BigRIPS.AoQCorr_F5Q2", 0.0);
  faoq_corr[0][1].B2 = set->GetValue("BigRIPS.AoQCorr_F5B2", 0.0);
  faoq_corr[0][1].Y2 = set->GetValue("BigRIPS.AoQCorr_F5Y2", 0.0);

  // BigRIPS F7 (sp=0, fp=2)
  faoq_corr[0][2].X = set->GetValue("BigRIPS.AoQCorr_F7X", 0.0);
  faoq_corr[0][2].A = set->GetValue("BigRIPS.AoQCorr_F7A", 0.0);
  faoq_corr[0][2].Q = set->GetValue("BigRIPS.AoQCorr_F7Q", 0.0);
  faoq_corr[0][2].B = set->GetValue("BigRIPS.AoQCorr_F7B", 0.0);
  faoq_corr[0][2].Y = set->GetValue("BigRIPS.AoQCorr_F7Y", 0.0);

  faoq_corr[0][2].X2 = set->GetValue("BigRIPS.AoQCorr_F7X2", 0.0);
  faoq_corr[0][2].A2 = set->GetValue("BigRIPS.AoQCorr_F7A2", 0.0);
  faoq_corr[0][2].Q2 = set->GetValue("BigRIPS.AoQCorr_F7Q2", 0.0);
  faoq_corr[0][2].B2 = set->GetValue("BigRIPS.AoQCorr_F7B2", 0.0);
  faoq_corr[0][2].Y2 = set->GetValue("BigRIPS.AoQCorr_F7Y2", 0.0);

  // ZeroDegree F8 (sp=1, fp=0) ==============
  faoq_corr[1][0].X = set->GetValue("ZeroDeg.AoQCorr_F8X", 0.0);
  faoq_corr[1][0].A = set->GetValue("ZeroDeg.AoQCorr_F8A", 0.0);
  faoq_corr[1][0].Q = set->GetValue("ZeroDeg.AoQCorr_F8Q", 0.0);
  faoq_corr[1][0].B = set->GetValue("ZeroDeg.AoQCorr_F8B", 0.0);
  faoq_corr[1][0].Y = set->GetValue("ZeroDeg.AoQCorr_F8Y", 0.0);

  faoq_corr[1][0].X2 = set->GetValue("ZeroDeg.AoQCorr_F8X2", 0.0);
  faoq_corr[1][0].A2 = set->GetValue("ZeroDeg.AoQCorr_F8A2", 0.0);
  faoq_corr[1][0].Q2 = set->GetValue("ZeroDeg.AoQCorr_F8Q2", 0.0);
  faoq_corr[1][0].B2 = set->GetValue("ZeroDeg.AoQCorr_F8B2", 0.0);
  faoq_corr[1][0].Y2 = set->GetValue("ZeroDeg.AoQCorr_F8Y2", 0.0);

  // ZeroDegree F9 (sp=1, fp=1)
  faoq_corr[1][1].X = set->GetValue("ZeroDeg.AoQCorr_F9X", 0.0);
  faoq_corr[1][1].A = set->GetValue("ZeroDeg.AoQCorr_F9A", 0.0);
  faoq_corr[1][1].Q = set->GetValue("ZeroDeg.AoQCorr_F9Q", 0.0);
  faoq_corr[1][1].B = set->GetValue("ZeroDeg.AoQCorr_F9B", 0.0);
  faoq_corr[1][1].Y = set->GetValue("ZeroDeg.AoQCorr_F9Y", 0.0);

  faoq_corr[1][1].X2 = set->GetValue("ZeroDeg.AoQCorr_F9X2", 0.0);
  faoq_corr[1][1].A2 = set->GetValue("ZeroDeg.AoQCorr_F9A2", 0.0);
  faoq_corr[1][1].Q2 = set->GetValue("ZeroDeg.AoQCorr_F9Q2", 0.0);
  faoq_corr[1][1].B2 = set->GetValue("ZeroDeg.AoQCorr_F9B2", 0.0);
  faoq_corr[1][1].Y2 = set->GetValue("ZeroDeg.AoQCorr_F9Y2", 0.0);

  // ZeroDegree F11 (sp=1, fp=2)
  faoq_corr[1][2].X = set->GetValue("ZeroDeg.AoQCorr_F11X", 0.0);
  faoq_corr[1][2].A = set->GetValue("ZeroDeg.AoQCorr_F11A", 0.0);
  faoq_corr[1][2].Q = set->GetValue("ZeroDeg.AoQCorr_F11Q", 0.0);
  faoq_corr[1][2].B = set->GetValue("ZeroDeg.AoQCorr_F11B", 0.0);
  faoq_corr[1][2].Y = set->GetValue("ZeroDeg.AoQCorr_F11Y", 0.0);

  faoq_corr[1][2].X2 = set->GetValue("ZeroDeg.AoQCorr_F11X2", 0.0);
  faoq_corr[1][2].A2 = set->GetValue("ZeroDeg.AoQCorr_F11A2", 0.0);
  faoq_corr[1][2].Q2 = set->GetValue("ZeroDeg.AoQCorr_F11Q2", 0.0);
  faoq_corr[1][2].B2 = set->GetValue("ZeroDeg.AoQCorr_F11B2", 0.0);
  faoq_corr[1][2].Y2 = set->GetValue("ZeroDeg.AoQCorr_F11Y2", 0.0);

  faoq_lin[0][0] = set->GetValue("BigRIPS.AoQCorr_Gain", 1.0);
  faoq_lin[0][1] = set->GetValue("BigRIPS.AoQCorr_Offs", 0.0);

  faoq_lin[1][0] = set->GetValue("ZeroDeg.AoQCorr_Gain", 1.0);
  faoq_lin[1][1] = set->GetValue("ZeroDeg.AoQCorr_Offs", 0.0);

  // Z corrections based on ripsbeta and AoQ
  fzet_corr[0][0] = set->GetValue("BigRIPS.ZCorr_RipsBeta1", 0.0);
  fzet_corr[0][1] = set->GetValue("BigRIPS.ZCorr_RipsBeta1sq", 0.0);
  fzet_corr[0][2] = set->GetValue("BigRIPS.ZCorr_AoQ2", 0.0);
  fzet_corr[0][3] = set->GetValue("BigRIPS.ZCorr_AoQ2sq", 0.0);
  fzet_corr[0][4] = set->GetValue("BigRIPS.ZCorr_Constant", 0.0);

  fzet_corr[1][0] = set->GetValue("ZeroDeg.ZCorr_RipsBeta5", 0.0);
  fzet_corr[1][1] = set->GetValue("ZeroDeg.ZCorr_RipsBeta5sq", 0.0);
  fzet_corr[1][2] = set->GetValue("ZeroDeg.ZCorr_AoQ5", 0.0);
  fzet_corr[1][3] = set->GetValue("ZeroDeg.ZCorr_AoQ5sq", 0.0);
  fzet_corr[1][4] = set->GetValue("ZeroDeg.ZCorr_Constant", 0.0);

  if (fverbose > 0)
    PrintSettings();
}
/*!
  Print the settings to the screen
*/
void Settings::PrintSettings()
{
  cout << "verbose level\t" << fverbose << endl;
  if (fwithDALI)
    cout << "using DALI" << endl;
  else
    cout << "without DALI" << endl;

  for (int i = 0; i < 6; i++)
    cout << Form("TOF offset.%d\t", i) << ftoffset[i] << endl;

  cout << "BigRIPS.PPAC.File\t" << fPPACfile << endl;
  cout << "BigRIPS.PPAC.Def.File\t" << fPPACdefaultfile << endl;
  cout << "BigRIPS.Plastic.File\t" << fplasticfile << endl;
  cout << "BigRIPS.IC.File\t" << fICfile << endl;
  cout << "BigRIPS.Focal.File\t" << ffocalfile << endl;
  cout << "Matrix.35.File\t" << fmatrixfile[0] << endl;
  cout << "Matrix.57.File\t" << fmatrixfile[1] << endl;
  cout << "Matrix.89.File\t" << fmatrixfile[2] << endl;
  cout << "Matrix.911.File\t" << fmatrixfile[3] << endl;

  cout << "bad channels file\t" << fDALIbadfile << endl;
  cout << "DALI calibration file\t" << fDALIfile << endl;
  if (fdorecal)
  {
    cout << "performing re-calibration with second order" << endl;
    cout << "DALI second order calibration file\t" << fDALIrecalfile << endl;
  }
  cout << "position file\t" << fDALIposfile << endl;
  cout << "timing gate\t" << ftimegate[0] << " to " << ftimegate[1] << endl;

  cout << "addback type\t" << faddbacktype << endl;
  cout << "addback distance\t" << faddbackdistance << endl;
  cout << "addback angle\t" << faddbackangle << endl;
  cout << "addback time difference\t" << faddbacktdiff[0] << " to " << faddbacktdiff[1] << endl;

  cout << "beta\t" << fbeta << endl;
  cout << "align PPAC 3 x0 = " << fppac3align[0] << " , y0 = " << fppac3align[1] << endl;
  cout << "align PPAC 3 x1 = " << fppac3align[2] << " , y1 = " << fppac3align[3] << endl;
  cout << "target position\t" << ftargetposition << endl;
  cout << "gate on F5X position\t" << ff5xgate[0] << " to " << ff5xgate[1] << endl;
  cout << "gate on delta for charge changes BR\t" << fdeltagate[0] << " to " << fdeltagate[1] << endl;
  cout << "gate on delta for charge changes ZD\t" << fdeltagate[2] << " to " << fdeltagate[3] << endl;

  // cout << "BigRIPS.AoQCorr_F3X\t" << faoq_corr[0][0][0] << endl;
  // cout << "BigRIPS.AoQCorr_F3A\t" << faoq_corr[0][0][1] << endl;
  // cout << "BigRIPS.AoQCorr_F5X\t" << faoq_corr[0][1][0] << endl;
  // cout << "BigRIPS.AoQCorr_F5A\t" << faoq_corr[0][1][1] << endl;
  // cout << "BigRIPS.AoQCorr_F7X\t" << faoq_corr[0][2][0] << endl;
  // cout << "BigRIPS.AoQCorr_F7A\t" << faoq_corr[0][2][1] << endl;
  // cout << "ZeroDeg.AoQCorr_F8X\t" << faoq_corr[1][0][0] << endl;
  // cout << "ZeroDeg.AoQCorr_F8A\t" << faoq_corr[1][0][1] << endl;
  // cout << "ZeroDeg.AoQCorr_F9X\t" << faoq_corr[1][1][0] << endl;
  // cout << "ZeroDeg.AoQCorr_F9A\t" << faoq_corr[1][1][1] << endl;
  // cout << "ZeroDeg.AoQCorr_F11X\t" << faoq_corr[1][2][0] << endl;
  // cout << "ZeroDeg.AoQCorr_F11A\t" << faoq_corr[1][2][1] << endl;

  // ===== BigRIPS =====
  cout << "BigRIPS.AoQCorr_F3X\t" << faoq_corr[0][0].X << endl;
  cout << "BigRIPS.AoQCorr_F3A\t" << faoq_corr[0][0].A << endl;
  cout << "BigRIPS.AoQCorr_F3Q\t" << faoq_corr[0][0].Q << endl;
  cout << "BigRIPS.AoQCorr_F3B\t" << faoq_corr[0][0].B << endl;
  cout << "BigRIPS.AoQCorr_F3Y\t" << faoq_corr[0][0].Y << endl;
  cout << "BigRIPS.AoQCorr_F3X2\t" << faoq_corr[0][0].X2 << endl;
  cout << "BigRIPS.AoQCorr_F3A2\t" << faoq_corr[0][0].A2 << endl;
  cout << "BigRIPS.AoQCorr_F3Q2\t" << faoq_corr[0][0].Q2 << endl;
  cout << "BigRIPS.AoQCorr_F3B2\t" << faoq_corr[0][0].B2 << endl;
  cout << "BigRIPS.AoQCorr_F3Y2\t" << faoq_corr[0][0].Y2 << endl;

  cout << "BigRIPS.AoQCorr_F5X\t" << faoq_corr[0][1].X << endl;
  cout << "BigRIPS.AoQCorr_F5A\t" << faoq_corr[0][1].A << endl;
  cout << "BigRIPS.AoQCorr_F5Q\t" << faoq_corr[0][1].Q << endl;
  cout << "BigRIPS.AoQCorr_F5B\t" << faoq_corr[0][1].B << endl;
  cout << "BigRIPS.AoQCorr_F5Y\t" << faoq_corr[0][1].Y << endl;
  cout << "BigRIPS.AoQCorr_F5X2\t" << faoq_corr[0][1].X2 << endl;
  cout << "BigRIPS.AoQCorr_F5A2\t" << faoq_corr[0][1].A2 << endl;
  cout << "BigRIPS.AoQCorr_F5Q2\t" << faoq_corr[0][1].Q2 << endl;
  cout << "BigRIPS.AoQCorr_F5B2\t" << faoq_corr[0][1].B2 << endl;
  cout << "BigRIPS.AoQCorr_F5Y2\t" << faoq_corr[0][1].Y2 << endl;

  cout << "BigRIPS.AoQCorr_F7X\t" << faoq_corr[0][2].X << endl;
  cout << "BigRIPS.AoQCorr_F7A\t" << faoq_corr[0][2].A << endl;
  cout << "BigRIPS.AoQCorr_F7Q\t" << faoq_corr[0][2].Q << endl;
  cout << "BigRIPS.AoQCorr_F7B\t" << faoq_corr[0][2].B << endl;
  cout << "BigRIPS.AoQCorr_F7Y\t" << faoq_corr[0][2].Y << endl;
  cout << "BigRIPS.AoQCorr_F7X2\t" << faoq_corr[0][2].X2 << endl;
  cout << "BigRIPS.AoQCorr_F7A2\t" << faoq_corr[0][2].A2 << endl;
  cout << "BigRIPS.AoQCorr_F7Q2\t" << faoq_corr[0][2].Q2 << endl;
  cout << "BigRIPS.AoQCorr_F7B2\t" << faoq_corr[0][2].B2 << endl;
  cout << "BigRIPS.AoQCorr_F7Y2\t" << faoq_corr[0][2].Y2 << endl;

  // ===== ZeroDegree =====
  cout << "ZeroDeg.AoQCorr_F8X\t" << faoq_corr[1][0].X << endl;
  cout << "ZeroDeg.AoQCorr_F8A\t" << faoq_corr[1][0].A << endl;
  cout << "ZeroDeg.AoQCorr_F8Q\t" << faoq_corr[1][0].Q << endl;
  cout << "ZeroDeg.AoQCorr_F8B\t" << faoq_corr[1][0].B << endl;
  cout << "ZeroDeg.AoQCorr_F8Y\t" << faoq_corr[1][0].Y << endl;
  cout << "ZeroDeg.AoQCorr_F8X2\t" << faoq_corr[1][0].X2 << endl;
  cout << "ZeroDeg.AoQCorr_F8A2\t" << faoq_corr[1][0].A2 << endl;
  cout << "ZeroDeg.AoQCorr_F8Q2\t" << faoq_corr[1][0].Q2 << endl;
  cout << "ZeroDeg.AoQCorr_F8B2\t" << faoq_corr[1][0].B2 << endl;
  cout << "ZeroDeg.AoQCorr_F8Y2\t" << faoq_corr[1][0].Y2 << endl;

  cout << "ZeroDeg.AoQCorr_F9X\t" << faoq_corr[1][1].X << endl;
  cout << "ZeroDeg.AoQCorr_F9A\t" << faoq_corr[1][1].A << endl;
  cout << "ZeroDeg.AoQCorr_F9Q\t" << faoq_corr[1][1].Q << endl;
  cout << "ZeroDeg.AoQCorr_F9B\t" << faoq_corr[1][1].B << endl;
  cout << "ZeroDeg.AoQCorr_F9Y\t" << faoq_corr[1][1].Y << endl;
  cout << "ZeroDeg.AoQCorr_F9X2\t" << faoq_corr[1][1].X2 << endl;
  cout << "ZeroDeg.AoQCorr_F9A2\t" << faoq_corr[1][1].A2 << endl;
  cout << "ZeroDeg.AoQCorr_F9Q2\t" << faoq_corr[1][1].Q2 << endl;
  cout << "ZeroDeg.AoQCorr_F9B2\t" << faoq_corr[1][1].B2 << endl;
  cout << "ZeroDeg.AoQCorr_F9Y2\t" << faoq_corr[1][1].Y2 << endl;

  cout << "ZeroDeg.AoQCorr_F11X\t" << faoq_corr[1][2].X << endl;
  cout << "ZeroDeg.AoQCorr_F11A\t" << faoq_corr[1][2].A << endl;
  cout << "ZeroDeg.AoQCorr_F11Q\t" << faoq_corr[1][2].Q << endl;
  cout << "ZeroDeg.AoQCorr_F11B\t" << faoq_corr[1][2].B << endl;
  cout << "ZeroDeg.AoQCorr_F11Y\t" << faoq_corr[1][2].Y << endl;
  cout << "ZeroDeg.AoQCorr_F11X2\t" << faoq_corr[1][2].X2 << endl;
  cout << "ZeroDeg.AoQCorr_F11A2\t" << faoq_corr[1][2].A2 << endl;
  cout << "ZeroDeg.AoQCorr_F11Q2\t" << faoq_corr[1][2].Q2 << endl;
  cout << "ZeroDeg.AoQCorr_F11B2\t" << faoq_corr[1][2].B2 << endl;
  cout << "ZeroDeg.AoQCorr_F11Y2\t" << faoq_corr[1][2].Y2 << endl;
}

double Settings::GetAoQCorrection(int sp, int fp, const char *var)
{
  if (strcmp(var, "X") == 0)
    return faoq_corr[sp][fp].X;
  else if (strcmp(var, "A") == 0)
    return faoq_corr[sp][fp].A;
  else if (strcmp(var, "Q") == 0)
    return faoq_corr[sp][fp].Q;
  else if (strcmp(var, "B") == 0)
    return faoq_corr[sp][fp].B;
  else if (strcmp(var, "Y") == 0)
    return faoq_corr[sp][fp].Y;
  else if (strcmp(var, "X2") == 0)
    return faoq_corr[sp][fp].X2;
  else if (strcmp(var, "A2") == 0)
    return faoq_corr[sp][fp].A2;
  else if (strcmp(var, "Q2") == 0)
    return faoq_corr[sp][fp].Q2;
  else if (strcmp(var, "B2") == 0)
    return faoq_corr[sp][fp].B2;
  else if (strcmp(var, "Y2") == 0)
    return faoq_corr[sp][fp].Y2;
  else
  {
    cerr << "Unknown AoQ correction variable: " << var << endl;
    return 0.0;
  }
}