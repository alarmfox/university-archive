#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// from problem data
#define HT_MAX_SIZE 10000

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

// Return 64-bit FNV-1a hash for key. See description:
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
  int value;
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

int ht_exists(struct ht_map *hm, int key, int *val) {
  char bkey[] = {(key >> 24) & 0xFF, (key >> 16) & 0xFF, (key >> 8) & 0xFF,
                 key & 0xFF};
  uint64_t index = hash_key(bkey, 4) % HT_MAX_SIZE;

  struct ht_entry *entry = hm->buckets[index];

  while (entry) {
    // found
    if (entry->key == key) {
      (*val) = entry->value;
      return 0;
    }

    // scan the linked list bucket for collision
    entry = entry->next;
  }

  return -1;
}

void ht_put(struct ht_map *hm, int key, int value) {
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
  new->value = value;
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
int *twoSum(int *nums, int numsSize, int target, int *returnSize) {
  int *res = (int *)malloc(2 * sizeof(int));
  *returnSize = 2;

  for (int i = 0; i < numsSize; ++i) {
    for (int j = i + 1; j < numsSize; ++j) {
      if (nums[i] + nums[j] == target) {
        res[0] = i;
        res[1] = j;
      }
    }
  }
  return res;
}

int abs(int a) { return a > 0 ? a : -a; }
int *twoSumMap(int *nums, int numsSize, int target, int *returnSize) {
  int *res = (int *)malloc(2 * sizeof(int));
  *returnSize = 2;
  struct ht_map *hm;
  ht_init(&hm);

  int b;
  for (int i = 0; i < numsSize; ++i) {
    if (ht_exists(hm, nums[i], &b) == 0) {
      res[0] = i;
      res[1] = b;
      break;
    }
    ht_put(hm, target - nums[i], i);
  }
  return res;
}

int main(void) {
  int nums[4] = {2, 7, 11, 5};
  int rsize = 0;
  int *res = twoSumMap(nums, 4, 9, &rsize);

  printf("[%d, %d]", res[0], res[1]);

  free(res);
}
