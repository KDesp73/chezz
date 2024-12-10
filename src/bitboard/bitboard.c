#include "bitboard.h"
#include "square.h"
#include <stdio.h>

void Uint32Print(uint32_t value)
{
    for (int i = 31; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
        if (i % 4 == 0) {
            printf(" ");
        }
    }
    printf("\n");
}


