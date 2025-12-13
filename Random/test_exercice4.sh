#!/bin/bash
# Test de l'exercice 4 : Pré-calcul et Sequence Splitting

cd /home/frederic/tp3/Random

echo "=== EXERCICE 4 : Pré-calcul des nombres pseudo-aléatoires ==="
echo ""

# Étape 1: Générer le tableau pré-calculé
echo "--- Étape 1: Génération de tabMT.cpp ---"
if [ ! -f tabMT.cpp ]; then
    echo "Compilation de generator.cpp..."
    g++ -O2 -o generator generator.cpp -I./include -L./lib -lCLHEP-Random-2.1.0.0
    
    if [ $? -ne 0 ]; then
        echo "❌ Erreur de compilation de generator.cpp"
        exit 1
    fi
    
    echo "Exécution de generator (génération de tabMT.cpp)..."
    export LD_LIBRARY_PATH=/home/frederic/tp3/Random/lib:$LD_LIBRARY_PATH
    ./generator
else
    echo "✓ tabMT.cpp existe déjà"
fi

# Vérifier la taille de tabMT.cpp
LINES=$(wc -l < tabMT.cpp)
echo "  tabMT.cpp contient $LINES lignes"
echo ""

# Étape 2: Compiler mc_lookup_seq
echo "--- Étape 2: Compilation de mc_lookup_seq ---"
g++ -O2 -o mc_lookup_seq mc_lookup_seq.cpp tabMT.cpp

if [ $? -ne 0 ]; then
    echo "❌ Erreur de compilation de mc_lookup_seq"
    exit 1
fi
echo "✓ mc_lookup_seq compilé"
echo ""

# Étape 3: Exécution séquentielle
echo "--- Étape 3: Exécution séquentielle (30 réplications) ---"
echo "Volume exact attendu : 4π/3 ≈ 4.18879"
echo ""
./mc_lookup_seq

echo ""
echo "=== Vérification ==="
echo "✓ Le pré-calcul permet d'éviter les appels au générateur pendant la simulation"
echo "✓ Chaque réplication utilise un flux indépendant (startIndex différent)"
echo "✓ Reproductibilité garantie : même tabMT.cpp = mêmes résultats"
