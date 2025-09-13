#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define N 10
#define M 4

// Paramètres de blocking
#define Pb 256
#define Kb 256

// Fonction computeBlock : applique Bellman sur une tuile (P0..P_lim) × (K0..K_lim)
void computeBlock(int m, int P0, int P_lim, int K0, int K_lim,
                  double *b, double *Benef, double *Benef_prev, int choixOpt[M][N+1]) {
    for (int k = K0; k < K_lim; k++) {
        double bk = b[m * (N+1) + k];
        for (int p = P0; p < P_lim; p++) {
            if (p >= k) {
                double val = bk + Benef_prev[p - k];
                if (val > Benef[p]) {
                    Benef[p] = val;
                    choixOpt[m][p] = k;
                }
            }
        }
    }
}

void solve_cageots_de_fraise(double b[M*(N+1)]) {
    static int choixOpt[M][N+1];
    static double Benef[N+1], Benef_prev[N+1];

    // Initialisation avec m=0
    for (int p = 0; p <= N; ++p) {
        Benef_prev[p] = b[0*(N+1) + p];
        choixOpt[0][p] = p;
    }

    // Boucle sur les marchés
    for (int m = 1; m < M; m++) {
        // Étape 1 : initialiser avec b[m][p]
        for (int p = 0; p <= N; p++) {
            Benef[p] = b[m*(N+1) + p];
            choixOpt[m][p] = p;
        }

        // Étape 2 : amélioration avec k par blocs
        for (int P0 = 0; P0 <= N; P0 += Pb) {
            int P_lim = (P0 + Pb <= N+1) ? P0 + Pb : N+1;
            for (int K0 = 0; K0 <= N; K0 += Kb) {
                int K_lim = (K0 + Kb <= N+1) ? K0 + Kb : N+1;
                computeBlock(m, P0, P_lim, K0, K_lim, b, Benef, Benef_prev, choixOpt);
            }
        }

        // Étape 3 : copier Benef → Benef_prev pour le marché suivant
        for (int p = 0; p <= N; p++) {
            Benef_prev[p] = Benef[p];
        }
    }

    // Reconstruction de la solution
    int repartition[M];
    int p = N;
    for (int m = M-1; m >= 0; m--) {
        int choix = choixOpt[m][p];
        repartition[m] = choix;
        p -= choix;
    }

    // Affichage
    printf("Bénéfice optimal = %.0f\n", Benef[N]);
    printf("Répartition des cageots :\n");
    for (int m = 0; m < M; m++) {
        printf("  Magasin %d : %d cageots\n", m, repartition[m]);
    }
}

void remplir_matrice(double b[M*(N+1)]) {
    for (int m = 0; m < M; m++) {
        for (int n = 0; n <= N; n++) {
            b[m*(N+1) + n] = (n > 0) ? n * ceil(log(n)) + ((m*m)%17) : 0;
        }
    }
}

int main() {
    static double b[M*(N+1)];
    remplir_matrice(b);
    solve_cageots_de_fraise(b);
    return 0;
}
