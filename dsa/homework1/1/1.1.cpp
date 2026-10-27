#include <iostream>
#include <vector>
#include <cassert>

#define DEBUG 1

template <typename T>
struct LinkedList {
    struct Node {
        T elem;
        Node* prev, *next;
        Node(const T& e) {elem = e; prev = nullptr; next = nullptr; } 
    };
    Node* head;
    int size;

    LinkedList() { size = 0; head = nullptr; }

    void destroy() {
        struct Node* temp;
        while (head) {
            temp = head->next;
            delete head;
            head = temp;
        }
        head = nullptr;
        size = 0;
    }

    void insert(const T& elem) {
        Node* p = new Node(elem);

        size+=1;
        // se è il primo elemento lo associamo alla testa della lista
        if (!head) {
            head = p;
            return;
        }
        // scorriamo fino in fondo ed aggiungiamo l'elemento in coda
        Node* temp = head;
        while (temp->next) temp = temp->next;
        temp->next = p;
        p->prev = temp;
    }

    void half_reverse() {
        // se la lista ha meno di due elementi non ha senso continuare
        if (size <=2) {
            return;
        }
        // troviamo la metà degli elementi e li scorriamo
        const int mid = size / 2;
        Node* current = head, *prev = nullptr, *next = nullptr, *last = nullptr;
        for (int i = 0; i < mid; ++i) {
            current = current->next;
        }
        // salviamo l'ultimo elemento della prima metà per poi ricollegare alla metà invertita
        last = current -> prev;
        while (current) {
            // swap di un elemento della lista con il suo successivo e "incrementiamo" il puntatore current
            next = current->next;
            current->next = prev;
            prev = current;
            current = next;
        }
        // uniamo la prima metà con quella invertita
        last -> next = prev;
    }

    std::ostream& print(std::ostream& out) const {
        Node* current = head;
        while (current) {
            out << current->elem << ' ';
            current = current->next;
        }
        return out;
    }
};

template <typename T>
void assert_list(const struct LinkedList<T> a, const std::vector<T> & b) {
    assert(a.size == b.size());

    struct LinkedList<T>::Node* current = a.head;
    for (int i = 0;  i < b.size() && current; ++i, current = current->next) {
        assert(b[i] == current->elem);
    }
}

template <typename T>
std::ostream& print_vector(const std::vector<T>& input, std::ostream& out) {
    for (const int elem: input) {
        out << elem << ' ';
    }
    return out;
}

int main() {
    std::vector<std::vector<int>> inputs = {
        { 5, 6, 4, 5, 7, 9 }, 
        { 1, 2, 3 },
        { 1, 2 },
        { 2, 4, 3, 6, 6, 9, 3 },
    };
    std::vector<std::vector<int>> results = {
        { 5, 6, 4, 9, 7, 5 }, 
        { 1, 3, 2 },
        { 1, 2 },
        { 2, 4, 3, 3, 9, 6, 6 },
    };

    struct LinkedList<int> linked_list;
    for (int i = 0; i < inputs.size(); ++i) {
        for (int j = 0; j < inputs[i].size(); ++j) {
            linked_list.insert(inputs[i][j]);
        }
        linked_list.half_reverse();
        if (DEBUG) {
            std::cout<<"INPUT : "; print_vector(inputs[i], std::cout) << std::endl;
            std::cout<<"OUTPUT: "; linked_list.print(std::cout) << "\n\n";
        }
        assert_list(linked_list, results[i]);
        linked_list.destroy();
    }

}