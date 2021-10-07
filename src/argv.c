#include "libtd/argv.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "libtd/gc.h"

static void
argv_init(struct argv* a) {
    a->capacity = 8;
    a->argc = 0;
    a->gc = gc_new();

    a->argv = gc_malloc(a->capacity * sizeof(char*), &a->gc);
    *a->argv = NULL;
}

void
argv_reset(struct argv* a) {
    a->argc = 0;
    *a->argv = NULL;
}

struct argv
argv_new(void) {
    struct argv ret;
    argv_init(&ret);
    return ret;
}

void
argv_append(struct argv* a, char* str) {
    a->argv[a->argc++] = str;

    if (a->argc >= a->capacity) {
        a->argv = gc_realloc(a->argv, 2 * a->capacity * sizeof(char*), &a->gc);
        a->capacity *= 2;
    }
    a->argv[a->argc] = NULL;
}

void
argv_dappend(struct argv* a, const char* str) {
    char* s = gc_strdup(str, &a->gc);
    argv_append(a, s);
}

static char*
argv_prep_format(const char* fmt, const char delim, struct gc_unit* gc) {
    char* ret = gc_strdup(fmt, gc);

    char* end = strchr(ret, ' ');
    while (end) {
        *end = delim;
        end = strchr(end + 1, ' ');
    }
    return ret;
}

static bool
argv_printf_arglist(struct argv* a, const char* format, va_list arglist) {
    const char delim = 0x1D; /* ASCII Group Separator (GS) */
    char* fmt = argv_prep_format(format, delim, &a->gc);
    if (!fmt)
        return false;

    va_list tmplist;
    va_copy(tmplist, arglist);
    int len = vsnprintf(NULL, 0, fmt, tmplist);
    va_end(tmplist);
    if (len < 0)
        return false;

    size_t size = len + 1;
    char* buf = gc_malloc(size, &a->gc);
    len = vsprintf(buf, fmt, arglist);
    if (len < 0 || len >= (int)size)
        return false;

    char* end = strchr(buf, delim);
    while (end) {
        *end = '\0';
        argv_append(a, buf);
        buf = end + 1;
        end = strchr(buf, delim);
    }
    argv_append(a, buf);

    return true;
}

bool
argv_printf(struct argv* a, const char* format, ...) {
    va_list arglist;
    va_start(arglist, format);

    argv_reset(a);
    bool ret = argv_printf_arglist(a, format, arglist);
    va_end(arglist);
    return ret;
}

bool
argv_printf_cat(struct argv* a, const char* format, ...) {
    va_list arglist;
    va_start(arglist, format);
    bool ret = argv_printf_arglist(a, format, arglist);
    va_end(arglist);
    return ret;
}

char*
argv_to_string(struct argv* a, struct gc_unit* gc, unsigned int flags) {
    size_t size = 1;
    for (size_t i = 0; i < a->argc; i++) {
        if (i)
            size++;

        int len;
        if (flags & AF_BRACKET)
            len = snprintf(NULL, 0, "[%s]", a->argv[i]);
        else
            len = snprintf(NULL, 0, "%s", a->argv[i]);
        if (len < 0)
            return NULL;
        size += len;
    }

    char* buf = gc_malloc(size, gc);

    char* ptr = buf;
    for (size_t i = 0; i < a->argc; i++) {
        if (i)
            *ptr++ = ' ';
        if (flags & AF_BRACKET)
            ptr += sprintf(ptr, "[%s]", a->argv[i]);
        else
            ptr += sprintf(ptr, "%s", a->argv[i]);
    }
    return buf;
}

void
argv_free(struct argv* a) {
    gc_free_unit(&a->gc);
}
