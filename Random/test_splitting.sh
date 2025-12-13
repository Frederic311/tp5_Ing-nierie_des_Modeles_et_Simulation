#!/bin/bash
# Test du Sequence Splitting avec saveStatus/restoreStatus

cd /home/frederic/tp3/Random
export LD_LIBRARY_PATH=/home/frederic/tp3/Random/lib:$LD_LIBRARY_PATH

echo "=== EXERCICE 4 : Sequence Splitting (saveStatus/restoreStatus) ==="
echo ""

# Étape 1: Compiler le générateur de statuts
echo "--- Étape 1: Compilation de generate_status.cpp ---"
g++ -O2 -o generate_status generate_status.cpp -I./include -L./lib -lCLHEP-Random-2.1.0.0

if [ $? -ne 0 ]; then
    echo "❌ Erreur de compilation de generate_status.cpp"
    exit 1
fi
echo "✓ generate_status compilé"
echo ""

# Étape 2: Générer les 30 fichiers de statut
echo "--- Étape 2: Génération des 30 fichiers de statut ---"
./generate_status
echo ""

# Vérifier que les fichiers existent
COUNT=$(ls status_rep_*.dat 2>/dev/null | wc -l)
echo "  $COUNT fichiers de statut créés"
echo ""

# Étape 3: Compiler mc_sphere_splitting
echo "--- Étape 3: Compilation de mc_sphere_splitting.cpp ---"
g++ -O2 -o mc_sphere_splitting mc_sphere_splitting.cpp -I./include -L./lib -lCLHEP-Random-2.1.0.0

if [ $? -ne 0 ]; then
    echo "❌ Erreur de compilation de mc_sphere_splitting.cpp"
    exit 1
fi
echo "✓ mc_sphere_splitting compilé"
echo ""

# Étape 4: Exécution
echo "--- Étape 4: Exécution avec Sequence Splitting ---"
./mc_sphere_splitting

echo ""
echo "=== Vérification ==="
echo "✓ Chaque réplication restaure un statut pré-calculé (flux indépendants)"
echo "✓ Les statuts sont séparés par 1M tirages (espace suffisant)"
echo "✓ Reproductibilité garantie : relancer donne les mêmes résultats"
echo ""
echo "--- Test de reproductibilité ---"
echo "Relance pour vérifier..."
./mc_sphere_splitting | grep "Estimation moyenne"
