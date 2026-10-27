#include <ostream>

template <typename T> class CircularBuffer {

private:
    static const int n = 10;
    T v[n];
    int head;
    int tail;
    int riemp;

private:
    inline bool is_empty() const { return riemp == 0; }
    inline bool is_full() const { return riemp == n; }

public:
    CircularBuffer() { head = tail = riemp = 0; }
    bool push(const T&);
    const T& pop();
    void print(std::ostream&) const;

};

template <typename T> bool CircularBuffer<T>::push(const T& e) {
    if (is_full()) return false;
    v[tail] = e;
    tail = (tail + 1) % n;
    riemp++;
    return true;

}
template <typename T> const T& CircularBuffer<T>::pop() {
    auto e = v[head];
    head = (head + 1) % n;
    riemp--;
    return e;

}
template <typename T> void CircularBuffer<T>::print(std::ostream& out)const {
    for (int i = head; i < tail; i++) {
        out << v[i] << std::endl;
    }
}
