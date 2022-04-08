#include <n7OS/sbrk.h>

void *sbrk(ptrdiff_t diff)
{
	char *s = curptr;
	char *c = s + diff;
	if ((c < curptr) || (c > mem_heap_end)) return ((void*)(-1));
	curptr = c;
	return s;
}
