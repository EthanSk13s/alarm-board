// Note: Implementation is based off: https://benhoyt.com/writings/hash-table-in-c/
#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include <stddef.h>

typedef struct ht ht;

ht* ht_create();
void* ht_get(ht* table, const char* key);
const char* ht_set(ht* table, const char* key, void* value);
size_t ht_length(ht* table);
void ht_destroy(ht* table);

typedef struct
{
    const char* key;
    void* value;
    ht* _table;
    size_t _index;
} hti;

hti ht_iterator(ht* table);
int ht_next(hti* it);

#endif