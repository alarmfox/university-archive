#include <iomanip>
#include <iostream>

struct BinaryTree {
  int v;
  BinaryTree *parent, *sibling, *child;
  BinaryTree(const int val) {
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
void dfs(BinaryTree *t) {
  if (t != nullptr) {
    // pre-order
    // function()
    dfs(t->sibling);

    // in-order
    // function()

    dfs(t->child);

    // post order
    // function()
  }
}

// esempio contare i nodi di un albero
// uso la post order visit
int count(BinaryTree *t) {
  if (!t) {
    return 0;
  }
  int cl = count(t->sibling);
  int cr = count(t->child);

  return cr + cl + 1;
}

void remove(BinaryTree *t) {
  if (t) {
    remove(t->sibling);
    remove(t->child);
    delete t;
    t = nullptr;
  }
}

void print(BinaryTree *t, const int level) {
  if (t != nullptr) {
    print(t->sibling, level + 1);
    std::cout << std::setw(level * 20) << t->v << std::setw(level * 20)
              << std::endl;
    print(t->child, level + 1);
  }
}

void insert_left(BinaryTree* t, BinaryTree* u) {
  if (!t->sibling) {
    u->parent = t;
    t->sibling = u;
  }
}

void insert_right(BinaryTree* t, BinaryTree* u) {
  if (!t->child) {
    u->parent = t;
    t->child = u;
  }
}

int main() {
  BinaryTree *bt = new BinaryTree(10);
  BinaryTree *bt2 = new BinaryTree(0);
  
  insert_left(bt, new BinaryTree(6));
  insert_right(bt,  new BinaryTree(12));
  insert_right(bt->sibling,new BinaryTree (5));
  insert_right(bt->child, new BinaryTree(11));

  insert_left(bt2, new BinaryTree(8));
  insert_right(bt2,  new BinaryTree(19));
  insert_left(bt2->sibling,new BinaryTree (55));
  insert_right(bt2->child, new BinaryTree(32));

  insert_left(bt->sibling, bt2);

  std::cout << count(bt) << std::endl;
  print(bt, 0);
  remove(bt);
  return 0;
}
