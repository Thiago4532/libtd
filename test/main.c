#include <errno.h>
#include <stdio.h>

#include "libtd/argv.h"
#include "libtd/gc.h"
#include "libtd/string.h"

int
main() {
    struct gc_unit gc = gc_new();
    struct string str = string_new();

    string_concat(&str, "thiago mota");

    printf("String: %s\n", str.data);
    return 0;
}
