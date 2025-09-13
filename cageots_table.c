#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define N 10
#define M 4


void afficher_matrice(double *b) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j <= N; j++) {
            printf("%4.0f ", b[i*(N+1)+j]);
        }
        printf("\n");
    }
}


void solve_cageots_de_fraise(double *b) {
    int *choixOpt = malloc(M * (N+1) * sizeof(int));
    double *Benef = malloc((N+1) * sizeof(double));
    int *repartition = malloc(M * sizeof(int));
    if (!choixOpt || !Benef || !repartition) {
        fprintf(stderr, "Erreur allocation mémoire\n");
        exit(1);
    }

    // Initialisation
    for (int p = 0; p <= N; ++p) {
        Benef[p] = b[0*(N+1) + p];
        choixOpt[0*(N+1) + p] = p;
    }

    // DP pour M marchés
    for (int m = 1; m < M; ++m) {
        for (int p = N; p >= 0; --p) {
            double max = b[m*(N+1) + p];
            int argmax = p;
            for (int k = 0; k <= p-1; ++k) {
                double tmp = b[m*(N+1) + k] + Benef[p - k];
                if (tmp > max) {
                    max = tmp;
                    argmax = k;
                }
            }
            Benef[p] = max;
            choixOpt[m*(N+1) + p] = argmax;
        }
    }

    // Reconstruction solution
    int p = N;
    for (int m = M-1; m >= 0; --m) {
        int choix = choixOpt[m*(N+1) + p];
        repartition[m] = choix;
        p -= choix;
    }

    // Affichage du résultat
    printf("Bénéfice optimal = %.0f\n", Benef[N]);
    printf("Répartition des cageots :\n");
    for (int m = 0; m < M; ++m) {
        printf(" Magasin %d : %d cageots\n", m, repartition[m]);
    }

    // Libération mémoire
    free(choixOpt);
    free(Benef);
    free(repartition);
}

void remplir_matrice(double *b) {
    srand(time(NULL));
    for (int m = 0; m < M; ++m) {
        for (int n = 0; n <= N; ++n) {
            b[m*(N+1)+n] = (n > 0) ? n * ceil(log(n)) + ((m*m)%17) : 0;
        }
    }
}

int main() {
    // Allocation dynamique pour b
    double *b = malloc(M*(N+1) * sizeof(double));
    if (!b) {
        fprintf(stderr, "Erreur allocation matrice\n");
        return 1;
    }

    remplir_matrice(b);
    afficher_matrice(b); // Décommenter si nécessaire
    solve_cageots_de_fraise(b);

    free(b);
    return 0;
}
