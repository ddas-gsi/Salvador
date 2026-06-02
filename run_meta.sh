#!/bin/bash


#Metamorphosis -i ~/Lustre/gamma/ddas/RIBF249/ridf/Gamma25_6078.ridf.gz -o test.root -s set_50Ca_Be.dat -n 1000 -v 2
# -n 1000 is to run for first 1000 events
# -v 5 is for verbose 5

# Metamorphosis -i ./ridf/Gamma25_6095.ridf.gz -o ./rootfiles/ddas/salva/6095_53Au_final.root -s set_53Ca_Au.dat 
# #Metamorphosis -i ./ridf/Gamma25_6095.ridf.gz -o ./rootfiles/ddas/salva/6095_53Au_tof0_300.root -s set_53Ca_Au_300.dat 
# #Metamorphosis -i ./ridf/Gamma25_6095.ridf.gz -o ./rootfiles/ddas/salva/6095_53Au_tof0_305.root -s set_53Ca_Au_305.dat 

# Metamorphosis -i ./ridf/Gamma25_5020.ridf.gz -o ./rootfiles/ddas/salva/5020_53Be_tof_plus2.root -s set_53Ca_Be_plus2.dat 
# Metamorphosis -i ./ridf/Gamma25_5020.ridf.gz -o ./rootfiles/ddas/salva/5020_53Be_tof_minus2.root -s set_53Ca_Be_minus2.dat 
# Metamorphosis -i ./ridf/Gamma25_5020.ridf.gz -o ./rootfiles/ddas/salva/5020_53Au_tof_fixed.root -s set_53Ca_Be.dat

# Metamorphosis -i ./ridf/Gamma25_3003.ridf.gz -o ./rootfiles/ddas/salva/3003_50Be_tof_mid.root -s set_50Ca_Be.dat 
# Metamorphosis -i ./ridf/Gamma25_3003.ridf.gz -o ./rootfiles/ddas/salva/3003_50Be_tof_plus2.root -s set_50Ca_Be_plus2.dat
# Metamorphosis -i ./ridf/Gamma25_3003.ridf.gz -o ./rootfiles/ddas/salva/3003_50Be_tof_minus2.root -s set_50Ca_Be_minus2.dat

# Metamorphosis -i ./ridf/Gamma25_2010.ridf.gz -o ./rootfiles/ddas/salva/2010_50Au_tof_mid.root -s set_50Ca_Au.dat

# Metamorphosis -i ./ridf/Gamma25_2010.ridf.gz -o ./rootfiles/ddas/salva/2010_50Au_tof_minus2.root -s set_50Ca_Au_minus2.dat 
# Metamorphosis -i ./ridf/Gamma25_2010.ridf.gz -o ./rootfiles/ddas/salva/2010_50Au_tof_plus2.root -s set_50Ca_Au_plus2.dat
# Metamorphosis -i ./ridf/Gamma25_3003.ridf.gz -o ./rootfiles/ddas/salva/3003_50Be_tof_fixed.root -s set_50Ca_Be.dat 

# Metamorphosis -i ./ridf/Gamma25_1010.ridf.gz -o ./rootfiles/ddas/salva/1010_50C_tof_mid.root -s set_50Ca_C.dat
# Metamorphosis -i ./ridf/Gamma25_1010.ridf.gz -o ./rootfiles/ddas/salva/1010_50C_tof_plus2.root -s set_50Ca_C_plus2.dat
# Metamorphosis -i ./ridf/Gamma25_1010.ridf.gz -o ./rootfiles/ddas/salva/1010_50C_tof_minus2.root -s set_50Ca_C_minus2.dat


# Metamorphosis -i ./ridf/Gamma25_2010.ridf.gz -o ./rootfiles/ddas/salva/2010_50Au_tof_fixed.root -s set_50Ca_Au.dat

# Metamorphosis -i ./ridf/Gamma25_1010.ridf.gz -o ./rootfiles/ddas/salva/1010_50C_tof_fixed.root -s set_50Ca_C.dat


# Metamorphosis -i ./ridf/Gamma25_1010.ridf.gz -o ./rootfiles/ddas/salva/1010_50C_tof_PPAC_fixed.root -s set_50Ca_C.dat

# Metamorphosis -i ./ridf/Gamma25_6095.ridf.gz -o ./rootfiles/ddas/salva/6095_53Au_tof_PPAC_fixed.root -s set_53Ca_Au.dat 

# Metamorphosis -i ./ridf/Gamma25_6095.ridf.gz -o ./rootfiles/ddas/salva/6095_53Au_PPAC_openGate.root -s set_53Ca_Au_openGatePPAC.dat
# Metamorphosis -i ./ridf/Gamma25_6095.ridf.gz -o ./rootfiles/ddas/salva/6095_53Au_PPAC_openGate_tdcOpen.root -s set_53Ca_Au_openGatePPAC.dat
# Metamorphosis -i ./ridf/Gamma25_6095.ridf.gz -o ./rootfiles/ddas/salva/6095_53Au_PPAC_tsumtdcTxTy_test.root -s set_53Ca_Au_tsumtdcTxTy_test.dat
# Metamorphosis -i ./ridf/Gamma25_6095.ridf.gz -o ./rootfiles/ddas/salva/6095_53Au_PPAC_tsumtdcfixed.root -s set_53Ca_Au_tsumtdcfixed.dat

# Metamorphosis -i ./ridf/Gamma25_6095.ridf.gz -o ./rootfiles/ddas/salva/meta/meta_6095.root -s set_53Ca_Au.dat

# Metamorphosis -i ./ridf/Gamma25_1010.ridf.gz -o ./rootfiles/ddas/salva/meta/meta_1010.root -s set_50Ca_C.dat

# Metamorphosis -i ./ridf/Gamma25_1010.ridf.gz -o ./rootfiles/ddas/salva/meta/meta_1010_temp.root -s set_50Ca_C.dat -v 1

# Metamorphosis -i ./ridf/Gamma25_1010.ridf.gz -o ./rootfiles/ddas/salva/meta/meta_1010_plMultiHit.root -s ./settings/set_50Ca_C.dat  // Added for Plastic multihit information 24.10.2025

# Metamorphosis -i ./ridf/Gamma25_1010.ridf.gz -o ./rootfiles/ddas/salva/meta/meta_1010_plMultiHit.root -s ./settings/set_50Ca_C.dat -n 100000

# Metamorphosis -i ./ridf/Gamma25_1010.ridf.gz -o ./rootfiles/ddas/salva/meta/meta_1010_aoqc.root -s ./settings/set_50Ca_C.dat  

# Metamorphosis -i ./ridf/Gamma25_1010.ridf.gz -o ./rootfiles/ddas/salva/meta/meta_1010_zetCalc.root -s ./settings/set_50Ca_C.dat  

# Metamorphosis -i ./ridf/Gamma25_1010.ridf.gz -o ./rootfiles/ddas/salva/meta/meta_1010_zetDone.root -s ./settings/set_50Ca_C.dat  

# Metamorphosis -i ./ridf/Gamma25_1010.ridf.gz -o ./rootfiles/ddas/salva/meta/meta_1010_ripsBeta.root -s ./settings/set_50Ca_C.dat  

# Metamorphosis -i ./ridf/Gamma25_1010.ridf.gz -o ./rootfiles/ddas/salva/meta/meta_1010_fzetc_minus2.root -s ./settings/set_50Ca_C_minus2.dat -n 500000
# Metamorphosis -i ./ridf/Gamma25_1010.ridf.gz -o ./rootfiles/ddas/salva/meta/meta_1010_fzetc_plus2.root -s ./settings/set_50Ca_C_plus2.dat -n 500000

# Metamorphosis -i ./ridf/Gamma25_1010.ridf.gz -o ./rootfiles/ddas/salva/meta/meta_1010_plc.root -s ./settings/set_50Ca_C.dat 

# Metamorphosis -i ./ridf/Gamma25_1010.ridf.gz -o ./rootfiles/ddas/salva/meta/meta_1010_plcWContinue.root -s ./settings/set_50Ca_C.dat

# Metamorphosis -i ./ridf/Gamma25_1010.ridf.gz -o ./rootfiles/ddas/meta/meta_1010.root -s ./settings/set_50Ca_C.dat   -n 500000

# Metamorphosis -i ./ridf/Gamma25_1010.ridf.gz -o ./rootfiles/ddas/meta/meta_1010_new.root -s ./settings/set_50Ca_C.dat   -n 500000


# Running after defining plastic cuts for 2000 series
# Metamorphosis -i ./ridf/Gamma25_2010.ridf.gz -o ./rootfiles/ddas/meta/meta_2010.root -s ./settings/set_50Ca_Au.dat   
# Metamorphosis -i ./ridf/Gamma25_2009.ridf.gz -o ./rootfiles/ddas/meta/meta_2009.root -s ./settings/set_50Ca_Au.dat   -n 2000000


# Running for 3000 series
# Metamorphosis -i ./ridf/Gamma25_3003.ridf.gz -o ./rootfiles/ddas/meta/meta_3003.root -s ./settings/set_50Ca_Be.dat   -n 2000000

# Running for 6000 series timeCalibration
# Metamorphosis -i ./ridf/Gamma25_6030.ridf.gz -o ./rootfiles/ddas/meta/meta_6030.root -s ./settings/set_53Ca_Au.dat   

# BurningGiraffe -i ./rootfiles/ddas/meta/meta_6030.root \
#                -o ./rootfiles/ddas/burn/burn_6030.root \
#                -tn tr -v 2 

# Fixing 49K AoQC for 2000 series
# Metamorphosis -i ./ridf/Gamma25_2020.ridf.gz -o ./rootfiles/ddas/meta/meta_2020_minus2.root -s ./settings/set_50Ca_Au_minus2.dat -n 1000000
# Metamorphosis -i ./ridf/Gamma25_2020.ridf.gz -o ./rootfiles/ddas/meta/meta_2020_plus2.root -s ./settings/set_50Ca_Au_plus2.dat -n 1000000
# Metamorphosis -i ./ridf/Gamma25_2020.ridf.gz -o ./rootfiles/ddas/meta/meta_2020.root -s ./settings/set_50Ca_Au.dat -n 1000000

# Empty target runs
# Metamorphosis -i ./ridf/Gamma25_0006.ridf.gz -o ./rootfiles/ddas/meta/empty_target_meta_0006.root -s ./settings/set_49K_Au.dat
# Metamorphosis -i ./ridf/Gamma25_0010.ridf.gz -o ./rootfiles/ddas/meta/empty_target_meta_0010.root -s ./settings/set_49K_Au.dat

# Fixing 49K AoQC for 3000 series
# Metamorphosis -i ./ridf/Gamma25_3002.ridf.gz -o ./rootfiles/ddas/meta/meta_3002_minus2.root -s ./settings/bkp/set_49K_Be_minus2.dat -n 1000000
# Metamorphosis -i ./ridf/Gamma25_3002.ridf.gz -o ./rootfiles/ddas/meta/meta_3002_plus2.root -s ./settings/bkp/set_49K_Be_plus2.dat -n 1000000
# Metamorphosis -i ./ridf/Gamma25_3002.ridf.gz -o ./rootfiles/ddas/meta/meta_3002.root -s ./settings/set_49K_Be.dat -n 1000000

# Fixing 49K AoQC for 1000 series
# Metamorphosis -i ./ridf/Gamma25_1010.ridf.gz -o ./rootfiles/ddas/meta/meta_1010_minus2.root -s ./settings/bkp/set_49K_C_minus2.dat -n 1000000
# Metamorphosis -i ./ridf/Gamma25_1010.ridf.gz -o ./rootfiles/ddas/meta/meta_1010_plus2.root -s ./settings/bkp/set_49K_C_plus2.dat -n 1000000
# Metamorphosis -i ./ridf/Gamma25_1010.ridf.gz -o ./rootfiles/ddas/meta/meta_1010.root -s ./settings/set_49K_C.dat -n 1000000

# Fixing 51K AoQC for 5000 series
# Metamorphosis -i ./ridf/Gamma25_5020.ridf.gz -o ./rootfiles/ddas/meta/meta_5020_minus2.root -s ./settings/bkp/set_51K_Be_minus2.dat -n 1000000
# Metamorphosis -i ./ridf/Gamma25_5020.ridf.gz -o ./rootfiles/ddas/meta/meta_5020_plus2.root -s ./settings/bkp/set_51K_Be_plus2.dat -n 1000000
# Metamorphosis -i ./ridf/Gamma25_5020.ridf.gz -o ./rootfiles/ddas/meta/meta_5020.root -s ./settings/set_51K_Be.dat -n 1000000

# Fixing 51K AoQC for 6000 series
# Metamorphosis -i ./ridf/Gamma25_6030.ridf.gz -o ./rootfiles/ddas/meta/meta_6030_minus2.root -s ./settings/bkp/set_51K_Au_minus2.dat -n 500000
# Metamorphosis -i ./ridf/Gamma25_6030.ridf.gz -o ./rootfiles/ddas/meta/meta_6030_plus2.root -s ./settings/bkp/set_51K_Au_plus2.dat -n 500000
Metamorphosis -i ./ridf/Gamma25_6030.ridf.gz -o ./rootfiles/ddas/meta/meta_6030.root -s ./settings/set_51K_Au.dat -n 500000


