#include "grid-tests.h"
#include "extern/test.h"
#include <stdlib.h>
#include <string.h>

#define NUM_TESTS 100000
int test_hash_uniqueness()
{
    uint64_t hashes[NUM_TESTS];
    memset(hashes, 0, sizeof(hashes));

    for (int i = 0; i < NUM_TESTS; i++) {
        uint64_t hash = ((uint64_t)rand() << 32) | rand();
        for (int j = 0; j < i; j++) {
            if (hashes[j] == hash) {
                FAIL("Collision detected: 0x%llx", (unsigned long long)hash);
                return 0;
            }
        }
        hashes[i] = hash;
    }
    SUCC("No collisions found in %d tests", NUM_TESTS);
    return 1;
}

int test_zobrist_uniqueness()
{
    
}
