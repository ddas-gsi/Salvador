#!/bin/bash

# Metamorphosis -i ./ridf/Calib/CalibGamma25_0077.ridf.gz -o ./rootfiles/ddas/Calib/calib_Co60_0077.root -s ./settings/set_50Ca_C.dat

# BurningGiraffe -i ./rootfiles/ddas/Calib/calib_Co60_0077.root \
#                -o ./rootfiles/ddas/Calib/Co60_burn_0077.root \
#                -tn tr

# Metamorphosis -i ./ridf/Calib/CalibGamma25_0078.ridf.gz -o ./rootfiles/ddas/Calib/calib_Y88_0078.root -s ./settings/set_50Ca_C.dat

# BurningGiraffe -i ./rootfiles/ddas/Calib/calib_Y88_0078.root \
#                -o ./rootfiles/ddas/Calib/Y88_burn_0078.root \
#                -tn tr

Metamorphosis -i ./ridf/Calib/CalibGamma25_0079.ridf.gz -o ./rootfiles/ddas/Calib/calib_Cs137_0079.root -s ./settings/set_50Ca_C.dat

BurningGiraffe -i ./rootfiles/ddas/Calib/calib_Cs137_0079.root \
               -o ./rootfiles/ddas/Calib/Cs137_burn_0079.root \
               -tn tr

Metamorphosis -i ./ridf/Calib/CalibGamma25_0080.ridf.gz -o ./rootfiles/ddas/Calib/calib_Ba133_0080.root -s ./settings/set_50Ca_C.dat

BurningGiraffe -i ./rootfiles/ddas/Calib/calib_Ba133_0080.root \
               -o ./rootfiles/ddas/Calib/Ba133_burn_0080.root \
               -tn tr