#ifndef TESTS_H
#define TESTS_H

#include <stddef.h>
int test_square_from_name(const char* name, size_t expected_x, size_t expected_y);


#define TEST_SQUARE_FROM_NAME \
    test_square_from_name("a1", 0, 0), \
    test_square_from_name("b1", 1, 0), \
    test_square_from_name("c1", 2, 0), \
    test_square_from_name("d1", 3, 0), \
    test_square_from_name("e1", 4, 0), \
    test_square_from_name("f1", 5, 0), \
    test_square_from_name("g1", 6, 0), \
    test_square_from_name("h1", 7, 0), \
    test_square_from_name("a1", 0, 0), \
    test_square_from_name("b2", 1, 1), \
    test_square_from_name("c3", 2, 2), \
    test_square_from_name("d4", 3, 3), \
    test_square_from_name("e5", 4, 4), \
    test_square_from_name("f6", 5, 5), \
    test_square_from_name("g7", 6, 6), \
    test_square_from_name("h8", 7, 7), \
    test_square_from_name("a2", 0, 1), \
    test_square_from_name("a3", 0, 2)


#endif // TESTS_H
