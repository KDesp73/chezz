#ifndef HASHING_H
#define HASHING_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint64_t hash;
    int count;
} hash_entry_t;

typedef struct {
    hash_entry_t* entries;
    size_t size;
    size_t capacity;
} hash_table_t;

_Bool update_hash_table(hash_table_t* table, uint64_t hash);
void free_hash_table(hash_table_t* table);
void init_hash_table(hash_table_t* table, size_t capacity);


#endif // HASHING_H
