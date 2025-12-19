#!/bin/bash


# ./run_meta.sh
# ./run_disi.sh
# python3 pragmma.py


Metamorphosis -i ./ridf/Gamma25_1010.ridf.gz -o ./rootfiles/ddas/meta/meta_1010.root -s ./settings/set_50Ca_C.dat   

sleep 10

Disintegration -i ./rootfiles/ddas/meta/meta_1010.root \
            -o ./rootfiles/ddas/disi/disi_1010.root \
            -c ./rootfiles/ddas/cuts/disicuts/disi_cuts_50Ca_C.root

sleep 10

python3 pragmma.py