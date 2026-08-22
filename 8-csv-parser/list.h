#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

typedef enum {
  LIST_OK = 0,
  LIST_ERR_NOMEM,
  LIST_ERR_BADPTR,
  LIST_ERR_OOB
} ListError;

struct List {
  void *data;
  size_t length;
  size_t capacity;
  size_t element_size;
  void (*free_element)(void *);
};

struct List *list_new(size_t size, void (*free_element)(void *));
void list_free(struct List *list);
ListError list_add(struct List *list, const void *value);
void *list_get(const struct List *list, size_t index);
ListError list_remove(struct List *list, size_t index);
void list_print(const struct List *list, void (*print_fn)(void *));
void list_raw_print(const struct List *list, void (*print_fn)(void *));

#endif