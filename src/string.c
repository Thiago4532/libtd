#include <stddef.h>
#include "libtd/string.h"

#include <malloc.h>

#define MIN_CAPACITY 16

struct string
string_new() {
    return string_new_gc(NULL);
}

struct string
string_new_gc(struct gc_unit* gc) {
    struct string ret;

    ret.capacity = MIN_CAPACITY;
    ret.size = 0;
    ret.gc = gc;

    ret.data = gc_malloc(ret.capacity, ret.gc);
    ret.data[0] = '\0';
    return ret;
}

void
string_append(struct string* s, char c) {
    s->data[s->size++] = c;
    if (s->size >= s->capacity)
        s->data = gc_realloc(s->data, s->capacity *= 2, s->gc);
    s->data[s->size] = '\0';
}

void
string_concat(struct string* s, const char* str) {
    while (*str)
        string_append(s, *str++);
}

void
string_free(struct string* s) {
    gc_free(s->data, s->gc);

    s->data = NULL;
    s->capacity = 0;
    s->size = 0;
    s->gc = NULL;
}
