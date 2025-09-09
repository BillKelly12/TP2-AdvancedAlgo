#include <stdlib.h>
#include <stdio.h>
#define N 100
#define M 100
int main(int argc, char *argvar[])
{
}

void solve_cageots_de_fraise(int n, int m)
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
}