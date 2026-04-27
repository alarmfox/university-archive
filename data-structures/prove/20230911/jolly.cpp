#include <bits/stdc++.h>
using namespace std;

// Approccio simile al counting sort
// uso un vettore di appoggio booleano b[0...n-1] in cui memorizzo se un dato valore
// è presente come differenza o meno.
// Ogni elemento di b è inizializzato a false
// abs(v[i] - v[i-1]) = k con 0 < k < n allora b[k] viene messo a true
// Il risultato sarà "false" se viene trovato un valore falso in b altrimenti "true"
// Complessità: O(n)
bool jolly(vector<int>& v) {
  int k;
  int n = v.size();
  bool b[n];

  for(int i = 0; i < n; ++i) {
    b[i] = false;
  }

  for(int i = 1; i < n; ++i) {
    k = abs(v[i] - v[i-1]);
    b[k] = true;
  }


  for(int i = 1; i < n; ++i) {
    if (!b[i]) {
      return false;
    }
  }

  return true;
}

int main (int argc, char *argv[]) {
  int n;
  vector<int> v;

  while(cin >> n) {
    v.resize(n);
    for(int i = 0; i < n; ++i) {
      cin >> v[i];
    }

    if(jolly(v)) {
      cout << "Jolly";
    } else {
      cout << "Not jolly";
    }
    cout << endl;
  }


  
  return 0;
}


