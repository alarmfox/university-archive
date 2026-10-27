#include <bits/stdc++.h>

using namespace std;

void swap(int *a, int *b) {
  int tmp = *a;

  *a = *b;
  *b = tmp;
}

void print_array(int *arr, int n) {
  for(int i = 0; i < n; ++i) {
    cout << arr[i] << ' '; 
  }
  cout << endl;
}

struct heap {
  size_t heap_size;
  size_t capacity;
  int *keys;

  heap(int n) {
    capacity = n;
    heap_size = 0;

    keys = new int[capacity];
  }
  ~heap() {
    heap_size = capacity = 0;
    delete []keys;
    keys = nullptr;
  }
  
  static int parent(int i) {
    return (i-1)/2;
  }
  static int left(int i) {
    return 2*i+1;
  }
  static int right(int i) {
    return 2*i+2;
  }
  int operator[](const int i) const {
    return keys[i];
  }

  void min_heapify(const int i) {

    int l = heap::left(i);
    int r = heap::right(i);

    int min;
    if (l < heap_size && keys[l] < keys[i]) {
      min = l;
    } else {
      min = i;
    }

    if (r < heap_size && keys[r] < keys[min]) {
      min = r;
    }

    if (min != i) {
      swap(&keys[min], &keys[i]);
      min_heapify(min);
    }
  }

  void heap_decrease_key(int i, int key) {
    if (key > keys[i]) {
      cout << "new key greater then current key" << endl;
      return;
    }
    keys[i] = key;
    while(i > 0 && keys[heap::parent(i)] > keys[i]) {
      swap(&keys[heap::parent(i)], &keys[i]);
      i = heap::parent(i);
    }
  }

  void min_heap_insert(const int k) {
    heap_size += 1 ;
    keys[heap_size - 1] = INT_MAX;

    heap_decrease_key(heap_size - 1, k);
  }

  int extract_min() {
    if(heap_size < 1) {
      cout << "heap empty" << endl;
      return INT_MAX;
    } 

    if (heap_size == 1) {
      heap_size--;
      return keys[0];
    }

    int root = keys[0];
    keys[0] = keys[heap_size-1];
    heap_size--;

    min_heapify(0);

    return root;
  }

  bool empty() {
    return heap_size == 0;
  }

  int get_minimum() {
    return keys[0];
  }

};


void sort_k(int arr[], int n, int k) {
  int size = (n == k) ? k : k + 1;

  struct heap h(size);

  for (int i = 0; i < size; ++i) {
    h.min_heap_insert(arr[i]);
  }

  int index = 0;

  for (int i = k + 1; i < n; i++) {
    arr[index++] = h.extract_min();
    h.min_heap_insert(arr[i]);
  }

  while (!h.empty()) {
    arr[index++] = h.extract_min();
  }
}




int main (int argc, char *argv[]) {
  int test_cases[3][5] = {
    {
      8, 7, 3, 10, 22,
    },
    {
      1, 2, 3, 4, 5,
    },
    {
      -1, -12, 6, -2, 5,
    }

  };

  for (int i = 0; i < 3; ++i ) {
    cout << "CASE " << i + 1 << endl;
    cout << "IN: ";
    print_array(test_cases[i], 5);
    sort_k(test_cases[i], 5, 2);
    cout << "OUT: ";
    print_array(test_cases[i], 5);

  }
  
  return 0;
}
