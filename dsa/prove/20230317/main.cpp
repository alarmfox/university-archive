#include <bits/stdc++.h>
using namespace std;

//I traghetti fluviali servono sono usati per trasportare auto da una riva all’altra di un fiume. Le auto
// salgono sul traghetto da un'estremità, il traghetto attraversa il fiume e le auto escono dall'altra
// estremità del traghetto. Un traghetto può portare n auto in t minuti e tornare in t minuti.
// Sia m il numero di auto che arrivano al terminal dei traghetti entro un determinato orario.
// Qual è il minimo tempo entro cui tutte le auto possono essere trasportate attraverso il fiume? Qual
// è il numero minimo di viaggi che l'operatore deve effettuare per trasportare tutte le auto entro tale
// tempo?

struct result {
    int time;
    int travels;
    result(int time, int travels): time(time), travels(travels) {}
    void print(ostream& out) const {
        out << time << ' ' << travels;
    }

    friend ostream& operator << (ostream& out, struct result v) {
       v.print(out);
       return out;
    }
};

int max(int v[], int n) {
    int m = v[0];
    for(int i = 1; i < n; ++i) {
        if (v[i] > m) {
            m = v[i];
        }
    }
    return m;
}

// approccio greedy:
// prendo n macchine: le accompagno e ritorno dopo un tempo dato da il tempo_corrente = tempo_corrente + 2t + max(tempo arrivo macchine selezionate)
// con una capacità n e m macchine si faranno al più m/n giri
struct result traghetto (vector<int>& cars ,int n, int t) {
    int m = cars.size();
    int current_time = 0;
    int time_last_car = 0, n_travels = 0;
    int selected[n];
    int remaining = m;

    // cout << "\n\n\n\n\n\n\n";
    // for(int i = 0; i < m; i++) {
    //     cout << cars[i] << ' ';
    // }
    // cout << endl;

    // sto scorrendo il vettore n macchine alla volta
    for (int i = 0; i < m; i+=n) {
        int k = 0;
        // seleziono il sotto vettore [...i,..,i+n,...] 
        for(int j = i; j < min(i+n, remaining); j++) {
            if (cars[j] < 2*t) {
                selected[k++] = cars[j];
            }
        }
        current_time += 2*t + max(selected, k);
        remaining -= n;
        n_travels++;
    }
    struct result r(current_time - t, n_travels);
    return r;
}
// La prima riga di input contiene il numero di casi di test. Ogni test case inizia con n, t, m. Seguono m
// linee, ciascuna delle quali indica l'orario di arrivo di un'auto (in minuti dall'inizio della giornata).

int main(int argc, char const *argv[]) {
    int n, t, m, N;
    vector<int> cars;
    cin >> N;
    while(N--) {
        cin >> n >> t >> m;
        cars.resize(m);
        for(int i = 0; i < m;  ++i) {
            cin >> cars[i]; 
        }
        cout << traghetto(cars, n, t) << endl;
    }
    return 0;
}