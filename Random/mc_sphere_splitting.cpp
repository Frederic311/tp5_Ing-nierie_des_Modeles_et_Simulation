// mc_sphere_splitting.cpp - Monte Carlo avec Sequence Splitting (restoreStatus)
#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <chrono>
#include <string>
#include <CLHEP/Random/MTwistEngine.h>

double monteCarloSphere(long long N, CLHEP::MTwistEngine& gen) {
    long long inside = 0;
    for (long long i = 0; i < N; i++) {
        double x = gen.flat() * 2.0 - 1.0;
        double y = gen.flat() * 2.0 - 1.0;
        double z = gen.flat() * 2.0 - 1.0;
        if (x*x + y*y + z*z <= 1.0) inside++;
    }
    return 8.0 * static_cast<double>(inside) / static_cast<double>(N);
}

int main() {
    long long N = 100000;  // points par réplication
    int replications = 30;
    double volume_exact = 4.0 * M_PI / 3.0;

    CLHEP::MTwistEngine gen;
    std::vector<double> results;

    std::cout << "=== Monte Carlo avec Sequence Splitting (restoreStatus) ===" << std::endl;
    std::cout << "N = " << N << " points par réplication" << std::endl;
    std::cout << "Volume exact : " << volume_exact << std::endl;
    std::cout << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    for (int r = 0; r < replications; r++) {
        // Restaurer le statut pré-calculé pour cette réplication
        std::string filename = "status_rep_" + std::to_string(r) + ".dat";
        gen.restoreStatus(filename.c_str());

        // Exécuter la simulation
        double estimate = monteCarloSphere(N, gen);
        results.push_back(estimate);
        std::cout << "Réplication " << r << " : " << estimate << std::endl;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    // Calcul des statistiques
    double mean = std::accumulate(results.begin(), results.end(), 0.0) / replications;

    double var = 0.0;
    for (auto v : results) var += (v - mean) * (v - mean);
    var /= (replications - 1);
    double stddev = std::sqrt(var);

    double margin = 2.045 * stddev / std::sqrt(replications);  // t de Student, 29 ddl

    std::cout << "\n--- Résultats ---" << std::endl;
    std::cout << "Estimation moyenne : " << mean << std::endl;
    std::cout << "Écart-type : " << stddev << std::endl;
    std::cout << "IC 95% : [" << mean - margin << ", " << mean + margin << "]" << std::endl;
    std::cout << "Erreur relative : " << 100.0 * std::abs(mean - volume_exact) / volume_exact << " %" << std::endl;
    std::cout << "Temps total (séquentiel) : " << elapsed.count() << " s" << std::endl;

    return 0;
}
