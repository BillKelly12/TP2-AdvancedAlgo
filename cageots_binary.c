#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>

#define N 10
#define M 4

void solve_cageots_de_fraise(double b[M*(N+1)]) {
    int choixOpt[M][N + 1]; // Tableau des choix optimaux
    double Benef[N + 1];    // Bénéfice optimal (en place, par ligne)

    // Initialisation
    for (int p = 0; p <= N; ++p) {
        Benef[p] = b[0 * (N+1) + p];   // b[0][p]
        choixOpt[0][p] = p;
    }

    for (int m = 1; m < M; ++m) {
        for (int p = N; p >= 0; --p) {
            double max = b[m*(N+1) + p];   
            int argmax = p;
            for (int k = 0; k <= p - 1; ++k) {
                double tmp = b[m*(N+1) + k] + Benef[p - k];
                if (tmp > max) {
                    max = tmp;
                    argmax = k;
                }
            }
            Benef[p] = max;
            choixOpt[m][p] = argmax;
        }
    }

    // Reconstruction
    int repartition[M];
    int p = N;
    for (int m = M - 1; m >= 0; --m) {
        int choix = choixOpt[m][p];
        repartition[m] = choix;
        p -= choix;
    }

    // Résultats
    printf("Bénéfice optimal = %.2f\n", Benef[N]);
    printf("Répartition des cageots :\n");
    for (int m = 0; m < M; ++m) {
        printf(" Magasin %d : %d cageots\n", m, repartition[m]);
    }
}

void lire_matrice(const char *nom_fichier, double b[M*(N+1)]) {
    int fd = open(nom_fichier, O_RDONLY);
    if (fd < 0) {
        perror("Erreur ouverture fichier");
        exit(1);
    }

    int dims[2];
    if (read(fd, dims, sizeof(dims)) != sizeof(dims)) {
        perror("Erreur lecture dimensions");
        exit(1);
    }

    int n = dims[0];
    int m = dims[1];
    if (n != N || m != M) {
        fprintf(stderr, "Erreur: dimensions incompatibles (fichier: %d,%d attendu: %d,%d)\n", n, m, N, M);
        exit(1);
    }

    int tmp[M*(N+1)];
    if (read(fd, tmp, sizeof(tmp)) != sizeof(tmp)) {
        perror("Erreur lecture coefficients");
        exit(1);
    }
    close(fd);

    // Conversion en double
    for (int i = 0; i < M*(N+1); i++) {
        b[i] = (double) tmp[i];
    }
}

void afficher_matrice(double b[M*(N+1)]) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j <= N; j++) {
            printf("%4.0f ", b[i * (N+1) + j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s fichier_binaire\n", argv[0]);
        exit(1);
    }

    double b[M*(N+1)];
    lire_matrice(argv[1], b);
    afficher_matrice(b);
    solve_cageots_de_fraise(b);

    return 0;
}
