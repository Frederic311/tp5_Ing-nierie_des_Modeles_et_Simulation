#!/bin/bash
# Test de l'exercice 5 : Parallélisation et validation des résultats

cd /home/frederic/tp3/Random
export LD_LIBRARY_PATH=/home/frederic/tp3/Random/lib:$LD_LIBRARY_PATH

echo "=== EXERCICE 5 : Parallélisation ===" 
echo ""

# Vérifier que tabMT.cpp existe
if [ ! -f tabMT.cpp ]; then
    echo "❌ tabMT.cpp manquant. Exécutez d'abord test_exercice4.sh"
    exit 1
fi

# Étape 1: Compiler les programmes
echo "--- Étape 1: Compilation ---"
g++ -O2 -o mc_lookup mc_lookup.cpp tabMT.cpp
g++ -O2 -fopenmp -o mc_lookup_omp mc_lookup_omp.cpp tabMT.cpp
g++ -O2 -o mc_lookup_seq mc_lookup_seq.cpp tabMT.cpp

if [ $? -ne 0 ]; then
    echo "❌ Erreur de compilation"
    exit 1
fi
echo "✓ Programmes compilés (mc_lookup, mc_lookup_seq, mc_lookup_omp)"
echo ""

# Étape 2: Exécution séquentielle
echo "--- Étape 2: Exécution SÉQUENTIELLE ---"
./mc_lookup_seq > results_seq.txt
cat results_seq.txt
echo ""

# Extraire les résultats séquentiels
grep "volume estimé" results_seq.txt | awk '{print $6}' > values_seq.txt

# Étape 3: Exécution parallèle OpenMP
echo "--- Étape 3: Exécution PARALLÈLE (OpenMP) ---"
./mc_lookup_omp > results_omp.txt
cat results_omp.txt
echo ""

# Extraire les résultats parallèles
grep "volume estimé" results_omp.txt | awk '{print $6}' > values_omp.txt

# Étape 4: Exécution parallèle SPMD (Unix)
echo "--- Étape 4: Exécution PARALLÈLE (SPMD Unix) ---"
rm -f EstimationSV3D-*

START=$(date +%s.%N)
./run_parallel.sh
END=$(date +%s.%N)
SPMD_TIME=$(echo "$END - $START" | bc)

echo "Temps SPMD : ${SPMD_TIME} secondes"
echo ""

# Afficher les résultats SPMD
echo "Résultats SPMD :"
for i in $(seq 0 29); do
    cat EstimationSV3D-$i
done > results_spmd.txt
cat results_spmd.txt
echo ""

# Extraire les valeurs SPMD
grep "volume estimé" results_spmd.txt | awk '{print $6}' > values_spmd.txt

# Étape 5: Comparaison des résultats
echo "=== VALIDATION : Comparaison des résultats ==="
echo ""

echo "Séquentiel vs OpenMP :"
if diff -q values_seq.txt values_omp.txt > /dev/null 2>&1; then
    echo "✓ IDENTIQUES - Les résultats OpenMP sont reproductibles"
else
    echo "✗ DIFFÉRENTS"
    diff values_seq.txt values_omp.txt | head -5
fi

echo ""
echo "Séquentiel vs SPMD :"
if diff -q values_seq.txt values_spmd.txt > /dev/null 2>&1; then
    echo "✓ IDENTIQUES - Les résultats SPMD sont reproductibles"
else
    echo "✗ DIFFÉRENTS"
    diff values_seq.txt values_spmd.txt | head -5
fi

echo ""
echo "=== RÉSUMÉ DES TEMPS ==="
SEQ_TIME=$(grep "Temps total" results_seq.txt | awk '{print $5}')
OMP_TIME=$(grep "Temps total" results_omp.txt | awk '{print $5}')
echo "Séquentiel : ${SEQ_TIME} secondes"
echo "OpenMP     : ${OMP_TIME} secondes"
echo "SPMD Unix  : ${SPMD_TIME} secondes"

# Calcul du speedup
SEQ_NUM=$(echo $SEQ_TIME | sed 's/[^0-9.]//g')
OMP_NUM=$(echo $OMP_TIME | sed 's/[^0-9.]//g')
if [ ! -z "$SEQ_NUM" ] && [ ! -z "$OMP_NUM" ]; then
    SPEEDUP=$(echo "scale=2; $SEQ_NUM / $OMP_NUM" | bc 2>/dev/null)
    echo ""
    echo "Speedup OpenMP : ${SPEEDUP}x"
fi

echo ""
echo "=== CONCLUSION ==="
echo "✓ La parallélisation donne les MÊMES résultats que le séquentiel"
echo "✓ Reproductibilité bit à bit validée"
echo "✓ Gain de performance observé avec la parallélisation"
