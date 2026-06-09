// You are given the head of a linked list. Delete the middle node, and return
// the head of the modified linked list.
//
// The middle node of a linked list of size n is the ⌊n / 2⌋th node from the
// start using 0-based indexing, where ⌊x⌋ denotes the largest integer less than
// or equal to x.
//
//     For n = 1, 2, 3, 4, and 5, the middle nodes are 0, 1, 1, 2, and 2,
//     respectively.

#include <stdlib.h>

struct ListNode {
  int val;
  struct ListNode *next;
};

struct ListNode *deleteMiddle(struct ListNode *head) {
  int n = 0;
  struct ListNode *p = head;

  while (p) {
    n++;
    p = p->next;
  }

  int mid = n / 2;

  p = head;
  for (int i = 0; i < mid - 1; ++i)
    p = p->next;

  struct ListNode *middle = p->next;
  if (p->next) {
    p->next = p->next->next;
    free(middle);

  } else {
    free(middle);
    return 0;
  }

  return head;
}
