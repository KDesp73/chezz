#ifndef HASHING_H
#define HASHING_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint64_t hash;
    int count;
} hash_entry_t;

typedef struct {
    hash_entry_t* entries;
    size_t size;
    size_t capacity;
} hash_table_t;

void init_hash_table(hash_table_t* table, size_t capacity, const char* starting_fen);
void init_hash_table_(hash_table_t* table, size_t capacity, uint64_t starting_hash);
_Bool update_hash_table(hash_table_t* table, uint64_t hash);
void free_hash_table(hash_table_t* table);


#endif // HASHING_H
