#include <bits/stdc++.h>
using namespace std;

// Si consideri un algoritmo di tipo Divide et Impera D che opera su un array di interi con 1 ≤ n ≤ 1000
// elementi. Al primo passo l’algoritmo divide l’array in due parti; successivamente continua a dividere
// solo uno dei due sotto-array. Il numero di divisioni da fare e l’insieme dei pivot da usare (ossia i punti
// in cui si dovrà suddividere l’array) sono dati in ingresso. Ad ogni operazione di suddivisione fatta da
// Alg è associato un costo, che è pari al numero di elementi dell’array (o del sotto-array) da
// suddividere. È facile notare che selezioni diverse nell'ordine dei pivot (ossia dove suddividere prima)
// possono portare a costi diversi. Ad esempio, si consideri un array di 100 elementi, e siano i possibili
// pivot 25, 50 e 75. Ci sono diverse scelte. L’algoritmo D può suddividere prima a 25, poi a 50, poi a
// 75. Questo porta ad un costo di 100 elementi + 75 elementi + 50 elementi = 225 perché il primo
// array era di 100 elementi, il secondo, risultante dalla prima suddivisione, di 75 e l'ultima, risultante
// dalla seconda suddivisione, di 50. Un’altra scelta potrebbe essere scegliere come pivot 50, poi a 25,
// poi a 75. Questo porterebbe a un costo di 100 + 50 + 50 = 200, che è migliore. Si progetti un algoritmo
// per determinare il costo minimo che D impiega per suddividere l’array il numero di volte indicato.


int dp[101][101];
    
    int solve(int startstick,int endstick,vector<int>& cuts,int left,int right){

        if(left>right) return 0;

        if(dp[left][right]!=-1){
            return dp[left][right];
        }
         int cost=INT_MAX;
        for(int i=left;i<=right;i++){

            int leftCost=solve(startstick,cuts[i],cuts,left,i-1);
            int rightCost=solve(cuts[i],endstick,cuts,i+1,right);

            int currcost=(endstick-startstick)+leftCost+rightCost;

            cost=min(cost,currcost);
        }

        return dp[left][right]=cost;
    }

    int minCost(int n, vector<int>& cuts) {

        memset(dp,-1,sizeof(dp));
        
        sort(cuts.begin(),cuts.end());

        return solve(0,n,cuts,0,cuts.size()-1);
    }

// L'input è costituito da diversi test. La prima riga di ogni test case conterrà un numero intero positivo
// n che rappresenta la lunghezza dell’array. Si assuma 1 ≤ n ≤ 1000. La riga successiva conterrà il
// numero intero p (n < 50) di pivot da usare.
// La riga successiva è composta da p numeri interi positivi ci (0 < ci < n) che rappresentano i pivot, dati
// in ordine strettamente crescente.
// Un caso con n = 0 rappresenta la fine dell'input.

int main() {
    int n, p;
    vector<int>cuts;
    cin >> n;
    while(n > 0) {
      cin >> p;
      cuts.resize(p);
      for(int i = 0; i < p; ++i) {
        cin>>cuts[i];
      }
      cout << minCost(n, cuts) << endl;
      cin >> n;
    }
    
}