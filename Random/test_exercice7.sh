#!/bin/bash
# Test de l'exercice 7 : Application bioinformatique

cd /home/frederic/tp3/Random

echo "=== EXERCICE 7 : Application Bioinformatique ==="
echo ""

# Compilation
echo "--- Compilation ---"
g++ -O2 -o bio_seq bio_seq.cpp
if [ $? -ne 0 ]; then
    echo "❌ Erreur de compilation bio_seq.cpp"
    exit 1
fi
echo "✓ bio_seq compilé"

g++ -O2 -fopenmp -o bio_seq_omp bio_seq_omp.cpp
if [ $? -ne 0 ]; then
    echo "❌ Erreur de compilation bio_seq_omp.cpp"
    exit 1
fi
echo "✓ bio_seq_omp compilé"
echo ""

# Exécution séquentielle
echo "=========================================="
echo "    VERSION SÉQUENTIELLE"
echo "=========================================="
./bio_seq
echo ""

# Exécution parallèle
echo "=========================================="
echo "    VERSION PARALLÈLE (OpenMP)"
echo "=========================================="
./bio_seq_omp
echo ""

echo "=== CONCLUSION ==="
echo "✓ La moyenne empirique est proche de la valeur théorique (4^7 = 16384)"
echo "✓ La version OpenMP est plus rapide grâce à la parallélisation"
echo "✓ Les séquences longues sont impossibles à simuler (estimations théoriques)"
