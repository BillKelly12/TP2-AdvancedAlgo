#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Allocateur contigu 2D
int **alloc_matrix(int M, int N) {
    int **mat = malloc(M * sizeof(int *));
    if (!mat) { perror("malloc"); exit(1); }
    mat[0] = malloc(M * (N+1) * sizeof(int));
    if (!mat[0]) { perror("malloc"); exit(1); }
    for (int i = 1; i < M; i++)
        mat[i] = mat[0] + i * (N+1);
    return mat;
}

void free_matrix(int **mat) {
    free(mat[0]);
    free(mat);
}

// Lecture du fichier data.txt
int **read_matrix_from_file(const char *filename, int *outM, int *outN) {
    FILE *f = fopen(filename, "r");
    if (!f) { perror("Erreur ouverture fichier"); exit(1); }

    if (fscanf(f, "%d %d", outM, outN) != 2) {
        fprintf(stderr, "Erreur lecture M et N\n"); exit(1);
    }

    int M = *outM;
    int N = *outN;

    int **b = alloc_matrix(M, N);

    for (int i = 0; i < M; i++) {
        for (int j = 0; j <= N; j++) {
            if (fscanf(f, "%d", &b[i][j]) != 1) {
                fprintf(stderr, "Erreur lecture matrice b[%d][%d]\n", i, j);
                exit(1);
            }
        }
    }

    fclose(f);
    return b;
}

// Fonction de résolution
void solve_cageots_de_fraise(int **b, int M, int N) {
    int **choixOpt = alloc_matrix(M, N);
    double *Benef = malloc((N+1) * sizeof(double));
    if (!Benef) { perror("malloc"); exit(1); }

    // Init
    for (int p = 0; p <= N; ++p) {
        Benef[p] = b[0][p];
        choixOpt[0][p] = p;
    }

    // DP
    for (int m = 1; m < M; ++m) {
        for (int p = N; p >= 0; --p) {
            double max = b[m][p];
            int argmax = p;
            for (int k = 0; k <= p-1; ++k) {
                double tmp = b[m][k] + Benef[p - k];
                if (tmp > max) { max = tmp; argmax = k; }
            }
            Benef[p] = max;
            choixOpt[m][p] = argmax;
        }
    }

    // Reconstruction
    int *repartition = malloc(M * sizeof(int));
    int p = N;
    for (int m = M-1; m >=0; --m) {
        repartition[m] = choixOpt[m][p];
        p -= repartition[m];
    }

    // Affichage
    printf("Bénéfice optimal = %.2f\n", Benef[N]);
    printf("Répartition des cageots :\n");
    for (int m = 0; m < M; m++)
        printf(" Magasin %d : %d cageots\n", m, repartition[m]);

    // Libération
    free(Benef);
    free(repartition);
    free_matrix(choixOpt);
}

// Main
int main() {
    int M, N;
    int **b = read_matrix_from_file("data.txt", &M, &N);

    solve_cageots_de_fraise(b, M, N);

    free_matrix(b);
    return 0;
}
