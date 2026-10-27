#include <iostream>

template <typename T> 
class Treap {
private:
    struct Node {
        T data;
        float priority;
        struct Node* left, *right, *parent;
        Node(T x) : data(x), left(nullptr), right(nullptr), parent(nullptr) {
            priority = (float)std::rand() / (float) RAND_MAX;
        }
    };
    static const int count = 10;
    Node* root = nullptr;
    void _print(Node* n, int space, std::ostream& out) const {
        if (!n) return;

        space += Treap::count;

        _print(n -> right, space, out);

        out << std::endl;

        for (int i = Treap::count; i < space; ++i) {
            out<< ' ';
        }

        out << "(" << n->data << "," << n->priority<< ')' << "\n";

        _print(n -> left, space, out);

    }
    void destroy(Node* n) {
        if (n) {
            destroy(n->left);
            destroy(n->right);
            delete n;
        }
    }
    void left_rotate(Node* n) {
        Node* y = n->right;
        n->right = y->left;
        if (y->left) {
            y->left->parent = n;
        }
        y->parent = n->parent;
        if (!n->parent) {
            this->root = y;
        } else if (n == n->parent->left) {
            n->parent->left = y;
        } else {
            n->parent->right = y;
        }
        y->left = n;
        n->parent = y;
    }
    void right_rotate(Node* n) {
        Node* y = n->left;
        n->left = y->right;
        if (y->right) {
            y->right->parent = n;
        }
        y->parent = n->parent;
        if (!n->parent) {
            this->root = y;
        } else if (n == n->parent->right) {
            n->parent->right = y;
        } else {
            n->parent->left = y;
        }
        y->right = n;
        n->parent = y;
    }
    void min_heapify(Node* n) {
        if (!n->parent) {
		return;
        }
        if (n->parent && n->priority >= n->parent->priority) {
            return;
        }

        if (n == n->parent->left) {
            right_rotate(n->parent);
        } else {
            left_rotate(n->parent);
        }
        min_heapify(n);
    }

public:
    void treap_insert(const T& elem) {
        Node* z = new Node(elem);
        Node* y = nullptr;
        Node* x = root;
        while (x) {
            y = x;
            if (z->data < x->data) {
                x = x->left;
            }
            else {
                x = x->right;
            }
        }

        z->parent = y;

        if (!y) {
            root = z;
        } else if (z->data < y->data) {
            y->left = z;
        } else {
            y->right = z;
        }

        if (z->parent) {
            min_heapify(z);
        }
    }
    std::ostream& print(std::ostream& out) const {
        _print(root, 0, out);
        return out;
    }
    inline ~Treap() {destroy(root);}

};


int main() {

    std::srand(time(0));
    Treap<int> t;

    t.treap_insert(5);
    t.treap_insert(4);
    t.treap_insert(3);
    t.treap_insert(2);
    t.treap_insert(1);

    t.print(std::cout);


    
}