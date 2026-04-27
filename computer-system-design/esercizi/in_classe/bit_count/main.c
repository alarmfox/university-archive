#include <stdio.h>

// conta numero di di bit a 1 in un byte, con un vettore di maschere
int count_in_byte(const unsigned char c, const unsigned char mask[],
                  const int n) {
  int count = 0;
  for (int i = 0; i < n; i++) {
    if (c & mask[i])
      count++;
  }

  return count;
}
// versione senza maschere, ma che utilizza lo shift
// int count_in_byte(unsigned char c) {
//   int count = 0, i = 0;
//   while(i < 8) {
//     if (c & 0x01)
//       count++;
//     c = c >> 1;
//     i++;
//   }
//
//   return count;
// }
int main() {
  const unsigned char word[2] = {0x00, 0x0f};
  const unsigned char mask[8] = {0x01, 0x02, 0x04, 0x08,
                                 0x10, 0x20, 0x40, 0x80};

  printf("%d\n",
         count_in_byte(word[0], mask, 8) + count_in_byte(word[1], mask, 8));
}
