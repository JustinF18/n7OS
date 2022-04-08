#ifndef __SBRK_H__
#define __SBRK_H__

#include <types.h>

extern char mem_heap[];
extern char mem_heap_end[];
static char *curptr = mem_heap;

void *sbrk(ptrdiff_t diff);

#endif
