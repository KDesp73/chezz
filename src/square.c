#include "square.h"
#include "extern/clib.h"
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void square_set_rank(square_t* square, size_t rank)
{
    if(rank < 1 || rank > 8){
        ERRO("rank should range between 1 and 8. Found: %zu", rank);
        return;
    }

    square->rank = rank;
    square_t temp;
    square_from_fr(&temp, rank, square->file);
    square->x = square->file-1;
    square->y = rank-1;
    strncpy(square->name, temp.name, 2);
}

void square_set_file(square_t* square, size_t file)
{
    if(file < 1 || file > 8){
        ERRO("file should range between 1 and 8. Found: %zu", file);
        return;
    }

    square->file = file;
    square_t temp;
    square_from_fr(&temp, square->rank, file);
    square->x = file-1;
    square->y = square->rank-1;
    strncpy(square->name, temp.name, 2);

}

void square_set_x(square_t* square, size_t x)
{
    if(x < 0 || x > 8){
        ERRO("x should range between 0 and 7. Found: %zu", x);
        return;
    }

    square->x = x;
    square_t temp;
    square_from_coords(&temp, x, square->y);
    square->file = x+1;
    square->rank = square->y+1;
    strncpy(square->name, temp.name, 2);
}

void square_set_y(square_t* square, size_t y)
{
    if(y < 0 || y > 8){
        ERRO("y should range between 0 and 7. Found: %zu", y);
        return;
    }

    square->y = y;
    square_t temp;
    square_from_coords(&temp, square->x, y);
    square->file = square->x+1;
    square->rank = y+1;
    strncpy(square->name, temp.name, 2);
}

void square_set_name(square_t* square, const char* name)
{
    square_t temp;
    square_from_name(&temp, name);
    strncpy(square->name, name, 3);
    square->name[2] = '\0';
    square->x = temp.x;
    square->y = temp.y;
    square->rank = temp.rank;
    square->file = temp.file;
}

void square_from_coords(square_t* square, size_t y, size_t x)
{
    square_from_fr(square, y+1, x+1);
}

void square_from_fr(square_t* square, size_t rank, size_t file)
{
    if(file < 1 || file > 8){
        ERRO("file should range between 1 and 8. Found: %zu", file);
        square = NULL;
        return;
    }
    if(rank < 1 || rank > 8){
        ERRO("rank should range between 1 and 8. Found: %zu", rank);
        square = NULL;
        return;
    }

    square->file = file;
    square->rank = rank;

    square->x = file-1;
    square->y = rank-1;

    square->name[0] = 'a' + (file-1);
    square->name[1] = '1' + (rank-1);
    square->name[2] = '\0';
}

void square_from_name(square_t* square, const char* name)
{
    if(name == NULL) goto error;
    if(strlen(name) != 2) goto error;
    if(name[0] < 'a' || name[0] > 'h') goto error;
    if(name[1] < '1' || name[1] > '8') goto error;


    strcpy(square->name, name);
    square->file = name[0] - 'a' + 1;
    square->rank = name[1] - '1' + 1;
    square->y = square->rank-1;
    square->x = square->file-1;

error:
    square = NULL;
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

_Bool square_cmp(square_t square1, square_t square2)
{
    return (square1.name[0] == square2.name[0] &&
            square1.name[1] == square2.name[1]);
}

void squares_print(square_t** squares, size_t count)
{
    if(squares == NULL) return;
    if(count == 0) return;
    for(size_t i = 0; i < count; i++){
        if(squares[i] == NULL) continue;
        printf("%s ", squares[i]->name);
    }
    printf("\n");
}


void square_from_square(square_t* square, square_t src)
{
    if(square == NULL) return;

    square->x = src.x;
    square->y = src.y;
    square->file = src.file;
    square->rank = src.rank;
    strncpy(square->name, src.name, 2);
    square->name[2] = '\0';
}

// Heap allocation methods

void square_free(square_t** square)
{
    free(*square);
    *square = NULL;
}

square_t* square_new_name(const char* name)
{
    if(name == NULL) return NULL;
    if(strlen(name) != 2) return NULL;

    if(name[0] < 'a' || name[0] > 'h') return NULL;
    if(name[1] < '1' || name[1] > '8') return NULL;

    square_t* result = malloc(sizeof(square_t));

    strcpy(result->name, name);
    result->file = name[0] - 'a' + 1;
    result->rank = name[1] - '1' + 1;

    result->y = result->rank-1;
    result->x = result->file-1;

    return result;
}

square_t* square_new_coords(size_t y, size_t x)
{
    return square_new_fr(y+1, x+1);
}

square_t* square_new_fr(size_t rank, size_t file)
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

void squares_free(square_t*** squares, size_t count)
{
    for (size_t i = 0; i < count; i++) {
        square_free(&(*squares)[i]);
    }
    free(*squares);
    *squares = NULL;
}
