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
    size_t passed = 0, failed = 0;
    int result = 1;

    va_list args;
    va_start(args, first);

    if (first == 0) {
        failed++;
        result = 0;
    } else passed++;

    int current;
    while ((current = va_arg(args, int)) != END) {
        if (current == 0) {
            result = 0;
            failed++;
        } else passed++;
    }

    va_end(args);

    printf("\n%sPASSED: %zu/%zu%s\n", ANSI_GREEN, passed, passed + failed, ANSI_RESET);
    printf("%sFAILED: %zu/%zu%s\n", ANSI_RED, failed, passed + failed, ANSI_RESET);
    return result;
}
#endif // TEST_IMPLEMENTATION

#endif // TEST_H
