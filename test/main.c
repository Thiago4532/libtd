#include <errno.h>
#include <stdio.h>

#include "libtd/argv.h"
#include "libtd/gc.h"
#include "libtd/string.h"

int
main() {
    struct gc_unit gc = gc_new();
    struct string str = string_new_gc(&gc);

    string_concat(&str, "thiago mota");

    struct argv a = argv_new();

    argv_printf(&a, "echo %s", "Testing function...");

    printf("Argv: %s\n", argv_to_string(&a, &gc, 0));
    printf("String: %s\n", str.data);

    gc_free_unit(&gc);
    argv_free(&a);
    return 0;
}
