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


# Persistence -i ./rootfiles/ddas/disi/disi_1010.root \
#             -o ./rootfiles/ddas/pers/pers_in51Caout50Ca_1010.root \
#             -s ./settings/set_50Ca_C.dat \
#             -tn tr_in51Caout50Ca 
            
Persistence -i ./rootfiles/ddas/disi/disi_1010_kw.root \
            -o ./rootfiles/ddas/pers/pers_in51Caout50Ca_1010_kw.root \
            -s ./settings/set_50Ca_C.dat \
            -tn tr_in51Caout50Ca 