// Given the root of a binary tree, return its maximum depth.
//
// A binary tree's maximum depth is the number of nodes along the longest path
// from the root node down to the farthest leaf node.

#include <stdio.h>
#include <stdlib.h>

struct TreeNode {
  int val;
  struct TreeNode *left;
  struct TreeNode *right;
};

int dfs(struct TreeNode *root) {
  if (!root)
    return 0;

  int x = dfs(root->left) + 1;
  int y = dfs(root->right) + 1;

  return x > y ? x : y;
}
int maxDepth(struct TreeNode *root) { return dfs(root); }

void in_order_print(struct TreeNode *root) {
  if (!root)
    return;

  in_order_print(root->left);
  in_order_print(root->right);
  printf("%d ", root->val);
}

int main(void) {
  struct TreeNode *root = malloc(sizeof(struct TreeNode));
  root->val = 3;
  root->left = NULL;
  root->right = NULL;

  root->left = malloc(sizeof(struct TreeNode));
  root->left->val = 9;
  root->left->left = NULL;
  root->left->right = NULL;

  root->right = malloc(sizeof(struct TreeNode));
  root->right->val = 20;
  root->right->left = NULL;
  root->right->right = NULL;

  root->right->left = malloc(sizeof(struct TreeNode));
  root->right->left->val = 15;
  root->right->left->left = NULL;
  root->right->left->right = NULL;

  // root->right->right = malloc(sizeof(struct TreeNode));
  // root->right->right->val = 7;
  // root->right->right->left = NULL;
  // root->right->right->right = NULL;

  in_order_print(root);

  printf("\ndepth: %d\n", maxDepth(root));

  // TODO: free the memory
  return 0;
}
