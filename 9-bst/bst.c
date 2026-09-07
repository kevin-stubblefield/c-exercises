#include "bst.h"
#include <stdio.h>
#include <string.h>

static void destroy_node(struct TreeNode **node, struct Tree *tree);

struct Tree *bst_new(size_t element_size,
                     int (*compare_fn)(const void *, const void *),
                     void (*free_element)(void *)) {
  struct Tree *tree = malloc(sizeof(struct Tree));
  if (tree == NULL) {
    return NULL;
  }

  tree->element_size = element_size;
  tree->root = NULL;
  tree->compare_fn = compare_fn;
  tree->free_element = free_element;

  return tree;
}

static struct TreeNode *make_node(struct Tree *tree, const void *value) {
  struct TreeNode *node = malloc(sizeof(struct TreeNode) + tree->element_size);
  if (node == NULL) {
    return NULL;
  }

  node->count = 1;
  node->left = node->right = NULL;
  memcpy(node->data, value, tree->element_size);

  return node;
}

static TreeError insert_node(struct TreeNode **node, struct Tree *tree,
                             const void *value) {
  if (*node == NULL) {
    *node = make_node(tree, value);
    if (*node == NULL) {
      return TREE_ERR_NOMEM;
    } else {
      return TREE_OK;
    }
  }

  int cmp = tree->compare_fn(value, (*node)->data);
  if (cmp < 0) {
    return insert_node(&(*node)->left, tree, value);
  } else if (cmp > 0) {
    return insert_node(&(*node)->right, tree, value);
  } else {
    (*node)->count++;
    return TREE_OK;
  }
}

TreeError bst_insert(struct Tree *tree, const void *value) {
  return insert_node(&tree->root, tree, value);
}

static const struct TreeNode *find_node(const struct TreeNode *node,
                                        const struct Tree *tree,
                                        const void *value) {
  if (node == NULL) {
    return NULL;
  }

  int cmp = tree->compare_fn(value, node->data);

  if (cmp < 0) {
    return find_node(node->left, tree, value);
  } else if (cmp > 0) {
    return find_node(node->right, tree, value);
  } else {
    return node;
  }
}

const struct TreeNode *bst_search(const struct Tree *tree, const void *value) {
  return find_node(tree->root, tree, value);
}

static void traverse_helper(const struct TreeNode *node,
                            void (*callback)(const struct TreeNode *),
                            TraverseOrder order) {
  if (node == NULL) {
    return;
  }

  if (order == PREORDER)
    callback(node);
  traverse_helper(node->left, callback, order);
  if (order == INORDER)
    callback(node);
  traverse_helper(node->right, callback, order);
  if (order == POSTORDER)
    callback(node);
}

void bst_traverse(const struct Tree *tree,
                  void (*callback)(const struct TreeNode *),
                  TraverseOrder order) {
  traverse_helper(tree->root, callback, order);
}

static void recursive_free(struct Tree *tree, struct TreeNode *node) {
  if (node == NULL) {
    return;
  }

  recursive_free(tree, node->left);
  recursive_free(tree, node->right);
  if (tree->free_element) {
    tree->free_element(node->data);
  }
  free(node);
  node = NULL;
}

void bst_free(struct Tree *tree) {
  // traverse tree and free each node. gonna have to be recursive traversal
  // chief so that we free in reverse order, otherwise links will be broken
  // specifically it has to be post-order traversal because then the root will
  // be free'd last
  recursive_free(tree, tree->root);
  free(tree);
  tree = NULL;
}

static void destroy_node(struct TreeNode **node, struct Tree *tree) {
  if (*node == NULL) {
    return;
  }

  if (tree->free_element) {
    tree->free_element((*node)->data);
  }
  free(*node);
  *node = NULL;
}

static TreeError recursive_delete(struct TreeNode **node, struct Tree *tree,
                                  const void *value) {
  if (*node == NULL) {
    return TREE_OK;
  }

  int cmp = tree->compare_fn(value, (*node)->data);

  if (cmp < 0) {
    return recursive_delete(&(*node)->left, tree, value);
  }
  if (cmp > 0) {
    return recursive_delete(&(*node)->right, tree, value);
  }

  if (--((*node)->count) > 0) {
    return TREE_OK;
  }
  struct TreeNode *curr = *node;
  if (!curr->left) {
    curr = curr->right;
    destroy_node(node, tree);
    *node = curr;
    return TREE_OK;
  }
  if (!curr->right) {
    curr = curr->left;
    destroy_node(node, tree);
    *node = curr;
    return TREE_OK;
  }

  struct TreeNode **predecessor_link = &(*node)->left;
  while ((*predecessor_link)->right) {
    predecessor_link = &(*predecessor_link)->right;
  }
  curr = *predecessor_link;

  if (tree->free_element) {
    tree->free_element((*node)->data);
  }
  memcpy((*node)->data, curr->data, tree->element_size);
  (*node)->count = curr->count;
  *predecessor_link = curr->left;
  free(curr);

  return TREE_OK;
}

TreeError bst_delete(struct Tree *tree, const void *value) {
  return recursive_delete(&tree->root, tree, value);
}