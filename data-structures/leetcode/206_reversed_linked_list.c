// Given the head of a singly linked list, reverse the list, and return the
// reversed list.

#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 5000

struct stack {
  int elems[MAX_SIZE];
  int i;
};

void stack_init(struct stack *s) { s->i = 0; }

void stack_push(struct stack *s, int elem) { s->elems[s->i++] = elem; }

int stack_top(struct stack *s, int *elem) {
  if (s->i == 0)
    return -1;
  *elem = s->elems[s->i - 1];
  return 0;
}

int stack_pop(struct stack *s, int *elem) {
  if (s->i == 0)
    return -1;
  if (elem)
    *elem = s->elems[s->i - 1];

  s->i--;
  return 0;
}

struct ListNode {
  int val;
  struct ListNode *next;
};

struct ListNode *reverseList(struct ListNode *head) {
  struct ListNode *tmp = head;
  struct ListNode *new = NULL, *res = NULL;
  struct stack s;
  stack_init(&s);

  while (tmp) {
    stack_push(&s, tmp->val);
    tmp = tmp->next;
  }

  int elem;
  while (stack_pop(&s, &elem) != -1) {
    tmp = malloc(sizeof(struct ListNode));
    tmp->val = elem;
    tmp->next = NULL;
    if (new == NULL) {
      new = tmp;
      res = new;
    } else {
      new->next = tmp;
      new = tmp;
    }
  }

  return res;
}

struct ListNode *reverseListNew(struct ListNode *head) {
  struct ListNode *curr = 0, *next = 0, *prev = 0;

  curr = head;
  while (curr) {
    next = curr->next;

    curr->next = prev;

    prev = curr;
    curr = next;
  }
  return prev;
}

int main(void) {
  struct ListNode root;
  root.val = 1;

  root.next = malloc(sizeof(struct ListNode));
  root.next->val = 2;

  root.next->next = malloc(sizeof(struct ListNode));
  root.next->next->val = 3;

  root.next->next->next = malloc(sizeof(struct ListNode));
  root.next->next->next->val = 4;

  root.next->next->next->next = malloc(sizeof(struct ListNode));
  root.next->next->next->next->val = 5;

  struct ListNode *tmp = &root;
  while (tmp) {
    printf("%d->", tmp->val);
    tmp = tmp->next;
  }
  printf("END\n");

  struct ListNode *new = reverseListNew(&root);

  tmp = new;
  while (tmp) {
    printf("%d->", tmp->val);
    tmp = tmp->next;
  }
  printf("END\n");

  // delete the lists
  tmp = root.next;
  while (tmp) {
    struct ListNode *q = tmp->next;
    free(tmp);
    tmp = q;
  }

  tmp = new;
  while (tmp) {
    struct ListNode *q = tmp->next;
    free(tmp);
    tmp = q;
  }
}
