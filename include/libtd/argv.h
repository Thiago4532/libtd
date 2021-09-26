#ifndef LIBTD_ARGV_H
#define LIBTD_ARGV_H

#include <stdbool.h>

#include "libtd/gc.h"

struct argv {
    struct gc_unit gc;
    size_t capacity;
    size_t argc;
    char** argv;
};

struct argv argv_new(void);

void argv_reset(struct argv* a);

void argv_append(struct argv* a, char* str);

void argv_dappend(struct argv* a, const char* str);

bool argv_printf(struct argv* a, const char* format, ...)
    __attribute__((format(__printf__, 2, 3)));

bool argv_printf_cat(struct argv* a, const char* format, ...)
    __attribute__((format(__printf__, 2, 3)));

#define AF_BRACKET 0x1
char* argv_to_string(struct argv* a, struct gc_unit* gc, unsigned int flags);

void argv_free(struct argv* a);

/* Inline functions */

static inline char*
argv_str(struct argv* a) {
    return argv_to_string(a, &a->gc, 0);
}

#endif
