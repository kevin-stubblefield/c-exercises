#include <stdlib.h>

typedef enum { INORDER = 0, PREORDER, POSTORDER } TraverseOrder;

typedef enum { TREE_OK = 0, TREE_ERR_BADPTR, TREE_ERR_NOMEM } TreeError;

struct TreeNode {
  size_t count;
  struct TreeNode *left, *right;
  unsigned char data[];
};

struct Tree {
  size_t element_size;
  struct TreeNode *root;
  int (*compare_fn)(const void *, const void *);
  void (*free_element)(void *);
};

struct Tree *bst_new(size_t element_size,
                     int (*compare_fn)(const void *, const void *),
                     void (*free_element)(void *));
TreeError bst_insert(struct Tree *tree, const void *value);
const struct TreeNode *bst_search(const struct Tree *tree, const void *value);
TreeError bst_delete(struct Tree *tree, const void *value);
void bst_traverse(const struct Tree *tree, void (*)(const struct TreeNode *),
                  TraverseOrder);
void bst_free(struct Tree *tree);