#include "square.h"
#include "extern/clib.h"
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO: replace asserts with error messages

void square_set_rank(square_t* square, size_t rank)
{
    if(rank < 1 || rank > 8){
        ERRO("rank should range between 1 and 8. Found: %zu", rank);
        return;
    }

    square->rank = rank;
    square_t* temp = square_from_fr(rank, square->file);
    square->x = square->file-1;
    square->y = rank-1;
    strncpy(square->name, temp->name, 2);
}

void square_set_file(square_t* square, size_t file)
{
    if(file < 1 || file > 8){
        ERRO("file should range between 1 and 8. Found: %zu", file);
        return;
    }

    square->file = file;
    square_t* temp = square_from_fr(square->rank, file);
    square->x = file-1;
    square->y = square->rank-1;
    strncpy(square->name, temp->name, 2);

}

void square_set_x(square_t* square, size_t x)
{
    if(x < 0 || x > 8){
        ERRO("x should range between 0 and 7. Found: %zu", x);
        return;
    }

    square->x = x;
    square_t* temp = square_from_coords(x, square->y);
    square->file = x+1;
    square->rank = square->y+1;
    strncpy(square->name, temp->name, 2);
}

void square_set_y(square_t* square, size_t y)
{
    if(y < 0 || y > 8){
        ERRO("y should range between 0 and 7. Found: %zu", y);
        return;
    }

    square->y = y;
    square_t* temp = square_from_coords(square->x, y);
    square->file = square->x+1;
    square->rank = y+1;
    strncpy(square->name, temp->name, 2);
}

void square_set_name(square_t* square, const char* name)
{
    square_t* temp = square_from_name(name);
    strncpy(square->name, name, 2);
    square->x = temp->x;
    square->y = temp->y;
    square->rank = temp->rank;
    square->file = temp->file;
}

square_t* square_from_coords(size_t y, size_t x)
{
    return square_from_fr(y+1, x+1);
}

square_t* square_from_fr(size_t rank, size_t file)
{
    if(file < 1 || file > 8){
        ERRO("file should range between 1 and 8. Found: %zu", file);
        return NULL;
    }
    if(rank < 1 || rank > 8){
        ERRO("rank should range between 1 and 8. Found: %zu", rank);
        return NULL;
    }

    square_t* result = malloc(sizeof(square_t));

    result->file = file;
    result->rank = rank;

    result->x = file-1;
    result->y = rank-1;

    result->name[0] = 'a' + (file-1);
    result->name[1] = '1' + (rank-1);

    return result;
}

void square_free(square_t** square)
{
    free(*square);
    *square = NULL;
}

square_t* square_from_name(const char* name)
{
    assert(strlen(name) == 2);
    assert(name != NULL);
    assert(name[0] >= 'a' && name[0] <= 'h');
    assert(name[1] >= '1' && name[1] <= '8');

    square_t* result = malloc(sizeof(square_t));

    strcpy(result->name, name);
    result->file = name[0] - 'a' + 1;
    result->rank = name[1] - '1' + 1;

    result->y = result->rank-1;
    result->x = result->file-1;

    return result;
}

void square_print(const square_t square)
{
    printf("square {\n\tname: %s\n\trank: %zu\n\tfile: %zu\n\tx: %zu\n\ty: %zu\n}\n",
        square.name, 
        square.rank, square.file, 
        square.x, square.y
    );
}

square_t** squares(size_t* count, square_t* square, ...)
{
    va_list args;
    va_start(args, square);

    square_t** squares_array = (square_t**)malloc(100 * sizeof(square_t*));
    int i = 0;

    squares_array[i++] = square;

    square_t* next_square;
    while ((next_square = va_arg(args, square_t*)) != NULL) {
        squares_array[i++] = next_square;
    }

    va_end(args);

    squares_array = (square_t**)realloc(squares_array, (i + 1) * sizeof(square_t*));

    *count = i;
    return squares_array;
}

_Bool square_cmp(const square_t* square1, const square_t* square2)
{
    return (
            square1->name[0] == square2->name[0] &&
            square1->name[1] == square2->name[1]
            );
}
