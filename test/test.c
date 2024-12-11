#include "zobrist.h"
#define TEST_IMPLEMENTATION
#include "extern/test.h"
#include "grid-tests.h"

int main(int argc, char** argv){
    init_zobrist();

    return !test(
        TEST_GRID,
        END
    );
}
