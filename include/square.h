#ifndef COORDS_H
#define COORDS_H

#include <stddef.h>
#include <stdint.h>
typedef struct {
    size_t rank;
    size_t file;
    size_t x;
    size_t y;
    char name[3];
} square_t;

#define COORDS(square) square.y][square.x
#define PCOORDS(square) square->y][square->x

// Stack allocation
_Bool square_from_fr(square_t* square, size_t rank, size_t file);
_Bool square_from_coords(square_t* square, size_t rank, size_t file);
_Bool square_from_name(square_t* square, const char* name);
_Bool square_from_square(square_t* square, square_t src);

// Heap alloctation
void square_free(square_t** square);
square_t* square_new_name(const char* name);
square_t* square_new_coords(size_t y, size_t x);
square_t* square_new_fr(size_t rank, size_t file);
void squares_free(square_t*** squares, size_t count);
square_t** squares(size_t* count, square_t* square, ...);


void square_set_rank(square_t* square, size_t rank);
void square_set_file(square_t* square, size_t file);
void square_set_x(square_t* square, size_t x);
void square_set_y(square_t* square, size_t y);
void square_set_name(square_t* square, const char* name);

_Bool square_is_valid(const char* name);
_Bool square_cmp(square_t square1, square_t square2);

void square_print(const square_t square);
void squares_print(square_t** squares, size_t count);


/*------------------------------------.
| Bitboard representation of squares. |
| Square is an 8-bit integer ranging  |
| from 0-63.                          |
|                                     |
| We start counting from a8 and       |
| finish with h1                      |
`------------------------------------*/

typedef uint8_t Square;

void SquareToName(char buffer[3], Square square);
Square NameToSquare(const char buffer[3]);

void SquareToSquareT(square_t* square, Square src);
int Rank(Square square);
int File(Square square);
_Bool IsSquareValid(Square square);

#endif // COORDS_H
