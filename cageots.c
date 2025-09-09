#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#define N 10  
#define M 10

void solve_cageots_de_fraise(int b[M][N])
{
    int choixOpt[M][N + 1]; // Tableau des choix optimaux
    double Benef[N + 1];    // Benefice optimal ( en place , par ligne )
    for (int p = 0; p <= N; ++p)
    { // Init
        Benef[p] = b[0][p];
        choixOpt[0][p] = p;
    }
    for (int m = 1; m < M; ++m)
    {
        for (int p = N; p >= 0; --p)
        {
            int max = b[m][p];
            int argmax = p;
            for (int k = 0; k <= p - 1; ++k)
            {
                double tmp = b[m][k] + Benef[p - k];
                if (tmp > max)
                {
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
    printf("Répartition des cageots :\n");
    for (int m = 0; m < M; ++m) {
        printf(" Magasin %d : %d cageots\n", m, repartition[m]);
    }
    

}
void remplir_matrice(int b[M][N]) {
    // Initialiser le générateur de nombres aléatoires
    srand(time(NULL));

    // Remplissage de la matrice avec des valeurs positives
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            b[i][j] = rand() % 100 + 1; // valeurs entre 1 et 100
        }
    }
}
int main(int argc, char *argvar[])
{
    int b[M][N];
    remplir_matrice(b);
    solve_cageots_de_fraise(b);
}

