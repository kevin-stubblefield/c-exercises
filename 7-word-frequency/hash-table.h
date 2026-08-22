#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct ht ht;

ht *ht_create(void);

void ht_destroy(ht *table);

void *ht_get(ht *table, const char *key);

const char *ht_set(ht *table, const char *key, void *value);

size_t ht_length(ht *table);

typedef struct
{
    const char *key;
    void *value;

    // don't use these fields directly
    ht *_table;
    size_t _index;
} hti;

hti ht_iterator(ht *table);

bool ht_next(hti *it);

#endif