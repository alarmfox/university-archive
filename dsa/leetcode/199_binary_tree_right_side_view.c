#include <stdlib.h>

#define MAX_ELEMS 128

struct TreeNode {
  int val;
  struct TreeNode *left;
  struct TreeNode *right;
};

struct queue {
  struct TreeNode *elems[MAX_ELEMS];
  int head;
  int tail;
  int count;
};

void qinit(struct queue *q) {
  for (int i = 0; i < MAX_ELEMS; ++i)
    q->elems[i] = 0;
  q->head = q->tail = q->count = 0;
}

void qpush(struct queue *q, struct TreeNode *v) {
  q->count++;
  q->elems[q->tail] = v;
  q->tail = (q->tail + 1) % MAX_ELEMS;
}
struct TreeNode *qpop(struct queue *q) {
  q->count--;
  struct TreeNode *v = q->elems[q->head];
  q->head = (q->head + 1) % MAX_ELEMS;
  return v;
}
int *rightSideView(struct TreeNode *root, int *returnSize) {
  if (!root) {
    (*returnSize) = 0;
    return malloc(0);
  }
  struct queue q;
  int *res = malloc(sizeof(int) * MAX_ELEMS);
  int pos = 0;
  qinit(&q);

  qpush(&q, root);

  while (q.count != 0) {
    int level_size = q.count;
    for (int i = 0; i < level_size; ++i) {
      // do something
      struct TreeNode *node = qpop(&q);
      if (node->left)
        qpush(&q, node->left);
      if (node->right)
        qpush(&q, node->right);

      if (i == (level_size - 1))
        res[pos++] = node->val;
    }
  }

  *returnSize = pos;
  return res;
}
