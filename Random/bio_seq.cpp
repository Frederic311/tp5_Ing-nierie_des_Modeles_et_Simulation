// Exercice 7 : Application bioinformatique - Génération de séquences ADN
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <cmath>
#include <chrono>
#include <iomanip>

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
    std::cout << "=== EXERCICE 7 : Bioinformatique ===" << std::endl;
    std::cout << std::endl;

    // ========== PARTIE 1 : GATTACA (7 caractères) ==========
    std::string target = "GATTACA";
    int L = target.size();
    std::mt19937 gen(42);

    int replications = 40;
    std::vector<long long> trials(replications);

    std::cout << "--- Simulation : séquence \"" << target << "\" (" << L << " bases) ---" << std::endl;
    std::cout << "Probabilité théorique : 1/4^" << L << " = 1/" << (long long)pow(4, L) << std::endl;
    std::cout << "Nombre moyen d'essais attendu : " << (long long)pow(4, L) << std::endl;
    std::cout << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    for (int r=0; r<replications; r++) {
        long long count = 0;
        while (true) {
            count++;
            std::string seq = randomSequence(L, gen);
            if (seq == target) break;
        }
        trials[r] = count;
        std::cout << "Réplication " << r << " : " << count << " essais" << std::endl;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    // Calcul moyenne et écart-type
    double mean = 0;
    for (auto t : trials) mean += t;
    mean /= replications;

    double var = 0;
    for (auto t : trials) var += (t-mean)*(t-mean);
    var /= (replications - 1);  // Variance non biaisée
    double sd = std::sqrt(var);

    // IC 95% avec t de Student (39 ddl ≈ 2.023)
    double t_coeff = 2.023;
    double margin = t_coeff * sd / std::sqrt(replications);

    std::cout << std::endl;
    std::cout << "--- Résultats statistiques ---" << std::endl;
    std::cout << "Moyenne des essais : " << std::fixed << std::setprecision(0) << mean << std::endl;
    std::cout << "Écart-type : " << sd << std::endl;
    std::cout << "IC 95% : [" << (mean - margin) << ", " << (mean + margin) << "]" << std::endl;
    std::cout << "Valeur théorique : " << (long long)pow(4, L) << std::endl;
    std::cout << "Temps total (séquentiel) : " << std::setprecision(3) << elapsed.count() << " secondes" << std::endl;

    // ========== PARTIE 2 : Estimations théoriques ==========
    std::cout << std::endl;
    std::cout << "=== ESTIMATIONS THÉORIQUES ===" << std::endl;
    std::cout << std::endl;

    // AAATTTGCGTTCGATTAG (18 bases)
    std::string seq18 = "AAATTTGCGTTCGATTAG";
    int L18 = seq18.size();
    double p18 = pow(4, L18);
    std::cout << "--- Séquence \"" << seq18 << "\" (" << L18 << " bases) ---" << std::endl;
    std::cout << "Combinaisons possibles : 4^" << L18 << " = " << std::scientific << p18 << std::endl;
    std::cout << "Probabilité : " << (1.0/p18) << std::endl;
    double time_per_try = elapsed.count() / (mean * replications);  // temps par essai
    double estimated_time = p18 * time_per_try;
    std::cout << "Temps estimé pour 1 réplication : " << estimated_time << " secondes";
    std::cout << " ≈ " << (estimated_time / 3600 / 24) << " jours" << std::endl;
    std::cout << std::endl;

    // "Le hasard n'écrit pas de messages" (ASCII)
    std::string phrase = "Le hasard n'ecrit pas de messages";
    int Lphrase = phrase.size();
    double p_phrase = pow(128, Lphrase);  // 128 caractères ASCII
    std::cout << "--- Phrase \"" << phrase << "\" (" << Lphrase << " chars ASCII) ---" << std::endl;
    std::cout << "Combinaisons : 128^" << Lphrase << " = " << p_phrase << std::endl;
    std::cout << "Probabilité : " << (1.0/p_phrase) << std::endl;
    std::cout << "IMPOSSIBLE à simuler !" << std::endl;
    std::cout << std::endl;

    // Génome humain (3 milliards de bases)
    long long genome_size = 3000000000LL;
    std::cout << "--- Génome humain (" << genome_size << " bases) ---" << std::endl;
    std::cout << "Combinaisons : 4^" << genome_size << std::endl;
    std::cout << "log10 du nombre de combinaisons : " << (genome_size * log10(4)) << std::endl;
    std::cout << "C'est un nombre avec " << (long long)(genome_size * log10(4)) << " chiffres !" << std::endl;
    std::cout << "Pour référence : nombre d'atomes dans l'univers ≈ 10^80" << std::endl;
    std::cout << "Probabilité de générer un génome par hasard : ABSOLUMENT NULLE" << std::endl;

    return 0;
}
