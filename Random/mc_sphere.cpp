#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <chrono>
#include <CLHEP/Random/MTwistEngine.h>

double monteCarloSphere(long long N, long seed) {
    CLHEP::MTwistEngine gen;
    gen.setSeed(seed, 0);

    long long inside = 0;
    for (long long i = 0; i < N; i++) {
        // flat() retourne [0,1), on transforme en [-1,1)
        double x = gen.flat() * 2.0 - 1.0;
        double y = gen.flat() * 2.0 - 1.0;
        double z = gen.flat() * 2.0 - 1.0;
        if (x*x + y*y + z*z <= 1.0) inside++;
    }
    return 8.0 * static_cast<double>(inside) / static_cast<double>(N);
}

int main() {
    // N et nombre de réplications associées
    std::vector<long long> Ns = {1000, 1000000, 1000000000};
    std::vector<int> replications_list = {30, 30, 15};  // 15 réplications pour 10^9 modifiable avec un un appareil plus puissant
    
    double volume_exact = 4.0 * M_PI / 3.0;

    for (size_t idx = 0; idx < Ns.size(); idx++) {
        long long N = Ns[idx];
        int replications = replications_list[idx];
        
        // Coefficient t de Student pour IC 95%
        // 29 ddl -> 2.045, 4 ddl -> 2.776
        double t_coeff = (replications == 30) ? 2.045 : 2.776;
        
        std::cout << "\n=== Simulation avec N = " << N << " points (" << replications << " réplications) ===\n";
        std::vector<double> results;

        auto start = std::chrono::high_resolution_clock::now();

        for (int r = 0; r < replications; r++) {
            double estimate = monteCarloSphere(N, 42 + r);
            results.push_back(estimate);
            std::cout << "Réplication " << r << " : " << estimate << std::endl;
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        double mean = std::accumulate(results.begin(), results.end(), 0.0) / replications;

        double var = 0.0;
        for (auto v : results) var += (v - mean) * (v - mean);
        var /= (replications - 1);
        double stddev = std::sqrt(var);

        double margin = t_coeff * stddev / std::sqrt(replications);

        std::cout << "\n--- Résultats ---" << std::endl;
        std::cout << "Volume exact : " << volume_exact << std::endl;
        std::cout << "Estimation moyenne : " << mean << std::endl;
        std::cout << "Écart-type : " << stddev << std::endl;
        std::cout << "IC 95% : [" << mean - margin << ", " << mean + margin << "]" << std::endl;
        std::cout << "Erreur relative : " << 100.0 * std::abs(mean - volume_exact) / volume_exact << " %" << std::endl;
        std::cout << "Temps : " << elapsed.count() << " s" << std::endl;
    }

    return 0;
}
