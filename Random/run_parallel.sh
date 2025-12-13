#!/bin/bash

N=30       # nombre de réplications
batch=20   # taille du paquet

for i in $(seq 0 $((N-1))); do
    ./mc_lookup $i > EstimationSV3D-$i &
    # toutes les 20 réplications, attendre avant de continuer
    if (( (i+1) % batch == 0 )); then
        wait
    fi
done

wait
echo "Toutes les réplications sont terminées."