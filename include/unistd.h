#ifndef __UNISTD_H__
#define __UNISTD_H__

#define NR_example 0

// Fonction d'enveloppe sans argument
#define syscall0(type,name) \
type name(void) \
{ \
type __res; \
__asm__ volatile ("int $0x80" \
	: "=a" (__res) \
	: "0" (NR_##name)); \
	return __res; \
}

// Fonction d'enveloppe 1 argument
#define syscall1(type,name,atype,a) \
type name(atype a) \
{ \
type __res; \
__asm__ volatile ("int $0x80" \
	: "=a" (__res) \
	: "0" (NR_##name),"b" (a)); \
	return __res; \
}

// Fonction d'enveloppe 2 arguments
#define syscall2(type,name,atype,a,btype,b) \
type name(atype a,btype b) \
{ \
type __res; \
__asm__ volatile ("int $0x80" \
	: "=a" (__res) \
	: "0" (NR_##name),"b" (a),"c" (b)); \
	return __res; \
}

// Fonction d'enveloppe 3 arguments
#define syscall3(type,name,atype,a,btype,b,ctype,c) \
type name(atype a,btype b,ctype c) \
{ \
type __res; \
__asm__ volatile ("int $0x80" \
	: "=a" (__res) \
	: "0" (NR_##name),"b" (a),"c" (b),"d" (c)); \
return __res;\
}

int example();

#endif
