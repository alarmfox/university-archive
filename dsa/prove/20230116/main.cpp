#include <bits/stdc++.h>
using namespace std;

// Due ladri mettono a segno un colpo alla banca d’Italia. Recuperano un bottino composto da monete
// d’oro di diverso valore. Devono dividerselo il più equamente possibile, ossia devono minimizzare la
// differenza tra il valore che ciascuno di loro ottiene. Scrivere un algoritmo per determinare la
// suddivisione, stampando in output la differenza (positiva) tra il valore che i due ladri ottengono
// dividendosi il bottino.
// Il bottino contiene al massimo 50 monete, il valore di ciascuna moneta varia da 1 a 1000. Il valore
// di una moneta può occorrere più di una volta.

// Soluzione: trovare il sottonsieme la cui somma si avvicina di più alla somma/2.
// quindi creo la matrice dp[n][sum/2] e prendo l'ultima riga dp[n][j] ovvero: il sottonsieme la cui somma si avvicina alla metà.
// In caso affermativo la differenza sarà n-2*j
int min_diff(vector<int>& coins) {
    int sum = accumulate(coins.begin(), coins.end(), 0);
    size_t n = coins.size();
    bool dp[n+1][sum+1];
    memset(dp, false, sizeof(dp));

    for(size_t i = 0; i <= n; ++i) {
        dp[i][0] = true;
    }

    for(int i = 1; i <= sum/2; ++i) {
        dp[0][i] = false;
    }


    for(size_t i = 1; i <= n; ++i) {
        for(int j = 1; j <= sum/2; ++j) {
            dp[i][j] = dp[i-1][j];
            
            if(coins[i-1] <= j) {
                dp[i][j] |= dp[i-1][j-coins[i-1]];
            }
        }
    }

    for(int j= sum/2; j >= 0; --j) {
        if(dp[n][j]) {
            return sum-2*j;
        }
    }
    return 0;
}
// La prima riga contiene il numero di casi di test, N. Dopo la prima linea, ogni caso di test è composto
// da 2 linee: la prima riporta un numero intero non negativo M compreso tra 1 e 50, che indica il
// numero di monete; la seconda riporta M numeri interi non negativi compresi tra 1 e 1000, separati
// da uno spazio, che indicano il valore di ciascuna delle M monete.

int main() {
    int N, M;
    vector<int> coins;
    cin >> N;
    while(N--) {
        cin >> M;
        coins.resize(M);
        for(int i = 0; i < M; ++i) {
            cin >> coins[i];
        }
        cout << min_diff(coins) << endl;
    }
}