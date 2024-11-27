#include "square.h"
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO: replace asserts with error messages

square_t* square_from_coords(size_t rank, size_t file)
{
    assert(file >= 1 && file <= 8);
    assert(rank >= 1 && rank <= 8);

    square_t* result = malloc(sizeof(square_t));

    result->file = file;
    result->rank = rank;

    result->x = file-1;
    result->y = rank-1;

    result->name[0] = 'a' + (file-1);
    result->name[1] = '1' + (rank-1);

    return result;
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
