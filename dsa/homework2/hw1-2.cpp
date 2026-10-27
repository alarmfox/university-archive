#include <bits/stdc++.h>

using namespace std;

void swap(int *a, int *b) {
  int tmp = *a;

  *a = *b;
  *b = tmp;
}

struct heap {
  size_t heap_size;
  size_t capacity;
  int *keys;

  heap(const vector<int> & v) {
    capacity = v.size();
    heap_size = 0;

    keys = new int[capacity];
    for (int i = 0; i < v.size(); i++) {
      keys[i] = v[i];
    }
  
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

  void max_heapify(const int i) {

    int l = heap::left(i);
    int r = heap::right(i);

    int max;
    if (l < heap_size && keys[l] > keys[i]) {
      max = l;
    } else {
      max = i;
    }

    if (r < heap_size && keys[r] > keys[max]) {
      max = r;
    }

    if (max != i) {
      swap(&keys[max], &keys[i]);
      max_heapify(max);
    }
  }

  void heap_increase_key(int i, int key) {
    if (key < keys[i]) {
      cout << "new key smaller then current key";
      return;
    }
    keys[i] = key;
    while(i > 0 && keys[heap::parent(i)] < keys[i]) {
      swap(&keys[heap::parent(i)], &keys[i]);
      i = heap::parent(i);
    }
  }

  void max_heap_insert(const int k) {
    heap_size += 1 ;
    keys[heap_size - 1] = INT_MIN;

    heap_increase_key(heap_size - 1, k);
  }

};
  
void build_max_heap(struct heap *h) {
  h->heap_size = h->capacity;
  for (int i = h->capacity / 2; i >= 0; --i) {
    h->max_heapify(i);
  }
}

void build_max_heap_v2(struct heap *h) {
  h->heap_size = 1;
  for (int i = 1; i < h->capacity; i++) {
    h->max_heap_insert((*h)[i]);
  }
}

int main (int argc, char *argv[]) {
  vector<vector<int>> test_cases = {
    {
      8, 7, 3, 10, 22,
    },
    {
      1, 2, 3, 4, 5, 6,
    },
    {
      -1, 5, 6, -2, -12, 10,
    }

  };

  for (const auto & test_case : test_cases) {
    struct heap h1(test_case), h2(test_case);
    build_max_heap(&h1);
    build_max_heap_v2(&h2);

    cout << "BUILD_MAX_HEAP: ";
    for (int i = 0; i < h1.heap_size; ++i) {
      cout << h1[i] << ' ';
    }
    cout << endl;
    cout << "BUILD_MAX_HEAP_V2: ";
    for (int i = 0; i < h1.heap_size; ++i) {
      cout << h2[i] << ' ';
    }
    cout << endl;
  }
  
  return 0;
}
