#ifndef LIBTD_STRING_H
#define LIBTD_STRING_H

#include <sys/types.h>

#include "gc.h"

struct string {
    char* data;

    size_t size;
    size_t capacity;
    struct gc_unit* gc;
};

struct string string_new();

struct string string_new_gc(struct gc_unit* gc);

void string_append(struct string* s, char c);

void string_concat(struct string* s, const char* str);

void string_free(struct string* s);

#endif
