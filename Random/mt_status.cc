#include <iostream>
#include "CLHEP/Random/MTwistEngine.h"

int main() {
    CLHEP::MTwistEngine mt;

    // Sauvegarde du statut initial directement avec le nom du fichier
    mt.saveStatus("status1.dat");

    // Tirer 10 nombres
    std::cout << "Séquence 1 :" << std::endl;
    for (int i = 0; i < 10; i++) {
        std::cout << mt.flat() << std::endl;
    }

    // Sauvegarde après 10 tirages
    mt.saveStatus("status2.dat");

    // Tirer encore 10 nombres
    std::cout << "\nSéquence 2 :" << std::endl;
    for (int i = 0; i < 10; i++) {
        std::cout << mt.flat() << std::endl;
    }

    // Sauvegarde après 20 tirages
    mt.saveStatus("status3.dat");
    

    return 0;
}