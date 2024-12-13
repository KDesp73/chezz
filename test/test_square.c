#include "square.h"
#include "tests.h"
#include "extern/test.h"

int test_square_from_name(const char* name, size_t expected_x, size_t expected_y)
{
    square_t square;
    square_from_name(&square, name);

    if(expected_x != square.x) {
        FAIL("At %s. Expected x: %zu. Found: %zu", name, expected_x, square.x);
        return 0;
    }
    if(expected_y != square.y) {
        FAIL("At %s. Expected y: %zu. Found: %zu", name, expected_y, square.y);
        return 0;
    }

    SUCC("Passed for %s", name);
    return 1;
}
