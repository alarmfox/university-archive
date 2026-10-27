#include <bits/stdc++.h>
using namespace std;

vector<string> sol;

void hamming_rec(int start, int h, string& s) {

}
// il backtracking prevede l'esplorazione di tutto lo spazio delle soluzioni.
// in questo caso bisogna generare tutte le stringhe di lunghezza n con h bit alti
// quindi in uscita ci sarà un vettore di dimensione (n choose k) (fattore binomiale n k)
// la soluzione può essere impostata come segue:
// partendo da una stringa di tutti 0:
// 1. si alza l'iesimo bit e si riconduce il problema con h-1 con un vettore v[i+1...n]
// 2. non si alza l'iesimo bit e si riconduce il problema a un vettore v[i+1...n]
vector<string> hamming(int n, int h) {
  vector<string> res;
  
  return res;
}

int main (int argc, char *argv[]) {
  int N, n, h;

  cin >> N;
  while(N--) {
    cin >> n >> h;
    
    hamming(n, h); 

    for(const auto& s: sol) {
      cout << s << endl;
    }

  }
  return 0;
}
