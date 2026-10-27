// Given two 0-indexed integer arrays nums1 and nums2, return a list answer of
// size 2 where:
//
//     answer[0] is a list of all distinct integers in nums1 which are not
//     present in nums2. answer[1] is a list of all distinct integers in nums2
//     which are not present in nums1.
//
// Note that the integers in the lists may be returned in any order.

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// from problem data
#define HT_MAX_SIZE 2048

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

// Return 64-bit FNV-1a hash for key (NUL-terminated). See description:
// https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
static uint64_t hash_key(const char *key, const int len) {
  uint64_t hash = FNV_OFFSET;
  for (int i = 0; i < len; ++i) {
    hash ^= (uint64_t)(unsigned char)(key[i]);
    hash *= FNV_PRIME;
  }
  return hash;
}

struct ht_entry {
  int key;
  struct ht_entry *next;
};

// Set like structure
struct ht_map {
  // an array of slots -> indexed by the hash function
  // int* because we are handling collisions with a linked list
  struct ht_entry *buckets[HT_MAX_SIZE];
};

void ht_init(struct ht_map **hm) {
  struct ht_map *h = malloc(sizeof(struct ht_map));

  memset(h, 0, sizeof(struct ht_map));

  *hm = h;
}

int ht_exists(struct ht_map *hm, int key) {
  char bkey[] = {(key >> 24) & 0xFF, (key >> 16) & 0xFF, (key >> 8) & 0xFF,
                 key & 0xFF};
  uint64_t index = hash_key(bkey, 4) % HT_MAX_SIZE;

  struct ht_entry *entry = hm->buckets[index];

  while (entry) {
    // found
    if (entry->key == key) {
      return 0;
    }

    // scan the linked list bucket for collision
    entry = entry->next;
  }

  return -1;
}

void ht_put(struct ht_map *hm, int key) {
  char bkey[] = {(key >> 24) & 0xFF, (key >> 16) & 0xFF, (key >> 8) & 0xFF,
                 key & 0xFF};
  uint64_t index = hash_key(bkey, 4) % HT_MAX_SIZE;

  struct ht_entry *entry = hm->buckets[index], *prev = 0;

  // navigate to the end of the list
  while (entry) {
    // element already present no need to do anything
    if (entry->key == key) {
      return;
    }
    prev = entry;
    entry = entry->next;
  }

  // append at the beginning
  struct ht_entry *new = malloc(sizeof(struct ht_entry));
  new->key = key;
  new->next = hm->buckets[index];
  // link the entry to the last first of the list
  hm->buckets[index] = new;
}

void ht_free(struct ht_map *hm) {
  if (!hm)
    return;

  // clear the linked list
  for (int i = 0; i < HT_MAX_SIZE; ++i) {
    struct ht_entry *entry = hm->buckets[i], *p;
    while (entry) {
      p = entry;
      entry = entry->next;
      free(p);
    }
  }

  free(hm);
}

/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume
 * caller calls free().
 */
int **findDifference(int *nums1, int nums1Size, int *nums2, int nums2Size,
                     int *returnSize, int **returnColumnSizes) {

  struct ht_map *h = NULL, *seen = NULL;
  *returnSize = 2;
  int **res = malloc(sizeof(int *) * 2);
  *returnColumnSizes = malloc(sizeof(int) * 2);
  int k = 0;

  // Set difference is limited by the initial set size. Ie. sizeof(A-B) <=
  // sizeof(A)
  res[0] = malloc(sizeof(int) * nums1Size);
  res[1] = malloc(sizeof(int) * nums2Size);

  ht_init(&h);
  ht_init(&seen);

  // load nums2 in the set
  for (int i = 0; i < nums2Size; ++i) {
    ht_put(h, nums2[i]);
  }

  // check if nums1 are in the set, if not push into the array
  for (int i = 0; i < nums1Size; ++i) {
    if (ht_exists(h, nums1[i]) < 0 && ht_exists(seen, nums1[i]) < 0) {
      res[0][k++] = nums1[i];
      ht_put(seen, nums1[i]);
    }
  }
  (*returnColumnSizes)[0] = k;

  ht_free(h);
  ht_free(seen);
  h = NULL;
  seen = NULL;
  ht_init(&h);
  ht_init(&seen);

  // load nums1 in the set
  for (int i = 0; i < nums1Size; ++i) {
    ht_put(h, nums1[i]);
  }

  k = 0;
  // check if nums1 are in the set, if not push into the array
  for (int i = 0; i < nums2Size; ++i) {
    if (ht_exists(h, nums2[i]) < 0 && ht_exists(seen, nums2[i]) < 0) {
      res[1][k++] = nums2[i];
      ht_put(seen, nums2[i]);
    }
  }
  (*returnColumnSizes)[1] = k;

  return res;
}

int main(void) {
  int nums1[] = {1, 2, 3};
  int nums2[] = {2, 4, 6};

  int nums1Size = sizeof(nums1) / sizeof(nums1[0]);
  int nums2Size = sizeof(nums2) / sizeof(nums2[0]);

  int returnSize;
  int *returnColumnSizes = NULL;

  int **res = findDifference(nums1, nums1Size, nums2, nums2Size, &returnSize,
                             &returnColumnSizes);

  int *a = res[0];
  int asize = returnColumnSizes[0];

  for (int i = 0; i < asize; ++i) {
    printf("%d ", a[i]);
  }
  printf("\n");

  a = res[1];
  asize = returnColumnSizes[1];
  for (int i = 0; i < asize; ++i) {
    printf("%d ", a[i]);
  }
  printf("\n");
  return 0;
}
