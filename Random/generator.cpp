// gen_tabMT_mt.cpp
#include <iostream>
#include <fstream>
#include <iomanip>
#include <CLHEP/Random/MTwistEngine.h>

int main() {
    // Paramètres
    long long N = 100000;        // points par réplication (ajuste si besoin)
    int replications = 30;       // nombre de réplications
    long long total = 3LL * N * replications; // nombres nécessaires

    // Initialisation du générateur CLHEP Mersenne Twister
    CLHEP::MTwistEngine mtGen;   // seed par défaut ; tu peux mtGen.setSeed(42);
    mtGen.setSeed(42, 0);        // fixe la graine pour reproductibilité (second arg is dummy)

    std::ofstream out("tabMT.cpp");
    if (!out) {
        std::cerr << "Erreur: impossible d'ouvrir tabMT.cpp en écriture\n";
        return 1;
    }

    out << "// Tableau pré-calculé de nombres pseudo-aléatoires (CLHEP::MTwistEngine)\n";
    out << "double tabMT[] = {\n";
    out << std::setprecision(17); // précision double

    for (long long i = 0; i < total; ++i) {
        double val = mtGen.flat(); // génère un double uniforme dans [0,1)
        out << val;
        if (i < total - 1) out << ",\n";
    }

    out << "\n};\n";
    out.close();

    std::cout << "Fichier tabMT.cpp généré avec " << total << " nombres (CLHEP::MTwistEngine)." << std::endl;
    return 0;
}