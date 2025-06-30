#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

#define INITIAL_SIZE 16

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME  1099511628211UL

/* Note: Above is the 64-bit variant. Below is the 32-bit variant. Leave it here
 for later.
 #define FNV_OFFSET 2166136261 
 #define FNV_PRIME  16777619 
*/

typedef struct
{
    const char* key;
    void* value;
} ht_entry;

struct ht
{
    ht_entry* entries;
    size_t capacity;
    size_t length;
};

ht* ht_create()
{
    ht* table = malloc(sizeof(ht));
    if (table == NULL)
    {
        return NULL;
    }
    table->length = 0;
    table->capacity = INITIAL_SIZE;

    // Calloc to zero out entries to make sure they are all NULLs
    table->entries = calloc(table->capacity, sizeof(ht_entry));
    if (table->entries == NULL)
    {
        free(table);
        return NULL;
    }

    return table;
}

void ht_destroy(ht* table)
{
    for (size_t i = 0; i < table->capacity; i++)
    {
        free((void*) table->entries[i].key);
    }

    free(table->entries);
    free(table);
}

static uint64_t hash_key(const char* key)
{
    uint64_t hash = FNV_OFFSET;
    for (const char* p = key; *p; p++)
    {
        hash ^= (uint64_t) (unsigned char) (*p);
        hash *= FNV_PRIME;
    }
    return hash;
}

void* ht_get(ht* table, const char* key)
{
    uint64_t hash = hash_key(key);

    size_t i = (size_t) (hash & (uint64_t) (table->capacity - 1));
    while (table->entries[i].key != NULL)
    {
        if (strcmp(key, table->entries[i].key) == 0)
        {
            return table->entries[i].value;
        }

        i++;
        if (i >= table->capacity)
        {
            return 0;
        }
    }

    return NULL;
}

static const char* set_entry(ht_entry* entries, size_t capacity,
    const char* key, void* value, size_t* p_length)
{
    uint64_t hash = hash_key(key);
    size_t i = (size_t) (hash &  (uint64_t) (capacity - 1));

    while (entries[i].key != NULL)
    {
        if (strcmp(key, entries[i].key) == 0)
        {
            entries[i].value = value;
            return entries[i].key;
        }

        i++;
        if (i >= capacity)
        {
            i = 0;
        }
    }

    if (p_length != NULL)
    {
        key = strdup(key);
        if (key == NULL)
        {
            return NULL;
        }

        (*p_length)++;
    }

    entries[i].key = (char*) key;
    entries[i].value = value;

    return key;
}

static int ht_expand(ht* table)
{
    size_t new_capacity = table->capacity * 2;
    if (new_capacity < table->capacity)
    {
        return 0;
    }

    ht_entry* new_entries = calloc(new_capacity, sizeof(ht_entry));
    if (new_entries == NULL)
    {
        return 0;
    }

    for (size_t i = 0; i < table->capacity; i++)
    {
        ht_entry entry = table->entries[i];
        if (entry.key != NULL)
        {
            set_entry(new_entries, new_capacity, entry.key, entry.value, NULL);
        }
    }

    free(table->entries);
    table->entries = new_entries;
    table->capacity = new_capacity;

    return 1;
}

const char* ht_set(ht* table, const char* key, void* value)
{
    if (value == NULL)
    {
        return NULL;
    }

    if (table->length >= table->capacity / 2)
    {
        if (!ht_expand(table))
        {
            return NULL;
        }
    }

    return set_entry(table->entries, table->capacity, key, value, &table->length);
}

size_t ht_length(ht *table)
{
    return table->length;
}

hti ht_iterator(ht *table)
{
    hti it;
    it._table = table;
    it._index = 0;

    return it;
}

int ht_next(hti* it)
{
    ht* table = it->_table;
    while (it->_index < table->capacity)
    {
        size_t i = it->_index;
        it->_index++;

        if (table->entries[i].key != NULL)
        {
            ht_entry entry = table->entries[i];
            it->key = entry.key;
            it->value = entry.value;

            return 1;
        }
    }

    return 0;
}