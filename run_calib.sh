#!/bin/bash

# Metamorphosis -i ./ridf/Calib/CalibGamma25_0077.ridf.gz -o ./rootfiles/ddas/Calib/calib_Co60_0077.root -s ./settings/set_50Ca_C.dat

# BurningGiraffe -i ./rootfiles/ddas/Calib/calib_Co60_0077.root \
#                -o ./rootfiles/ddas/Calib/Co60_burn_0077.root \
#                -tn tr

# Metamorphosis -i ./ridf/Calib/CalibGamma25_0078.ridf.gz -o ./rootfiles/ddas/Calib/calib_Y88_0078.root -s ./settings/set_50Ca_C.dat

# BurningGiraffe -i ./rootfiles/ddas/Calib/calib_Y88_0078.root \
#                -o ./rootfiles/ddas/Calib/Y88_burn_0078.root \
#                -tn tr

# Metamorphosis -i ./ridf/Calib/CalibGamma25_0079.ridf.gz -o ./rootfiles/ddas/Calib/calib_Cs137_0079.root -s ./settings/set_50Ca_C.dat

# BurningGiraffe -i ./rootfiles/ddas/Calib/calib_Cs137_0079.root \
#                -o ./rootfiles/ddas/Calib/Cs137_burn_0079.root \
#                -tn tr

# Metamorphosis -i ./ridf/Calib/CalibGamma25_0080.ridf.gz -o ./rootfiles/ddas/Calib/calib_Ba133_0080.root -s ./settings/set_50Ca_C.dat

# BurningGiraffe -i ./rootfiles/ddas/Calib/calib_Ba133_0080.root \
#                -o ./rootfiles/ddas/Calib/Ba133_burn_0080.root \
#                -tn tr


# After generating new calibration parameters using calibDALI.C with the three sources Co60, Y88, Cs137,
# we now reprocess the Y88 data with the new calibration parameters. DALI DB: DALI/DALI.HYPATIA.20260130.xml
# This is for the RUNNbr 1000 series, 50,51Ca centered
# Metamorphosis -i ./ridf/Calib/CalibGamma25_0078.ridf.gz -o ./rootfiles/ddas/Calib/calib_Y88_0078_wNewCalibPar_20260130.root -s ./settings/set_50Ca_C.dat

# BurningGiraffe -i ./rootfiles/ddas/Calib/calib_Y88_0078_wNewCalibPar_20260130.root \
#                -o ./rootfiles/ddas/Calib/Y88_burn_0078_wNewCalibPar_20260130.root \
#                -tn tr


# Metamorphosis -i ./ridf/Calib/CalibGamma25_0081.ridf.gz -o ./rootfiles/ddas/Calib/bkg_meta_0081.root -s ./settings/set_50Ca_C.dat

# BurningGiraffe -i ./rootfiles/ddas/Calib/bkg_meta_0081.root \
#                -o ./rootfiles/ddas/Calib/bkg_burn_0081.root \
#                -tn tr

# Persistence -i ./rootfiles/ddas/Calib/bkg_meta_0081.root \
#             -o ./rootfiles/ddas/Calib/bkg_pers_0081.root \
#             -s ./settings/set_50Ca_C.dat \
#             -tn tr 


# Running for efficiency
# BurningGiraffe -i ./rootfiles/ddas/Calib/calib_Y88_0078.root \
#                -o ./rootfiles/ddas/Calib/Y88_burn_0078_eff.root \
#                -tn tr 

# BurningGiraffe -i ./rootfiles/ddas/Calib/calib_Co60_0077.root \
#                -o ./rootfiles/ddas/Calib/Co60_burn_0077_eff.root \
#                -tn tr

# BurningGiraffe -i ./rootfiles/ddas/Calib/calib_Cs137_0079.root \
#                -o ./rootfiles/ddas/Calib/Cs137_burn_0079_eff.root \
#                -tn tr

# Running different calibration runs 28.05.2026
# Running with DALI/DALI.HYPATIA.20260414.xml... it has same energy calibration as DALI/DALI.HYPATIA.20260130.xml

# Metamorphosis -i ./ridf/Calib/CalibGamma25_0078.ridf.gz -o ./rootfiles/ddas/Calib/calib_Y88_0078.root -s ./settings/set_Calib_C.dat

# BurningGiraffe -i ./rootfiles/ddas/Calib/calib_Y88_0078.root \
#                -o ./rootfiles/ddas/Calib/Y88_burn_0078_eff.root \
#                -tn tr

# Metamorphosis -i ./ridf/Calib/CalibGamma25_0077.ridf.gz -o ./rootfiles/ddas/Calib/calib_Co60_0077.root -s ./settings/set_Calib_C.dat

# BurningGiraffe -i ./rootfiles/ddas/Calib/calib_Co60_0077.root \
#                -o ./rootfiles/ddas/Calib/Co60_burn_0077_eff.root \
#                -tn tr

# Metamorphosis -i ./ridf/Calib/CalibGamma25_0079.ridf.gz -o ./rootfiles/ddas/Calib/calib_Cs137_0079.root -s ./settings/set_Calib_C.dat

# BurningGiraffe -i ./rootfiles/ddas/Calib/calib_Cs137_0079.root \
#                -o ./rootfiles/ddas/Calib/Cs137_burn_0079_eff.root \
#                -tn tr

# Metamorphosis -i ./ridf/Calib/CalibGamma25_0105.ridf.gz -o ./rootfiles/ddas/Calib/calib_Ba133_0105.root -s ./settings/set_Calib_C.dat

# BurningGiraffe -i ./rootfiles/ddas/Calib/calib_Ba133_0105.root \
#                -o ./rootfiles/ddas/Calib/Ba133_burn_0105_eff.root \
#                -tn tr

# Metamorphosis -i ./ridf/Calib/CalibGamma25_0104.ridf.gz -o ./rootfiles/ddas/Calib/calib_Y88_0104.root -s ./settings/set_Calib_C.dat

# BurningGiraffe -i ./rootfiles/ddas/Calib/calib_Y88_0104.root \
#                -o ./rootfiles/ddas/Calib/Y88_burn_0104_eff.root \
#                -tn tr

# Metamorphosis -i ./ridf/Calib/CalibGamma25_0106.ridf.gz -o ./rootfiles/ddas/Calib/calib_Co60_0106.root -s ./settings/set_Calib_C.dat

# BurningGiraffe -i ./rootfiles/ddas/Calib/calib_Co60_0106.root \
#                -o ./rootfiles/ddas/Calib/Co60_burn_0106_eff.root \
#                -tn tr