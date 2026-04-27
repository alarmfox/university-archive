#include <bits/stdc++.h>
using namespace std;


struct job {
  int index;
  int span;
  int penalty;
  float prio;
};


bool sort_by_prio(struct job a, struct job b) {
  if (a.prio == b.prio) {
    return a.index < b.index;
  }

  return a.prio < b.prio;
}

// una soluzione può essere calcolata usando un'euristica greedy:
// si potrebbe ordinare il vettore in base alla penalità
// in modo da eseguire prima i lavori più penalizzanti. Questo approccio 
// è poco efficiente in quanto non tiene traccia della durata del lavoro.
// In ambito greedy, per migliorare la soluzione (mantenendo semplice l'implementazione)
// può essere utile ricavare una metrica attraverso la quale schedulare i lavori.
// In questo caso può essere utile combinare sia il fattore di penalità che la durata 
// di un lavoro con una relazione del tipo:
// p =  span/penalty
// Complessità: O(n) + O(n) + O(n log n) + O(1) = O(n log n)
// - O(n log n) è dato dalla complessità dell'algoritmo di ordinamento usato nella STL;
// - O(1): costo per calcolare il fattore di priorità;
// - O(n): costo per iterare sul vettore;
// NOTA: la soluzione greedy non è ottima
vector<int> schedule_greedy(vector<struct job>& v) {
  int n = v.size();
  vector<int> res;
  res.resize(n);
  

  for(auto& j: v) {
    j.prio = float (j.span) / float(j.penalty);
  }

  sort(v.begin(), v.end(), sort_by_prio);

  for(int i = 0; i < n; ++i) {
    // cout << v[i].index << ' ';
    res[i] = v[i].index;
  }

  // cout << endl;
  return res;
}

int main (int argc, char *argv[]) {
  int N, n;
  vector<struct job> v;
  vector<int> res;
  cin >> N;
  while(N--) {
    cin >> n;
    v.resize(n);

    for(int i = 0; i < n; ++i) {
      cin >> v[i].span >> v[i].penalty;
      v[i].index = i+1;
    }

    res = schedule_greedy(v);

    for(size_t i = 0; i < res.size(); ++i) {
      cout << res[i] << ' ';
    }
    cout << endl;
  }

  return 0;
}
