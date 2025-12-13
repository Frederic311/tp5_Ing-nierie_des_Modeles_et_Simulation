#!/bin/bash
# Test du calcul Monte Carlo du volume d'une sphère

echo "=== Compilation de mc_sphere.cpp ==="
cd /home/frederic/tp3/Random

g++ -O2 -o mc_sphere mc_sphere.cpp -I./include -L./lib -lCLHEP-Random-2.1.0.0

if [ $? -ne 0 ]; then
    echo "❌ Erreur de compilation"
    exit 1
fi

echo "✓ Compilation réussie"
echo ""

echo "=== Exécution de mc_sphere ==="
echo "Volume exact attendu : 4π/3 ≈ 4.18879..."
echo ""

export LD_LIBRARY_PATH=/home/frederic/tp3/Random/lib:$LD_LIBRARY_PATH
time ./mc_sphere

echo ""
echo "=== Analyse ==="
echo "Vérifiez que :"
echo "  1. L'intervalle de confiance contient la vraie valeur (4.18879)"
echo "  2. L'écart-type diminue quand N augmente"
echo "  3. L'erreur relative diminue avec N"
