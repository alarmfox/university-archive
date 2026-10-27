#include <stdio.h>
#include <stdlib.h>
// Given the head of a sorted linked list, delete all duplicates such that each
// element appears only once. Return the linked list sorted as well.

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
struct ListNode {
  int val;
  struct ListNode *next;
};

// 1 -> 1 -> 2
// 1. The list is sorted, get the first element where key=K, call it K*
// 2. Skip all the elements with key=K, till we find N
// 3. K*.next = N

struct ListNode *deleteDuplicates(struct ListNode *head) {
  struct ListNode *t = 0, *curr = head, *q = 0;

  while (curr) {
    t = curr;
    while (t && t->val == curr->val) {
      // delete the nodes?
      t = t->next;
    }

    curr->next = t;

    curr = curr->next;
  }

  return head;
}

int main(void) {
  struct ListNode *root = malloc(sizeof(struct ListNode)), *p = 0;
  root->val = 1;

  p = malloc(sizeof(struct ListNode));
  p->val = 1;
  root->next = p;

  p = malloc(sizeof(struct ListNode));
  p->val = 2;
  root->next->next = p;

  p = malloc(sizeof(struct ListNode));
  p->val = 3;
  root->next->next->next = p;

  p = malloc(sizeof(struct ListNode));
  p->val = 3;
  root->next->next->next->next = p;

  p = root;

  while (p) {
    printf("%d ->", p->val);
    p = p->next;
  }
  printf("\n");

  p = deleteDuplicates(root);

  while (p) {
    printf("%d ->", p->val);
    p = p->next;
  }
  printf("\n");
  return 0;
}
