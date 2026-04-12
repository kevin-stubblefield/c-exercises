#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>

struct SinglyLinkedListNode
{
    void *data;
    struct SinglyLinkedListNode *next;
};

struct SinglyLinkedList
{
    struct SinglyLinkedListNode *root;
};

void sll_print(struct SinglyLinkedList sll, void (*print_fn)(void *));

struct SinglyLinkedList sll_map(struct SinglyLinkedList sll, size_t size, int *err, void *(*map_fn)(void *));

struct SinglyLinkedList sll_filter(struct SinglyLinkedList sll, size_t size, int *err, int (*filter_fn)(void *));

void sll_new(struct SinglyLinkedList *sll);

void sll_append(struct SinglyLinkedList *sll, void *value, size_t size, int *err);

struct SinglyLinkedListNode *make_node(void *value, size_t size, int *err);

#endif
