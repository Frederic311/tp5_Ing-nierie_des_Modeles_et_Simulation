// generate_status.cpp - Génère 30 fichiers de statut pour Sequence Splitting
#include <iostream>
#include <string>
#include <CLHEP/Random/MTwistEngine.h>

int main() {
    CLHEP::MTwistEngine gen;
    gen.setSeed(42, 0);  // Seed fixe pour reproductibilité

    int replications = 30;
    long long spacing = 1000000;  // "Espace" entre chaque statut (1M tirages)

    std::cout << "Génération de 30 fichiers de statut..." << std::endl;
    std::cout << "Espacement : " << spacing << " tirages entre chaque statut" << std::endl;

    for (int r = 0; r < replications; r++) {
        // Sauvegarder le statut actuel
        std::string filename = "status_rep_" + std::to_string(r) + ".dat";
        gen.saveStatus(filename.c_str());
        std::cout << "Sauvegardé : " << filename << std::endl;

        // Avancer le générateur de 'spacing' tirages
        for (long long i = 0; i < spacing; i++) {
            gen.flat();
        }
    }

    std::cout << "\n✓ 30 fichiers de statut générés (status_rep_0.dat à status_rep_29.dat)" << std::endl;
    return 0;
}
