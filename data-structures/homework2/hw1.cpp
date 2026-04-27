#include "bits/stdc++.h"

using namespace std;

// Si implementi un algoritmo di ordinamento che sfrutta l’inserimento e la visita in un albero
// binario di ricerca. Dato un vettore di n numeri interi in input, l’algoritmo procede prima ad
// inserire i numeri in un albero binario di ricerca (usando ripetutamente TREE-INSERT per
// inserire i numeri uno alla volta), e poi stampa i numeri in ordine con un attraversamento in
// ordine simmetrico dell’albero. Si analizzi la complessità nel caso peggiore e nel caso
// migliore di per questo algoritmo di ordinamento.
struct node {
  node *left;
  node *right;
  int key;

  node(const int v):left(nullptr),right(nullptr),key(v) {}
  ~node() {
    delete left;
    delete right;
  }
};

void insert(struct node **root, struct node *z) {
  struct node *y = nullptr;
  struct node *x = *root;

  while (x != nullptr) {
    y = x;
    if (z->key < x->key) {
      x = x->left;
    } else {
      x=x->right;
    }
  }

  if (y == nullptr) {
    *root = z;
  } else if (z->key < y->key) {
    y->left = z;
  } else {
    y->right = z;
  }

}

void in_order_print(struct node *root) {
  if (root != nullptr) {
    in_order_print(root->left);
    cout << root->key << ' ';
    in_order_print(root->right);
  }
}

void solve(const vector<int>& a) {
  struct node *tree = nullptr;
  for (int i = 0; i < a.size(); ++i) {
    insert(&tree, new node(a[i]));
  }

  in_order_print(tree);
  delete tree;
}

int main (void) {

  vector<vector<int>> test_cases = {
    {
      8, 7, 3, 10, 22,
    },
    {
      1, 2, 3, 5, 6, 7,
    },
    {
      -1, 5, 6, -2, -12, 10,
    }

  };

  for (const auto & test_case : test_cases) {
    solve(test_case);
    cout << endl;
  }
  return 0;
}
