# Square

## Create a square

```c
#include <chess/square.h>

int main() {
    // Stack allocation
    square_t square;
    square_from_name(&square, "e2");
    square_from_coords(&square, 0, 4); // 0->7
    square_from_fr(&square, 1, 8); // 1->8 (rank, file)

    // Heap allocation (needs freeing)
    square_t* square1 = square_new_name("e4");
    square_free(&square1);

    return 0;
}
```
