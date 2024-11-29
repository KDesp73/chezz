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

#define COORDS(square) square.y][square.x
#define PCOORDS(square) square->y][square->x

#define SFC(rank, file) \
    square_from_coords(rank, file)
#define SFN(name) \
    square_from_name(name)

square_t* square_from_fr(size_t rank, size_t file);
square_t* square_from_coords(size_t rank, size_t file);
square_t* square_from_name(const char* name);
square_t** squares(size_t* count, square_t* square, ...);
void square_set_rank(square_t* square, size_t rank);
void square_set_file(square_t* square, size_t file);
void square_set_x(square_t* square, size_t x);
void square_set_y(square_t* square, size_t y);
void square_set_name(square_t* square, const char* name);
void square_free(square_t** square);
_Bool square_cmp(const square_t* square1, const square_t* square2);

void square_print(const square_t square);

#endif // COORDS_H
