#ifndef COORDS_H
#define COORDS_H

#include <stddef.h>
typedef struct {
    size_t rank;
    size_t file;
    size_t x;
    size_t y;
    char name[2];
} square_t;

#define SFC(rank, file) \
    square_from_coords(rank, file)
#define SFN(name) \
    square_from_name(name)

square_t* square_from_coords(size_t rank, size_t file);
square_t* square_from_name(const char* name);
square_t** squares(size_t* count, square_t* square, ...);

void square_print(const square_t square);

#endif // COORDS_H
