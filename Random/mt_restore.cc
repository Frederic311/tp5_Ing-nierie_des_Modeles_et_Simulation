#include <iostream>
#include "CLHEP/Random/MTwistEngine.h"

int main() {
    CLHEP::MTwistEngine mt;

    //  Restaurer le statut après 10 tirages
    mt.restoreStatus("status1.dat");

    //  Regénérer 10 nombres (doivent être identiques à la séquence initial du programme mt_status.cc)
    std::cout << "Séquence restaurée :" << std::endl;
    for (int i = 0; i < 10; i++) {
        std::cout << mt.flat() << std::endl;
    }

    return 0;
}