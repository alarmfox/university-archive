// You are given the root of a binary search tree (BST) and an integer val.
//
// Find the node in the BST that the node's value equals val and return the
// subtree rooted with that node. If such a node does not exist, return null.
//
#include <stdio.h>

#include <stdlib.h>

struct TreeNode {
  int val;
  struct TreeNode *left;
  struct TreeNode *right;
};
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     struct TreeNode *left;
 *     struct TreeNode *right;
 * };
 */
struct TreeNode *searchBST(struct TreeNode *root, int val) {
  if (!root || root->val == val)
    return root;
  if (val < root->val)
    return searchBST(root->left, val);
  return searchBST(root->right, val);
}

void in_order_print(struct TreeNode *root) {
  if (!root)
    return;

  in_order_print(root->left);
  printf("%d ", root->val);
  in_order_print(root->right);
}

int main(void) {
  struct TreeNode *root = malloc(sizeof(struct TreeNode));
  root->val = 4;
  root->left = NULL;
  root->right = NULL;

  root->left = malloc(sizeof(struct TreeNode));
  root->left->val = 2;
  root->left->left = NULL;
  root->left->right = NULL;

  root->right = malloc(sizeof(struct TreeNode));
  root->right->val = 7;
  root->right->left = NULL;
  root->right->right = NULL;

  root->left->left = malloc(sizeof(struct TreeNode));
  root->left->left->val = 1;
  root->left->left->left = NULL;
  root->left->left->right = NULL;

  root->left->right = malloc(sizeof(struct TreeNode));
  root->left->right->val = 3;
  root->left->right->left = NULL;
  root->left->right->right = NULL;

  in_order_print(root);
  printf("\n");

  struct TreeNode *t = searchBST(root, 2);
  in_order_print(t);
  printf("\n");

  return 0;
}
