#!/bin/bash

# Disintegration -i ./rootfiles/ddas/meta/meta_1010_plcWContinue.root \
#             -o ./rootfiles/ddas/disi/disi_1010.root \
#             -c ./rootfiles/ddas/cuts/disicuts/cuts50Cadisi.root

# Disintegration -i ./rootfiles/ddas/meta/meta_1010.root \
#             -o ./rootfiles/ddas/disi/disi_1010_like_kw_new.root \
#             -c ./rootfiles/ddas/cuts/disicuts/cuts_50Ca_C.root            


Disintegration -i ./rootfiles/ddas/meta/meta_1010.root \
            -o ./rootfiles/ddas/disi/disi_1010.root \
            -c ./rootfiles/ddas/cuts/disicuts/disi_cuts_50Ca_C.root



# Disintegration -i ./rootfiles/ddas/meta/meta_1002.root \
#             -o ./rootfiles/ddas/disi/disi_1002.root \
#             -c ./rootfiles/ddas/cuts/disicuts/disi_cuts_50Ca_C.root
