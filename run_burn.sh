#!/bin/bash

# BurningGiraffe -i ./rootfiles/ddas/salva/1010_50C_tof_fixed.root -o ./burn_1010.root tr
# BurningGiraffe -i ./rootfiles/ddas/salva/6095_53Au_PPAC_openGate.root -o ./burn_6095_openGate.root tr
# BurningGiraffe -i ./rootfiles/ddas/salva/6095_53Au_PPAC_openGate_tdcOpen.root -o ./burn_6095_openGate_tdcOpen.root tr
# BurningGiraffe -i ./rootfiles/ddas/salva/1010_50C_tof_fixed.root -o ./burn_1010_test.root tr
# BurningGiraffe -i ./rootfiles/ddas/salva/6095_53Au_PPAC_tsumtdcTxTy_test.root -o ./rootfiles/ddas/salva/burn/burn_6095_tsumtdcTxTy_test.root tr
# BurningGiraffe -i ./rootfiles/ddas/salva/6095_53Au_PPAC_tsumtdcfixed.root -o ./rootfiles/ddas/salva/burn/burn_6095_tsumtdcfixed.root -tn tr
# BurningGiraffe -i ./rootfiles/ddas/salva/meta/meta_6095.root -o ./rootfiles/ddas/salva/burn/burn_6095.root -tn tr
# BurningGiraffe -i ./rootfiles/ddas/salva/meta/meta_1010.root -o ./rootfiles/ddas/salva/burn/burn_1010.root -tn tr
# BurningGiraffe -i ./rootfiles/ddas/salva/meta/meta_1010.root -o ./rootfiles/ddas/salva/burn/burn_1010_test.root -tn tr
# BurningGiraffe -i ./rootfiles/ddas/salva/meta/meta_1010.root -o ./rootfiles/ddas/salva/burn/burn_1010_clean.root -tn tr
# BurningGiraffe -i ./rootfiles/ddas/salva/meta/meta_1010.root -o ./rootfiles/ddas/salva/burn/burn_1010_clean.root -tn tr -lcx 1 -fid 3 -v 2
# BurningGiraffe -i ./rootfiles/ddas/salva/meta/meta_1010.root -o ./rootfiles/ddas/salva/burn/burn_1010_clean_cutX1f3.root -tn tr -v 2 -lcx 1 -fid 3
# BurningGiraffe -i ./rootfiles/ddas/salva/meta/meta_1010.root -o ./rootfiles/ddas/salva/burn/burn_1010_aoqCorr.root -tn tr -v 2


# 50Ca on C target run set
# Run Numbers: 1001 - 1013 (with all cuts applied)
# BurningGiraffe -i ./rootfiles/ddas/salva/meta/meta_1001.root \
#                   ./rootfiles/ddas/salva/meta/meta_1002.root \
#                   ./rootfiles/ddas/salva/meta/meta_1003.root \
#                   ./rootfiles/ddas/salva/meta/meta_1004.root \
#                   ./rootfiles/ddas/salva/meta/meta_1005.root \
#                   ./rootfiles/ddas/salva/meta/meta_1006.root \
#                   ./rootfiles/ddas/salva/meta/meta_1007.root \
#                   ./rootfiles/ddas/salva/meta/meta_1008.root \
#                   ./rootfiles/ddas/salva/meta/meta_1009.root \
#                   ./rootfiles/ddas/salva/meta/meta_1010.root \
#                   ./rootfiles/ddas/salva/meta/meta_1011.root \
#                   ./rootfiles/ddas/salva/meta/meta_1012.root \
#                   ./rootfiles/ddas/salva/meta/meta_1013.root \
#                -o 1000_series_50CaCtgt_set_cutf-1_lc-1.root -tn tr -v 2 \
#                -lcx -1 -fid -1

# # Run Numbers: 1001 - 1013 (with fp: 3,7,8 cuts applied)
# BurningGiraffe -i ./rootfiles/ddas/salva/meta/meta_1001.root \
#                   ./rootfiles/ddas/salva/meta/meta_1002.root \
#                   ./rootfiles/ddas/salva/meta/meta_1003.root \
#                   ./rootfiles/ddas/salva/meta/meta_1004.root \
#                   ./rootfiles/ddas/salva/meta/meta_1005.root \
#                   ./rootfiles/ddas/salva/meta/meta_1006.root \
#                   ./rootfiles/ddas/salva/meta/meta_1007.root \
#                   ./rootfiles/ddas/salva/meta/meta_1008.root \
#                   ./rootfiles/ddas/salva/meta/meta_1009.root \
#                   ./rootfiles/ddas/salva/meta/meta_1010.root \
#                   ./rootfiles/ddas/salva/meta/meta_1011.root \
#                   ./rootfiles/ddas/salva/meta/meta_1012.root \
#                   ./rootfiles/ddas/salva/meta/meta_1013.root \
#                -o ./rootfiles/ddas/salva/burn/1000_series_50CaCtgt_set_cutf37_lc-1.root \
#                -tn tr -v 2 -lcx -1 -fid 37

# Run Numbers: 1001 - 1013 (with no cuts applied)
# BurningGiraffe -i ./rootfiles/ddas/salva/meta/meta_1001.root \
#                  ./rootfiles/ddas/salva/meta/meta_1002.root \
#                  ./rootfiles/ddas/salva/meta/meta_1003.root \
#                  ./rootfiles/ddas/salva/meta/meta_1004.root \
#                  ./rootfiles/ddas/salva/meta/meta_1005.root \
#                  ./rootfiles/ddas/salva/meta/meta_1006.root \
#                  ./rootfiles/ddas/salva/meta/meta_1007.root \
#                  ./rootfiles/ddas/salva/meta/meta_1008.root \
#                  ./rootfiles/ddas/salva/meta/meta_1009.root \
#                  ./rootfiles/ddas/salva/meta/meta_1010.root \
#                  ./rootfiles/ddas/salva/meta/meta_1011.root \
#                  ./rootfiles/ddas/salva/meta/meta_1012.root \
#                  ./rootfiles/ddas/salva/meta/meta_1013.root \
#               -o ./rootfiles/ddas/salva/burn/1000_series_50CaCtgt_set_noCut.root \
#               -tn tr -v 2

# BurningGiraffe -i ./rootfiles/ddas/salva/meta/meta_1010_plMultiHit.root \
#                -o ./rootfiles/ddas/salva/burn/burn_1010_test_plMultiHit.root \
#                -tn tr -v 2 -lcx -1 -fid 37811

# BurningGiraffe -i ./rootfiles/ddas/salva/meta/meta_1010.root \
#                -o ./rootfiles/ddas/salva/burn/burn_1010_f37811_lc-1_AoQ2corr.root \
#                -tn tr -v 2 -lcx -1 -fid 37811

# BurningGiraffe -i ./rootfiles/ddas/salva/meta/meta_1010.root \
#                -o ./rootfiles/ddas/salva/burn/burn_1010_f811_lc-1_AoQ2corr.root \
#                -tn tr -v 2 -lcx -1 -fid 811

# BurningGiraffe -i ./rootfiles/ddas/salva/meta/meta_1010_plMultiHit.root \
#                -o ./rootfiles/ddas/salva/burn/burn_1010_f37_lc-1_AoQ2corr_plMultiHit.root \
#                -tn tr -v 2 -lcx -1 -fid 37

# BurningGiraffe -i ./rootfiles/ddas/salva/meta/meta_1010_plMultiHit.root \
#                -o ./rootfiles/ddas/salva/burn/burn_1010_f37_lc-1_AoQ2corr_plMultiHit_woPlcorr.root \
#                -tn tr -v 2 -lcx -1 -fid 37

# BurningGiraffe -i ./rootfiles/ddas/salva/meta/meta_1010_plMultiHit.root \
#                -o ./rootfiles/ddas/salva/burn/burn_1010_f37_lc-1_AoQ2corr_plMultiHit_wPlGate.root \
#                -tn tr -v 2 -lcx -1 -fid 37 -r 1010

# BurningGiraffe -i ./rootfiles/ddas/salva/meta/meta_1010_plMultiHit.root \
#                -o ./rootfiles/ddas/salva/burn/burn_1010_f37811_lc-1_AoQ25corr.root \
#                -tn tr -v 2 -lcx -1 -fid 37811 -r 1010

# BurningGiraffe -i ./rootfiles/ddas/salva/meta/meta_1010_plMultiHit.root \
#                -o ./rootfiles/ddas/salva/burn/burn_1010_f811_lc-1_AoQ25corr.root \
#                -tn tr -v 2 -lcx -1 -fid 811 -r 1010

# BurningGiraffe -i ./rootfiles/ddas/salva/meta/meta_1010_plMultiHit.root \
#                -o ./rootfiles/ddas/salva/burn/burn_1010_f37811_separate_lc-1_AoQ25corr.root \
#                -tn tr -v 2 -lcx -1 -fid 37811 -r 1010

# BurningGiraffe -i ./rootfiles/ddas/salva/meta/meta_1010_aoqc.root \
#                -o ./rootfiles/ddas/salva/burn/burn_1010_f37811_lc-1_aoqc.root \
#                -tn tr -v 2 -lcx -1 -fid 37811 -r 1010

# BurningGiraffe -i ./rootfiles/ddas/salva/meta/meta_1010_aoqc.root \
#                -o ./rootfiles/ddas/salva/burn/burn_1010_f37_lc-1_aoqc.root \
#                -tn tr -v 2 -lcx -1 -fid 37 -r 1010

# BurningGiraffe -i ./rootfiles/ddas/disi/disi_1010.root \
#                -o ./rootfiles/ddas/burn/burn_1010_gamma.root \
#                -tn tr_in51Caout50Ca -v 2 -lcx -1 -fid 37 -r 1010

# Generating this to define Plastic Cuts for 2000 series runs (09.02.2026)
# BurningGiraffe -i ./rootfiles/ddas/meta/meta_2009.root \
#                -o ./rootfiles/ddas/burn/burn_2009_woPlcuts.root \
#                -tn tr -v 2 

# # After defining the plastic cuts for 2000 series runs, we apply them to the 2009 run (09.02.2026) for testing
# BurningGiraffe -i ./rootfiles/ddas/meta/meta_2009.root \
#                -o ./rootfiles/ddas/burn/burn_2009.root \
#                -tn tr -v 2 -lcx -1 -fid -1 -r 2010

# # For Drift corrections running for whole 2000 series (17.02.2026)
# Run All
# BurningGiraffe -i ./rootfiles/ddas/meta/meta_2*.root \
#                -o ./rootfiles/ddas/burn/burn_2000_all.root \
#                -tn tr -v 2 -lcx -1 -fid -1 -r 2010


# Generating this to define Plastic Cuts for 3000 series runs (12.02.2026)
# BurningGiraffe -i ./rootfiles/ddas/meta/meta_3003.root \
#                -o ./rootfiles/ddas/burn/burn_3003.root \
#                -tn tr -v 2 

# Run All
# BurningGiraffe -i ./rootfiles/ddas/meta/meta_3*.root \
#                -o ./rootfiles/ddas/burn/burn_3000_all.root \
#                -tn tr -v 2 -lcx -1 -fid -1 -r 3003


# # Generationg for Plastic Cuts for 5000 series (12.02.2026)
# BurningGiraffe -i ./rootfiles/ddas/meta/meta_5020.root \
#                -o ./rootfiles/ddas/burn/burn_5020.root \
#                -tn tr -v 2 

# # Run All
# BurningGiraffe -i ./rootfiles/ddas/meta/meta_5*.root \
#                -o ./rootfiles/ddas/burn/burn_5000_all.root \
#                -tn tr -v 2 -lcx -1 -fid -1 -r 5020


# # Generationg for Plastic Cuts for 6000 series (12.02.2026)
# BurningGiraffe -i ./rootfiles/ddas/meta/meta_6030.root \
#                -o ./rootfiles/ddas/burn/burn_6030.root \
#                -tn tr -v 2 

# # Run All
# BurningGiraffe -i ./rootfiles/ddas/meta/meta_6*.root \
#                -o ./rootfiles/ddas/burn/burn_6000_all.root \
#                -tn tr -v 2 -lcx -1 -fid -1 -r 6030

BurningGiraffe -i ./rootfiles/ddas/disi/disi_6*.root \
               -o ./rootfiles/ddas/burn/burn_disi_6000_all.root \
               -tn tr_in51Kout51K -v 2 -lcx -1 -fid -1 -r 6030