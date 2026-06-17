#!/bin/bash

# Persistence -i rootfiles/ddas/salva/1010_50C_tof_fixed.root -o per_test_1010.root -s set_50Ca_C.dat -tn tr

# Persistence -i ./rootfiles/ddas/salva/meta/meta_1010_plcWContinue.root \
#             -o ./rootfiles/ddas/salva/pers/per_1010.root \
#             -s ./settings/set_50Ca_C.dat -tn tr \
#             -br rootfiles/ddas/salva/meta/br50Ca.root \
#             -zd rootfiles/ddas/salva/meta/zd50Ca.root 


# Persistence -i ./rootfiles/ddas/salva/meta/meta_1010.root -o per_1010.root -s ./settings/set_50Ca_C.dat -tn tr 
# Persistence -i ./rootfiles/ddas/salva/1010_50C_tof_fixed.root -o per_1010.root -s ./settings/set_50Ca_C.dat -tn tr


# Persistence -i ./rootfiles/ddas/salva/disi/disi_1010.root \
#             -o ./rootfiles/ddas/salva/pers/pers_disi_1010.root \
#             -s ./settings/set_50Ca_C.dat \
#             -tn tr_in50Caout50Ca \
#             -b 0.5269
            
# Persistence -i ./rootfiles/ddas/disi/disi_1010_kw.root \
#             -o ./rootfiles/ddas/pers/pers_in51Caout50Ca_1010_kw.root \
#             -s ./settings/set_50Ca_C.dat \
#             -tn tr_in51Caout50Ca 

# Persistence -i ./rootfiles/ddas/disi/disi_1010.root \
#             -o ./rootfiles/ddas/pers/pers_in51Caout50Ca_1010.root \
#             -s ./settings/set_50Ca_C.dat \
#             -tn tr_in51Caout50Ca 

# Persistence -i ./rootfiles/ddas/disi/disi_1010.root \
#             -o ./rootfiles/ddas/pers/pers_in51Caout50Ca_1010_new.root \
#             -s ./settings/set_50Ca_C.dat \
#             -tn tr_in51Caout50Ca 

# Persistence -i ./rootfiles/ddas/disi/disi_1010.root \
#             -o ./rootfiles/ddas/pers/pers_in51Caout50Ca_1010_newCalib_20260130.root \
#             -s ./settings/set_50Ca_C.dat \
#             -tn tr_in51Caout50Ca 

# Doing it for Time Calibration for 1000,2000,3000 series
# Persistence -i ./rootfiles/ddas/meta/meta_2004.root ./rootfiles/ddas/meta/meta_2005.root ./rootfiles/ddas/meta/meta_2006.root ./rootfiles/ddas/meta/meta_2007.root \
#             -o ./rootfiles/ddas/pers/pers_tr_all_iso_2000.root \
#             -s ./settings/set_50Ca_Au.dat \
#             -tn tr -wt 0

# # Running for different beta values
# Persistence -i ./rootfiles/ddas/disi/disi_20*.root \
#             -o ./rootfiles/ddas/pers/pers_tr_in49Kout49K_all_2000_beta5136.root \
#             -s ./settings/set_50Ca_Au.dat \
#             -tn tr_in49Kout49K -wt 0 -b 0.5136

# Persistence -i ./rootfiles/ddas/disi/disi_20*.root \
#             -o ./rootfiles/ddas/pers/pers_tr_in49Kout49K_all_2000_beta54.root \
#             -s ./settings/set_50Ca_Au.dat \
#             -tn tr_in49Kout49K -wt 0 -b 0.54


# Running for 50Ca_Au 
# Persistence -i ./rootfiles/ddas/disi/new/disi_20*_new.root \
#             -o ./rootfiles/ddas/pers/pers_tr_in50Caout50Ca_all_2000_widerTgate_beta517.root \
#             -s ./settings/50Ca/set_50Ca_Au_50Ca.dat \
#             -tn tr_in50Caout50Ca -wt 0 -b 0.517

# Persistence -i ./rootfiles/ddas/disi/new/disi_20*_new.root \
#             -o ./rootfiles/ddas/pers/pers_tr_in50Caout50Ca_all_2000_widerTgate_beta5207.root \
#             -s ./settings/50Ca/set_50Ca_Au_50Ca.dat \
#             -tn tr_in50Caout50Ca -wt 0 -b 0.5207

# Doing it for time calibration for 5000, 6000 series
# Persistence -i ./rootfiles/ddas/meta/meta_6003.root ./rootfiles/ddas/meta/meta_6004.root ./rootfiles/ddas/meta/meta_6005.root \
#             -o ./rootfiles/ddas/pers/pers_tr_all_iso_6000.root \
#             -s ./settings/set_51K_Au.dat \
#             -tn tr -wt 0
         
# To fix timeCalibration finally for 1000,2000,3000 series 
# (after I compared the ANAROOT for my version and During_Experiment version, 
# and after I fixed the VME and other stuffs applied in TArtCalibDALI.hh, TArtDALINaI.hh, TArtBeam.hh)

# ./run_meta_parallel.sh 2008 2009 2010 2011 2012

# Persistence -i ./rootfiles/ddas/meta/meta_2008.root ./rootfiles/ddas/meta/meta_2009.root ./rootfiles/ddas/meta/meta_2010.root ./rootfiles/ddas/meta/meta_2011.root ./rootfiles/ddas/meta/meta_2012.root \
#             -o ./rootfiles/ddas/pers/pers_tr_all_iso_2000.root \
#             -s ./settings/set_49K_Au.dat \
#             -tn tr wt 0

# To calculate toffset for 3000 series, since the 2000 series dosen't work on 3000, except 1000 & 2000 series

Persistence -i ./rootfiles/ddas/meta/meta_3002.root ./rootfiles/ddas/meta/meta_3003.root \
            -o ./rootfiles/ddas/pers/pers_tr_all_iso_3000.root \
            -s ./settings/set_49K_Be.dat \
            -tn tr -wt 0