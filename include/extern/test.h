#ifndef TEST_H
#define TEST_H

#include <stdarg.h>
#include <stdio.h>

#define END -1
int test(int first, ...);

#define ANSI_RED "\e[0;31m"
#define ANSI_RESET "\e[0;39m"
#define ANSI_GREEN "\e[0;32m"

#define FAIL(fmt, ...) \
    printf(ANSI_RED "FAILED: " fmt ANSI_RESET "\n", ##__VA_ARGS__)
#define SUCC(fmt, ...) \
    printf(ANSI_GREEN "SUCCESS: " fmt ANSI_RESET "\n", ##__VA_ARGS__)

#define START_TESTS \
    int main(){ \
        return test(

#define END_TESTS \
        END \
    ); \
}

#ifdef TEST_IMPLEMENTATION
int test(int first, ...)
{
    int result = 1;

    va_list args;
    va_start(args, first);

    if (first == 0) {
        result = 0;
    }

    int current;
    while ((current = va_arg(args, int)) != END) {
        if (current == 0) {
            result = 0;
        }
    }

    va_end(args);

    return result;
}
#endif // TEST_IMPLEMENTATION

#endif // TEST_H
