// We are given an array asteroids of integers representing asteroids in a row.
// The indices of the asteriod in the array represent their relative position in
// space.
//
// For each asteroid, the absolute value represents its size, and the sign
// represents its direction (positive meaning right, negative meaning left).
// Each asteroid moves at the same speed.
//
// Find out the state of the asteroids after all collisions. If two asteroids
// meet, the smaller one will explode. If both are the same size, both will
// explode. Two asteroids moving in the same direction will never meet.

#include <stdio.h>
#include <stdlib.h>

struct stack {
  int *elems;
  int cap;
  int i;
};

void stack_init(struct stack *s, int cap) {
  s->elems = malloc(sizeof(int) * cap);
  s->i = 0;
  s->cap = cap;
}

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
void stack_free(struct stack *s) { free(s->elems); }
int stack_count(struct stack *s) { return s->i; }

int collide(int a, int b) {
  return (a > 0 && b < 0) || (a < 0 && b > 0) || (a == b);
}

int abs(int a) { return a > 0 ? a : -a; }

int *asteroidCollision(int *asteroids, int asteroidsSize, int *returnSize) {
  int elem;
  struct stack s;

  stack_init(&s, asteroidsSize);
  for (int i = 0; i < asteroidsSize; ++i) {
    int destroyed = 0;
    int top;
    int ast = asteroids[i];
    while (!destroyed && !stack_top(&s, &top) && top > 0 && ast < 0) {
      // Case 1: ast going left, top going right. Ast bigger than top -> ast
      // destroys bigger
      if (abs(ast) > top)
        stack_pop(&s, 0);
      // Case 2: ast going left, top going right. Ast and top are equal -> both
      // get destroyed.
      else if (abs(ast) == top) {
        stack_pop(&s, 0);
        destroyed = 1;
        continue;
        // Case 3: ast going left, top going right. Top is bigger than ast so it
        // gets destroyed
      } else {
        destroyed = 1;
        continue;
      }
    }

    if (!destroyed)
      stack_push(&s, ast);
  }

  *returnSize = stack_count(&s);
  int *res = malloc(sizeof(int) * (*returnSize));
  int count = *returnSize - 1;

  while (count >= 0 && stack_pop(&s, &elem) != -1) {
    res[count--] = elem;
  }

  stack_free(&s);
  return res;
}

int main(void) {
  int asteroids[] = {10, 2, -5};
  int n;
  int *res = asteroidCollision(asteroids,
                               sizeof(asteroids) / sizeof(asteroids[0]), &n);

  for (int i = 0; i < n; ++i)
    printf("%d ", res[i]);

  if (res)
    free(res);
  return 0;
}
