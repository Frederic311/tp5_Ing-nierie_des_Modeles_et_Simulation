#include <iostream>
#include <cmath>
#include <chrono>
#include <omp.h>

// Déclaration du tableau pré-calculé
extern double tabMT[];

double monteCarloSphere_lookup(long long N, long long startIndex) {
    long long inside = 0;
    long long idx = startIndex;

    for (long long i = 0; i < N; i++) {
        double x = tabMT[idx++] * 2.0 - 1.0;
        double y = tabMT[idx++] * 2.0 - 1.0;
        double z = tabMT[idx++] * 2.0 - 1.0;
        if (x*x + y*y + z*z <= 1.0) inside++;
    }
    return 8.0 * static_cast<double>(inside) / static_cast<double>(N);
}

int main() {
    long long N = 100000; // points par réplication
    int replications = 30;

    auto start = std::chrono::high_resolution_clock::now();

    double results[30];

    // Boucle parallèle avec OpenMP
    #pragma omp parallel for
    for (int r = 0; r < replications; r++) {
        long long startIndex = r * 3LL * N;
        results[r] = monteCarloSphere_lookup(N, startIndex);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    for (int r = 0; r < replications; r++) {
        std::cout << "Réplication " << r << " : volume estimé = " << results[r] << std::endl;
    }

    std::cout << "Temps total (parallèle OpenMP, 30 réplications) : "
              << elapsed.count() << " secondes" << std::endl;

    return 0;
}