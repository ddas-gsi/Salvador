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

# Doing it for Time Calibration
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

Persistence -i ./rootfiles/ddas/disi/new/disi_20*_new.root \
            -o ./rootfiles/ddas/pers/pers_tr_in50Caout50Ca_all_2000_widerTgate_beta5207.root \
            -s ./settings/50Ca/set_50Ca_Au_50Ca.dat \
            -tn tr_in50Caout50Ca -wt 0 -b 0.5207