#include "list.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

struct List *list_new(size_t size, void (*free_element)(void *)) {
  size_t initial_capacity = 4;

  struct List *list;
  if ((list = malloc(sizeof(struct List))) == NULL) {
    return NULL;
  }
  list->capacity = initial_capacity;
  list->length = 0;
  list->element_size = size;
  list->free_element = free_element;

  if ((list->data = malloc(size * initial_capacity)) == NULL) {
    free(list); // must free if allocating array fails, otherwise leaks memory
    return NULL;
  }

  return list;
}

void list_free(struct List *list) {
  if (list == NULL) {
    return;
  }

  if (list->free_element != NULL) {
    for (size_t i = 0; i < list->length; i++) {
      void *element_to_free = list_get(list, i);
      if (element_to_free != NULL) {
        list->free_element(element_to_free);
      }
    }
  }

  free(list->data);
  free(list);
}

ListError list_add(struct List *list, const void *value) {
  if (list == NULL || value == NULL) {
    return LIST_ERR_BADPTR;
  }

  if (list->length == list->capacity) {
    size_t new_capacity = list->capacity * 2;
    void *temp;
    if ((temp = realloc(list->data, list->element_size * new_capacity)) ==
        NULL) {
      return LIST_ERR_NOMEM;
    }
    list->data = temp;
    list->capacity = new_capacity;
  }

  // we do this because of the way void * works in C. casting list->data to char
  // * tells the compiler to work in 1-byte steps, then we can offset by the
  // index (list->length in this case since we're appending) multiplied by the
  // size of each element stored in the list
  memcpy((char *)list->data + list->length * list->element_size, value,
         list->element_size);
  list->length++;
  return LIST_OK;
}

void *list_get(const struct List *list, size_t index) {
  assert(list != NULL);
  assert(index < list->length);

  return (char *)list->data + index * list->element_size;
}

ListError list_remove(struct List *list, size_t index) {
  if (list == NULL) {
    return LIST_ERR_BADPTR;
  }

  if (index >= list->length) {
    return LIST_ERR_OOB;
  }

  if (list->free_element != NULL) {
    list->free_element(list_get(list, index));
  }

  size_t elements_to_move = list->length - index - 1;
  if (elements_to_move > 0) {
    memmove(list_get(list, index), list_get(list, index + 1),
            list->element_size * elements_to_move);
  }

  list->length--;
  return LIST_OK;
}

void list_print(const struct List *list, void (*print_fn)(void *)) {
  assert(list != NULL && print_fn != NULL);

  printf("[");
  for (size_t i = 0; i < list->length; i++) {
    print_fn(list_get(list, i));
    if (i < list->length - 1)
      printf(",");
  }
  printf("]");
}

void list_raw_print(const struct List *list, void (*print_fn)(void *)) {
  assert(list != NULL && print_fn != NULL);

  for (size_t i = 0; i < list->length; i++) {
    print_fn(list_get(list, i));
  }
}