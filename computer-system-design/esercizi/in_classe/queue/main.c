#include <stdio.h>

#define N 10

struct queue {
  int head;
  int tail;
  unsigned char v[N];
};

void init(struct queue *q) {
  q->tail = q->head = -1;
  for (int i = 0; i < N; ++i) {
    q->v[i] = 0;
  }
}

void print(struct queue *q) {
  if (q->head == -1) {
    printf("empty\n");
    return;
  }
  for (int i = q->head; i != q->tail; i = (i + 1) % N) {
    printf("%d ", q->v[i]);
  }
  printf("%d\n", q->v[q->tail]);
}

short int enqueue(struct queue *q, unsigned char c) {
  short int res = 0;
  if (q->head == -1) {
    q->head = q->tail = 0;
    q->v[0] = c;
  } else if ((q->tail == N - 1) && q->head == 0) {
    res = -1;
  } else {
    q->tail = (q->tail + 1) % N;
    q->v[q->tail] = c;
  }
  return res;
}

unsigned char dequeue(struct queue *q) {
  unsigned char c = 0xff;
  if (q->head != -1) {

    c = q->v[q->head];

    if (q->head == q->tail) {
      q->head = q->tail = -1;
    } else {
      (q->head)++;
    }
  }
  return c;
}

int main() {
  int ret;
  struct queue q;
  unsigned char c;
  unsigned char v1[N + 5] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  unsigned char v2[N + 5] = {0, 0, 255, 0, 127, 31, 15, 0,
                             7, 0, 63,  1, 2,   3,  4};

  init(&q);

  for (int i = 0; i < N + 5; ++i) {
    if (v1[i] & 0x01) {
      ret = enqueue(&q, v2[i]);
      if (ret < 0) {
        printf("enqueue failed: queue is full\n");
      } else {
        printf("enqueue: %d\n", v2[i]);
      }
    } else {
      c = dequeue(&q);
      printf("dequeue: %d\n", c);
    }
    printf("queue is: ");
    print(&q);
  }

  return 0;
}
