#include "libtd/gc.h"

#include <malloc.h>
#include <stdbool.h>
#include <string.h>

void*
gc_malloc(size_t size, struct gc_unit* g) {
    void* ret;

    if (g) {
        struct gc_entry* e = malloc(sizeof(struct gc_entry) + size);
        ret = (char*)e + sizeof(struct gc_entry);

        e->next = g->list;
        e->prev = NULL;

        g->list = e;
        if (e->next)
            e->next->prev = e;
    } else {
        ret = malloc(size);
    }

    return ret;
}

void*
gc_realloc(void* p, size_t size, struct gc_unit* gc) {
    if (gc) {
        struct gc_entry* e = (struct gc_entry*)p - 1;

        e = realloc(e, sizeof(struct gc_entry) + size);

        if (!e->prev)
            gc->list = e;
        else
            e->prev->next = e;
        if (e->next)
            e->next->prev = e;

        p = (char*)e + sizeof(struct gc_entry);
    } else {
        p = realloc(p, size);
    }
    return p;
}

void
gc_free(void* p, struct gc_unit* gc) {
    if (gc) {
        if (!p)
            return;
        struct gc_entry* e = (struct gc_entry*)p - 1;

        if (!e->prev)
            gc->list = e->next;
        else
            e->prev->next = e->next;
        if (e->next)
            e->next->prev = e->prev;

        free(e);
    } else {
        free(p);
    }
}

void gc_addcustom(void* addr, void (*free_function)(void*), struct gc_unit* g) {
    if (g) {
        struct gc_custom_entry* ec = malloc(sizeof *ec);
        ec->addr = addr;
        ec->free_func = free_function;

        ec->next = g->custom_list;
        g->custom_list = ec;
    }
}

void
gc_free_unit(struct gc_unit* g) {
    struct gc_custom_entry* ec;
    ec = g->custom_list;
    g->custom_list = NULL;

    while (ec) {
        struct gc_custom_entry* next = ec->next;
        ec->free_func(ec->addr);
        free(ec);
        ec = next;
    }

    struct gc_entry* e;
    e = g->list;
    g->list = NULL;

    while (e) {
        struct gc_entry* next = e->next;
        free(e);
        e = next;
    }
}

char*
gc_strdup(const char* str, struct gc_unit* g) {
    size_t len = strlen(str) + 1;

    char* ret = gc_malloc(len, g);
    return memcpy(ret, str, len);
}
