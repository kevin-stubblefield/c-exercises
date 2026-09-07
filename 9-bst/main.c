#include "bst.h"
#include <stdio.h>
#include <string.h>

int compare_ints(const void *a, const void *b) {
  int x = *(const int *)a, y = *(const int *)b;
  return (x > y) - (x < y);
}

void print_ints(const struct TreeNode *value) {
  printf("value=%d, count=%zu\n", *(int *)value->data, value->count);
}

int compare_strings(const void *a, const void *b) {
  return strcmp(*(char **)a, *(char **)b);
}

void print_strings(const struct TreeNode *value) {
  printf("value=%s, count=%zu\n", *(char **)value->data, value->count);
}

void free_string(void *s) { free(*(char **)s); }

int main(void) {
  struct Tree *tree = bst_new(sizeof(char *), compare_strings, free_string);
  char *s;

  s = strdup("30");
  bst_insert(tree, &s);
  s = strdup("20");
  bst_insert(tree, &s);
  s = strdup("40");
  bst_insert(tree, &s);

  char *target = "30";
  bst_delete(tree, &target);

  bst_traverse(tree, print_strings, PREORDER);
  bst_free(tree);
}