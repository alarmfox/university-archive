// Consider all the leaves of a binary tree, from left to right order, the
// values of those leaves form a leaf value sequence. For example, in the given
// tree above, the leaf value sequence is (6, 7, 4, 9, 8).
//
// Two binary trees are considered leaf-similar if their leaf value sequence is
// the same.
//
// Return true if and only if the two given trees with head nodes root1 and
// root2 are leaf-similar.

#include <stdbool.h>
struct TreeNode {
  int val;
  struct TreeNode *left;
  struct TreeNode *right;
};

void dfs(struct TreeNode *root, int *arr, int *pos) {
  if (!root)
    return;
  dfs(root->left, arr, pos);
  // in-order
  if (root->left == 0 && root->right == 0)
    arr[(*pos)++] = root->val;
  dfs(root->right, arr, pos);
}

bool leafSimilar(struct TreeNode *root1, struct TreeNode *root2) {
  int leaf1[200], leaf2[200], n1 = 0, n2 = 0;
  dfs(root1, leaf1, &n1);
  dfs(root2, leaf2, &n2);

  if (n1 != n2)
    return false;
  for (int i = 0; i < n1; ++i) {
    if (leaf1[i] != leaf2[i])
      return false;
  }

  return true;
}
