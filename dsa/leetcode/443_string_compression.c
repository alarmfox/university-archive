// Given an array of characters chars, compress it using the following
// algorithm:
//
// Begin with an empty string s. For each group of consecutive repeating
// characters in chars:
//
//     If the group's length is 1, append the character to s.
//     Otherwise, append the character followed by the group's length.
//
// The compressed string s should not be returned separately, but instead, be
// stored in the input character array chars. Note that group lengths that are
// 10 or longer will be split into multiple characters in chars.
//
// After you are done modifying the input array, return the new length of the
// array.
//
// You must write an algorithm that uses only constant extra space.
//
// Note: The characters in the array beyond the returned length do not matter
// and should be ignored.

#include <stdio.h>
#include <string.h>

// Puts l as char* in single digits. Returns the number of digits
int serialize_length(char *s, int l) {
  int i = 0;
  int n = 0;
  int tmp = l;
  // count first how many digits
  while (tmp != 0) {
    tmp /= 10;
    n++;
  }

  for (int i = n - 1; i >= 0; i--) {
    s[i] = '0' + (l % 10);
    l /= 10;
  }
  return n;
}

int compress(char *chars, int charsSize) {
  int start = 0;
  int i = 0;
  while (start < (charsSize)) {
    // count the number of occurrences of the character
    while (i < charsSize && chars[start] == chars[i])
      i++;
    if ((i - start) > 1) {
      int digits = serialize_length(&chars[start + 1], i - start);
      int nshift = i - digits - 1 - start;
      int pos = i;

      // shift left i - digits times
      for (int k = 0; k < nshift; ++k) {
        for (int j = pos; j < charsSize; ++j) {
          chars[j - 1] = chars[j];
        }
        charsSize--;
        pos--;
      }
      start = i - nshift;
      i -= nshift;
    } else {
      start = i;
    }
  }

  return charsSize;
}

int main(void) {
  // char chars[] = {'a', 'a', 'b', 'b', 'c', 'c', 'c', '\0'};
  // char chars[] = {'a', 'b', 'b', 'b', 'b', 'b', 'b',
  //                 'b', 'b', 'b', 'b', 'b', 'b', 0};

  // char chars[] = {'a', 0};
  // char chars[] = {'a', 'a', 'a', 'b', 'b', 'a', 'a', 0};
  char chars[] = {'a', 'a', 'a', 'a', 'a', 'b', 0};
  // char chars[] = {'a', 'b', 'c', 0};
  printf("%s\n", chars);

  int size = compress(chars, strlen(chars));
  chars[size] = 0;

  printf("%s\n", chars);
}
