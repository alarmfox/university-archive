#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Given a string s, reverse only all the vowels in the string and return it.
//
// The vowels are 'a', 'e', 'i', 'o', and 'u', and they can appear in both lower
// and upper cases, more than once.

struct stack {
  char *v;
  int i;
  int sz;
};

void stack_init(struct stack **s, const int size) {
  *s = (struct stack *)malloc(sizeof(struct stack));
  (*s)->i = 0;
  (*s)->v = (char *)malloc(size);
  (*s)->sz = size;

  for (int i = 0; i < size; ++i) {
    (*s)->v[i] = 0;
  }
}

void stack_push(struct stack *s, char elem) { s->v[s->i++] = elem; }

char stack_pop(struct stack *s) {
  char e = s->v[--(s->i)];

  return e;
}

void stack_destroy(struct stack **s) {
  free((*s)->v);
  (*s)->v = NULL;
  free(*s);
  *s = NULL;
}

char *reverseVowels(char *s) {
  struct stack *stack = NULL;
  int l = strlen(s);
  char *out = (char *)malloc(sizeof(char) * (l + 1));

  stack_init(&stack, l);

  for (int i = 0; i < l; ++i) {
    // clang-format off
    if (s[i] == 'A' || s[i] == 'a'
        || s[i] == 'E' || s[i] == 'e'
        || s[i] == 'I' || s[i] == 'i'
        || s[i] == 'O' || s[i] == 'o'
        || s[i] == 'U' || s[i] == 'u'
        )
      stack_push(stack, s[i]);
  }

  for (int i = 0; i < l; ++i) {
    if (s[i] == 'A' || s[i] == 'a'
        || s[i] == 'E' || s[i] == 'e'
        || s[i] == 'I' || s[i] == 'i'
        || s[i] == 'O' || s[i] == 'o'
        || s[i] == 'U' || s[i] == 'u'
        ) {
      char c = stack_pop(stack);
      out[i] = c;
    } else {
      out[i] = s[i];
    }
  }

  out[l] = '\0';

  stack_destroy(&stack);

  return out;
}

int main(void) {
  char *s = "leetcode";
  char *r = reverseVowels(s);

  printf("%s", r);
  free(r);
  return 0;
}
