#pragma once
#include <ostream>
template <typename T> class List {

private:
	struct Node {
		T elem;
		Node* next;

	};
	Node* l;
public:
	List() { l = nullptr; }
	~List();
	void insert(const T&);
	void remove (const T&);
	bool is_empty()const { return l == nullptr; }
	void print(std::ostream&)const;

};

template <typename T>List<T>::~List() {
	Node* temp;
	while (l) {
		temp = l;
		l = l->next;
		delete temp;
	}
}

template <typename T> void List<T>::insert(const T& e) {
	Node* p = new Node;
	p->elem = e;
	p->next = l;
	l = p;
}

template <typename T> void List<T>::remove(const T& c) {
	Node* temp = l;
	while (temp->next->elem != c) temp = temp->next;
	Node* temp1 = temp->next;
	temp->next = temp->next->next;
	delete temp1;
}

template <typename T> void List<T>::print(std::ostream& out)const {
	Node* temp = l;
	while (temp) {
		out << temp->elem << std::endl;
		temp = temp->next;
	}
}
