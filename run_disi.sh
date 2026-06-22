#!/bin/bash

# 1001 1002 1003 1004 1005 1006 1007 1008 1009 1010 1011 1012 1013

# Disintegration -i ./rootfiles/ddas/meta/meta_1010_plcWContinue.root \
#             -o ./rootfiles/ddas/disi/disi_1010.root \
#             -c ./rootfiles/ddas/cuts/disicuts/cuts50Cadisi.root

# Disintegration -i ./rootfiles/ddas/meta/meta_1010.root \
#             -o ./rootfiles/ddas/disi/disi_1010_like_kw_new.root \
#             -c ./rootfiles/ddas/cuts/disicuts/cuts_50Ca_C.root            

# Disintegration -i ./rootfiles/ddas/meta/meta_1010.root \
#             -o ./rootfiles/ddas/disi/disi_1010.root \
#             -c ./rootfiles/ddas/cuts/disicuts/disi_cuts_50Ca_C.root

# Disintegration -i ./rootfiles/ddas/meta/meta_1002.root \
#             -o ./rootfiles/ddas/disi/disi_1002.root \
#             -c ./rootfiles/ddas/cuts/disicuts/disi_cuts_50Ca_C.root

# Disintegration -i ./rootfiles/ddas/meta/meta_1010_new.root -o ./rootfiles/ddas/disi/disi_1010.root -c ./rootfiles/ddas/cuts/disicuts/disi_cuts_50Ca_C.root

# Disintegration -i ./rootfiles/ddas/meta/meta_1001_new.root -o ./rootfiles/ddas/disi/disi_1001.root -c ./rootfiles/ddas/cuts/disicuts/disi_cuts_50Ca_C_new.root
# Disintegration -i ./rootfiles/ddas/meta/meta_1002_new.root -o ./rootfiles/ddas/disi/disi_1002.root -c ./rootfiles/ddas/cuts/disicuts/disi_cuts_50Ca_C_new.root
# Disintegration -i ./rootfiles/ddas/meta/meta_1003_new.root -o ./rootfiles/ddas/disi/disi_1003.root -c ./rootfiles/ddas/cuts/disicuts/disi_cuts_50Ca_C_new.root
# Disintegration -i ./rootfiles/ddas/meta/meta_1004_new.root -o ./rootfiles/ddas/disi/disi_1004.root -c ./rootfiles/ddas/cuts/disicuts/disi_cuts_50Ca_C_new.root
# Disintegration -i ./rootfiles/ddas/meta/meta_1005_new.root -o ./rootfiles/ddas/disi/disi_1005.root -c ./rootfiles/ddas/cuts/disicuts/disi_cuts_50Ca_C_new.root
# Disintegration -i ./rootfiles/ddas/meta/meta_1006_new.root -o ./rootfiles/ddas/disi/disi_1006.root -c ./rootfiles/ddas/cuts/disicuts/disi_cuts_50Ca_C_new.root
# Disintegration -i ./rootfiles/ddas/meta/meta_1007_new.root -o ./rootfiles/ddas/disi/disi_1007.root -c ./rootfiles/ddas/cuts/disicuts/disi_cuts_50Ca_C_new.root
# Disintegration -i ./rootfiles/ddas/meta/meta_1008_new.root -o ./rootfiles/ddas/disi/disi_1008.root -c ./rootfiles/ddas/cuts/disicuts/disi_cuts_50Ca_C_new.root
# Disintegration -i ./rootfiles/ddas/meta/meta_1009_new.root -o ./rootfiles/ddas/disi/disi_1009.root -c ./rootfiles/ddas/cuts/disicuts/disi_cuts_50Ca_C_new.root
# Disintegration -i ./rootfiles/ddas/meta/meta_1010_new.root -o ./rootfiles/ddas/disi/disi_1010.root -c ./rootfiles/ddas/cuts/disicuts/disi_cuts_50Ca_C_new.root
# Disintegration -i ./rootfiles/ddas/meta/meta_1011_new.root -o ./rootfiles/ddas/disi/disi_1011.root -c ./rootfiles/ddas/cuts/disicuts/disi_cuts_50Ca_C_new.root
# Disintegration -i ./rootfiles/ddas/meta/meta_1012_new.root -o ./rootfiles/ddas/disi/disi_1012.root -c ./rootfiles/ddas/cuts/disicuts/disi_cuts_50Ca_C_new.root
# Disintegration -i ./rootfiles/ddas/meta/meta_1013_new.root -o ./rootfiles/ddas/disi/disi_1013.root -c ./rootfiles/ddas/cuts/disicuts/disi_cuts_50Ca_C_new.root

# 2000 Series
# Disintegration -i ./rootfiles/ddas/meta/meta_2010.root -o ./rootfiles/ddas/disi/disi_2010.root -c ./rootfiles/ddas/cuts/disicuts/disi_Cuts_50Ca_Au_2000.root

# for r in {2004..2021}; do Disintegration -i ./rootfiles/ddas/meta/meta_${r}.root -o ./rootfiles/ddas/disi/disi_${r}.root -c ./rootfiles/ddas/cuts/disicuts/disi_Cuts_50Ca_Au_2000.root; done


# 3000 Series
# Disintegration -i ./rootfiles/ddas/meta/meta_3003.root -o ./rootfiles/ddas/disi/disi_3003.root -c ./rootfiles/ddas/cuts/disicuts/disi_Cuts_50Ca_Be_3000.root

# # 5000 Series
# Disintegration -i ./rootfiles/ddas/meta/meta_5020.root -o ./rootfiles/ddas/disi/disi_5020.root -c ./rootfiles/ddas/cuts/disicuts/disi_Cuts_53Ca_Be_5000.root

# # 6000 Series
# Disintegration -i ./rootfiles/ddas/meta/meta_6030.root -o ./rootfiles/ddas/disi/disi_6030.root -c ./rootfiles/ddas/cuts/disicuts/disi_Cuts_53Ca_Au_6000.root

Disintegration -i ./rootfiles/ddas/meta/meta_2010.root -o ./rootfiles/ddas/disi/disi_2010.root -c ./rootfiles/ddas/cuts/disicuts/disi_Cuts_49K_Au_2000.root