#include <stdio.h>
#include "libtd/gc.h"
#include "libtd/argv.h"

int main() {
    struct argv a = argv_new();

    argv_printf(&a, "echo test");

    puts(argv_to_string(&a, &a.gc, AF_BRACKET));

    argv_free(&a);
}
