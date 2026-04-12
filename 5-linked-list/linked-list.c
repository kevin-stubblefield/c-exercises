#include <stdio.h>
#include <string.h>
#include "linked-list.h"

void sll_print(struct SinglyLinkedList sll, void (*print_fn)(void *))
{
    printf("[");

    struct SinglyLinkedListNode *p = sll.root;

    while (p != NULL)
    {
        print_fn(p->data);
        p = p->next;
        if (p != NULL)
        {
            printf(",");
        }
    }

    printf("]\n");
}

struct SinglyLinkedList sll_map(struct SinglyLinkedList sll, size_t size, int *err, void *(*map_fn)(void *))
{
    if (!sll.root)
    {
        return sll;
    }

    struct SinglyLinkedListNode *p = sll.root;
    struct SinglyLinkedList new_list;
    sll_new(&new_list);

    struct SinglyLinkedListNode *np = make_node(map_fn(p->data), size, err);
    new_list.root = np;
    while (p->next)
    {
        np->next = make_node(map_fn(p->next->data), size, err);
        p = p->next;
        np = np->next;
    }

    return new_list;
}

struct SinglyLinkedList sll_filter(struct SinglyLinkedList sll, size_t size, int *err, int (*filter_fn)(void *))
{
    if (!sll.root)
    {
        return sll;
    }

    struct SinglyLinkedListNode *p = sll.root;
    struct SinglyLinkedList new_list;
    sll_new(&new_list);

    while (p)
    {
        if (filter_fn(p->data))
        {
            sll_append(&new_list, p->data, size, err);
        }
        p = p->next;
    }

    return new_list;
}

void sll_new(struct SinglyLinkedList *sll)
{
    sll->root = NULL;
}

void sll_append(struct SinglyLinkedList *sll, void *value, size_t size, int *err)
{
    if (sll->root == NULL)
    {
        sll->root = make_node(value, size, err);
    }
    else
    {
        struct SinglyLinkedListNode *p = sll->root;

        while (p->next != NULL)
        {
            p = p->next;
        }

        p->next = make_node(value, size, err);
    }
}

struct SinglyLinkedListNode *make_node(void *value, size_t size, int *err)
{
    void *new_value = malloc(size);
    new_value = memcpy(new_value, value, size);

    struct SinglyLinkedListNode *node;
    if ((node = (struct SinglyLinkedListNode *)malloc(sizeof(struct SinglyLinkedListNode))) == NULL)
    {
        *err = 1;
        free(new_value);
        new_value = NULL;
        return NULL;
    }
    node->data = new_value;
    node->next = NULL;
    return node;
}