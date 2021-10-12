/* gc - A memory allocation tracker
 *
 * This is a library to help keep track of dynamic memory allocation.
 * WARNING: This isn't a "real" garbage collector.
 */

#ifndef LIBTD_GC_H
#define LIBTD_GC_H

#include <sys/types.h>

/**
 * Garbage collection entry for one dynamically allocated block of memory.
 *
 * This structure represents one link in the `struct gc_unit` linked list.
 */
struct gc_entry {
    struct gc_entry* next;
    struct gc_entry* prev;
};

struct gc_custom_entry {
    struct gc_custom_entry* next;

    void* addr;
    void (*free_func)(void*);
};

/**
 * Garbage collection unit used to keep track of dynamic allocated memory.
 *
 * @member list         Double linked list
 * @member custom_list  Custom entry linked list
 */
struct gc_unit {
    struct gc_entry* list;
    struct gc_custom_entry* custom_list;
};

/**
 * Allocates @size bytes in the `struct gc_unit` and returns a pointer to the
 * allocated memory. It's the garbage collector equivalent of malloc().
 *
 * If @g is NULL, then gc_malloc() is going to act like a normal malloc().
 *
 * @param size  Number of bytes to alloc.
 * @param *g    A garbage collector unit.
 *
 * @returns Returns a pointer to the allocated memory.
 */
void* gc_malloc(size_t size, struct gc_unit* g);

/**
 * Reallocates the memory block pointed to by @ptr to @size bytes.
 * It's the garbage collector equivalent of realloc().
 *
 * @param *ptr  A pointer to the memory block.
 * @param size  The number of bytes after reallocation.
 * @param *g    A garbage collector unit.
 *
 * @returns Returns a pointer to the alocated memory.
 */
void* gc_realloc(void* ptr, size_t size, struct gc_unit* g);

/**
 * Frees the memory space pointed to by @ptr in the @g unit.
 * You must use this function to free the memory allocated using gc functions.
 *
 * @param ptr  A pointer to the memory block.
 * @param *g   A garbage collector unit.
 */
void gc_free(void* ptr, struct gc_unit* g);

void gc_addcustom(void* addr, void (*free_function)(void*), struct gc_unit* g);

/**
 * Frees all the memory allocated in the @g unit.
 *
 * @param *g  A garbage collector unit.
 */
void gc_free_unit(struct gc_unit* g);

/**
 * Returns a pointer to a new string which is a duplicate of the string @str.
 * Memory for the new string is obtained with gc_malloc().
 * It's the garbage collector equivalent of strdup().
 *
 * @param *str  A pointer to a string.
 * @param *g    A garbage collector unit.
 *
 * @returns Returns a pointer to the duplicated string.
 */
char* gc_strdup(const char* str, struct gc_unit* g);

/* Inline functions */

/**
 * Initialize an already allocated struct gc_unit.
 * Note that it does not return a pointer, but a struct argv directly.
 *
 * @returns Returns an initialised and empty struct argv.
 */
static inline void
gc_init(struct gc_unit* a) {
    a->list = (void*)0;
    a->custom_list = (void*)0;
}

/**
 * Allocates a new struct gc_unit and ensures it is initialised.
 * Note that it does not return a pointer, but a `struct gc_unit` directly.
 *
 * @returns Returns an initialised and empty `struct gc_unit`.
 */
static inline struct gc_unit
gc_new(void) {
    struct gc_unit ret;
    gc_init(&ret);
    return ret;
}

#endif
