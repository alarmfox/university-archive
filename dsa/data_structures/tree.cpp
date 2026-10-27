#include <iomanip>
#include <iostream>
#include <queue>

/*
 * Albero generico. Ogni nodo può avere n figli.
 * Ogni nodo avrà un puntatore al fratello e al primo figlio.
 * Questo basta a consentire una visita in ampiezza.
 *
 */
struct Tree {
  int v;
  Tree *parent, *sibling, *child;
  Tree(const int val) {
    v = val;
    parent = sibling = child = nullptr;
  }
};

/*
in base al tipo di operazione da fare abilitare solo una delle tre:
- pre-order stampa i nodi nell'ordine in cui si visita;
- in-order stampa i nodi dall'ultimo in basso a sinistra;
- post-order stampa i nodi dall'ultimo a destra;
*/
void dfs(Tree *t) {
  if (t) {
    // pre-order visit
    // function()
    Tree *u = t->child;
    while (u) {
      // arrivo fino in fondo
      dfs(u);

      // scorro tutti i fratelli
      u = u->sibling;
    }

    // post-order visit
    // function()
  }
}
/*
 * Procedura iterativa che scorre tutti i figli per ogni livello.
 */
void bfs(Tree *t) {
  std::queue<Tree *> q;
  q.push(t);

  while (!q.empty()) {
    // u è il nodo in considerazione
    Tree *u = q.front();
    q.pop();
    // uso il nodo u
    // function()
    // prendo il primo figlio
    u = u->child;
    while (u) {
      // inserisco tutti i fratelli in coda
      q.push(u);
      u = u->sibling;
    }
  }
}

// esempio contare i nodi di un albero
// uso la bfs
int count(Tree *t) {
  std::queue<Tree *> q;
  q.push(t);
  int count = 0;
  while (!q.empty()) {
    // u è il nodo in considerazione
    Tree *u = q.front();
    q.pop();
    // uso il nodo u
    count++;
    // prendo il primo figlio
    u = u->child;
    while (u) {
      // inserisco tutti i fratelli in coda
      q.push(u);
      u = u->sibling;
    }
  }
  return count;
}

void remove(Tree *t) {
  // rimuovo tutti i figli ricorsivamente
  Tree *u = t->child;
  while (u) {
    Tree *next = u->sibling;
    remove(u);
    u = next;
  }
  // rimuovo il padre
  delete t;
}

void remove_child(Tree *t) {
  if (t->child) {
    Tree *newChild = t->child->sibling;
    remove(t->child);
    t->child = newChild;
  }
}
void remove_sibling(Tree *t) {
  if (t->sibling) {
    Tree *newsibling = t->sibling->sibling;
    remove(t->sibling);
    t->sibling = newsibling;
  }
}

void print(Tree *t, const int level) {
  Tree *u = t->child;
  while (u) {
    Tree *next = u->sibling;
    print(u, level);
    u = next;
  }
  std::cout << std::setw(level * 20) << t->v << std::setw(level * 20)
            << std::endl;
}

void insert_sibling(Tree *t, Tree *u) {
  // inserisco il fratello prima di tutti gli altri fratelli
  u->parent = t->parent;
  u->sibling = t->sibling;
  t->sibling = u;
}

void insert_child(Tree *t, Tree *u) {
  // inserisco il figlio collegandolo a t
  // sposto il figlio corrente come fratello di u
  // assegno u al figlio corrente di t
  u->parent = t;
  u->sibling = t->child;
  t->child = u;
}

int main() {
  Tree *t = new Tree(10);

  print(t, 0);
  remove(t);
  return 0;
}
