#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void afficher_matrice(double *b, int M, int N) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j <= N; j++) {
            printf("%4.0f ", b[i*(N+1)+j]);
        }
        printf("\n");
    }
}

void solve_cageots_de_fraise(double *b, int M, int N) {
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

int main() {
    const char *filename = "matrice.dat";

    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    // Récupérer la taille du fichier
    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("fstat");
        close(fd);
        return 1;
    }

    // mmap depuis le début
    void *addr = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }

    // Lire l’en-tête (2 int32)
    int *header = (int*) addr;
    int M = header[0];
    int N = header[1];
    double *b = (double*) (header + 2);

    printf("Dimensions lues depuis le fichier : M=%d, N=%d\n", M, N);

    // Utilisation
    afficher_matrice(b, M, N);
    solve_cageots_de_fraise(b, M, N);

    // Nettoyage
    munmap(addr, st.st_size);
    close(fd);

    return 0;
}
