// Exercice 7 : Version parallèle OpenMP avec Sequence Splitting
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <cmath>
#include <chrono>
#include <iomanip>
#include <omp.h>

std::string randomSequence(int length, std::mt19937 &gen) {
    static const char bases[] = {'A','C','G','T'};
    std::uniform_int_distribution<int> dist(0,3);
    std::string seq;
    for (int i=0; i<length; i++) {
        seq += bases[dist(gen)];
    }
    return seq;
}

int main() {
    std::cout << "=== EXERCICE 7 : Bioinformatique (OpenMP) ===" << std::endl;
    std::cout << "Nombre de threads disponibles : " << omp_get_max_threads() << std::endl;
    std::cout << std::endl;

    std::string target = "GATTACA";
    int L = target.size();
    int replications = 40;
    std::vector<long long> trials(replications);

    std::cout << "--- Simulation : séquence \"" << target << "\" (" << L << " bases) ---" << std::endl;
    std::cout << "Probabilité théorique : 1/4^" << L << " = 1/" << (long long)pow(4, L) << std::endl;
    std::cout << "Nombre moyen d'essais attendu : " << (long long)pow(4, L) << std::endl;
    std::cout << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    // Parallélisation avec OpenMP - chaque thread a son propre générateur (seed différent)
    #pragma omp parallel for
    for (int r=0; r<replications; r++) {
        std::mt19937 gen(42 + r);  // Seed différent = flux indépendant (sequence splitting)
        long long count = 0;
        while (true) {
            count++;
            std::string seq = randomSequence(L, gen);
            if (seq == target) break;
        }
        trials[r] = count;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    // Affichage des résultats
    for (int r=0; r<replications; r++) {
        std::cout << "Réplication " << r << " : " << trials[r] << " essais" << std::endl;
    }

    // Calcul moyenne et écart-type
    double mean = 0;
    for (auto t : trials) mean += t;
    mean /= replications;

    double var = 0;
    for (auto t : trials) var += (t-mean)*(t-mean);
    var /= (replications - 1);
    double sd = std::sqrt(var);

    double t_coeff = 2.023;
    double margin = t_coeff * sd / std::sqrt(replications);

    std::cout << std::endl;
    std::cout << "--- Résultats statistiques ---" << std::endl;
    std::cout << "Moyenne des essais : " << std::fixed << std::setprecision(0) << mean << std::endl;
    std::cout << "Écart-type : " << sd << std::endl;
    std::cout << "IC 95% : [" << (mean - margin) << ", " << (mean + margin) << "]" << std::endl;
    std::cout << "Valeur théorique : " << (long long)pow(4, L) << std::endl;
    std::cout << "Temps total (OpenMP) : " << std::setprecision(3) << elapsed.count() << " secondes" << std::endl;

    return 0;
}
