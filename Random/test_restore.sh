#!/bin/bash
# filepath: /home/frederic/tp3/Random/test_restore.sh

echo "=== Test de répétabilité MT ==="
echo ""
echo "--- Exécution de mt_status ---"
./mt_status > output_status.txt
cat output_status.txt

echo ""
echo "--- Exécution de mt_restore ---"
./mt_restore > output_restore.txt
cat output_restore.txt

echo ""
echo "--- Comparaison ---"
# Extraire Séquence 1 de mt_status et comparer avec mt_restore
grep -A10 "Séquence 1" output_status.txt | tail -10 > seq1.txt
grep -A10 "Séquence restaurée" output_restore.txt | tail -10 > restored.txt

if diff -q seq1.txt restored.txt > /dev/null; then
    echo "✓ SUCCÈS : Les séquences sont identiques (répétabilité vérifiée)"
else
    echo "✗ ÉCHEC : Les séquences diffèrent"
    diff seq1.txt restored.txt
fi