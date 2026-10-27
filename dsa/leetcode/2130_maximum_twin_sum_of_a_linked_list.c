// In a linked list of size n, where n is even, the ith node (0-indexed) of the
// linked list is known as the twin of the (n-1-i)th node, if 0 <= i <= (n / 2)
// - 1.
//
//     For example, if n = 4, then node 0 is the twin of node 3, and node 1 is
//     the twin of node 2. These are the only nodes with twins for n = 4.
//
// The twin sum is defined as the sum of a node and its twin.
//
// Given the head of a linked list with even length, return the maximum twin sum
// of the linked list.

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct ListNode {
  int val;
  struct ListNode *next;
};

struct ListNode *reverseList(struct ListNode *head) {
  struct ListNode *curr = head, *next = NULL, *prev = NULL;

  while (curr) {
    next = curr->next;

    curr->next = prev;

    prev = curr;
    curr = next;
  }
  return prev;
}

int pairSum(struct ListNode *head) {
  int len = 0;
  struct ListNode *p = head, *q = NULL;

  while (p) {
    len++;
    p = p->next;
  }

  assert(0 == (len % 2));

  p = head;
  for (int i = 0; i < len / 2; ++i) {
    p = p->next;
  }
  struct ListNode *reversed = reverseList(p);

  q = reversed;
  while (q) {
    printf("%d->", q->val);
    q = q->next;
  }
  printf("END\n");

  q = reversed;
  p = head;
  int max = 0;
  for (int i = 0; i < len / 2; ++i) {

    if ((p->val + q->val) > max) {
      max = p->val + q->val;
    }

    p = p->next;
    q = q->next;
  }

  return max;
}

int main(void) {
  struct ListNode *root = malloc(sizeof(*root));
  struct ListNode *tmp = NULL;

  root->val = 5;
  root->next = malloc(sizeof(struct ListNode));

  root->next->val = 4;
  root->next->next = malloc(sizeof(struct ListNode));

  root->next->next->val = 2;
  root->next->next->next = malloc(sizeof(struct ListNode));

  root->next->next->next->val = 1;
  root->next->next->next->next = NULL;

  tmp = root;
  while (tmp) {
    printf("%d->", tmp->val);
    tmp = tmp->next;
  }
  printf("END\n");

  int res = pairSum(root);

  printf("max pair sum: %d\n", res);

  tmp = root;
  while (tmp) {
    struct ListNode *q = tmp->next;
    free(tmp);
    tmp = q;
  }

  return 0;
}
