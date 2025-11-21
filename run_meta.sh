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

Metamorphosis -i ./ridf/Gamma25_1010.ridf.gz -o ./rootfiles/ddas/salva/meta/meta_1010_plcWContinue.root -s ./settings/set_50Ca_C.dat

