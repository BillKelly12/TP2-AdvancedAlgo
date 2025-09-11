#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#define N 10 
#define M 4

void solve_cageots_de_fraise(double b[M*(N+1)]) {
    int choixOpt[M][N + 1]; // Tableau des choix optimaux
    double Benef[N + 1];    // Bénéfice optimal (en place, par ligne)

    // Initialisation
    for (int p = 0; p <= N; ++p) {
        Benef[p] = b[0 * (N+1) + p];   // b[0][p] → b[p]
        choixOpt[0][p] = p;
    }

    printf("benef \n");
    for (int p = 0; p <= N; ++p) {
        printf("%f ", Benef[p]);
    }
    printf("\n");

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

    // Reconstruction de la solution
    int repartition[M]; // combien de cageots pour chaque marché
    int p = N;          // cageots restants à distribuer
    for (int m = M - 1; m >= 0; --m) {
        int choix = choixOpt[m][p]; // nb cageots donnés au marché m
        repartition[m] = choix;
        p -= choix; // on enlève les cageots attribués
    }

    // Affichage du résultat
    printf("Bénéfice optimal = %.2f\n", Benef[N]);
    printf("benef \n");
    for (int p = 0; p <= N; ++p) {
        printf("%f ", Benef[p]);
    }
    printf("\n");

    printf("Répartition des cageots :\n");
    for (int m = 0; m < M; ++m) {
        printf(" Magasin %d : %d cageots\n", m, repartition[m]);
    }
}

void remplir_matrice(double b[M*(N+1)]) {
    // Initialiser le générateur de nombres aléatoires
    srand(time(NULL));

    // Remplissage de la matrice avec des valeurs positives
    for (int m = 0; m < M; ++m) {
        for(int n = 0; n < N+1; ++n){
            b[m * (N+1) + n] =  n > 0 ? n * ceil(log(n)) + ((m*m)%17) : 0;
        }
    }
}
void afficher_matrice(double b[M*(N+1)]) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j <= N; j++) {
            printf("%1f ", b[i * (N+1) +j]);
        }
        printf("\n");
    }
}
int main(int argc, char *argvar[])
{
    double b[M*(N+1)];
    remplir_matrice(b);
    afficher_matrice(b);
    solve_cageots_de_fraise(b);
}

