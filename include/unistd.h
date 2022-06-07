#ifndef __UNISTD_H__
#define __UNISTD_H__

#include <n7OS/process.h>

#define NR_example 0
#define NR_shutdown 1
#define NR_write 2
#define NR_fork 3
#define NR_exit 4
#define NR_getpid 5
#define NR_sleep 6

// Fonction d'enveloppe sans argument
#define syscall0(type, name)                \
	type name(void)                         \
	{                                       \
		type __res;                         \
		__asm__ volatile("int $0x80"        \
						 : "=a"(__res)      \
						 : "0"(NR_##name)); \
		return __res;                       \
	}

// Fonction d'enveloppe 1 argument
#define syscall1(type, name, atype, a)              \
	type name(atype a)                              \
	{                                               \
		type __res;                                 \
		__asm__ volatile("int $0x80"                \
						 : "=a"(__res)              \
						 : "0"(NR_##name), "b"(a)); \
		return __res;                               \
	}

// Fonction d'enveloppe 2 arguments
#define syscall2(type, name, atype, a, btype, b)            \
	type name(atype a, btype b)                             \
	{                                                       \
		type __res;                                         \
		__asm__ volatile("int $0x80"                        \
						 : "=a"(__res)                      \
						 : "0"(NR_##name), "b"(a), "c"(b)); \
		return __res;                                       \
	}

// Fonction d'enveloppe 3 arguments
#define syscall3(type, name, atype, a, btype, b, ctype, c)          \
	type name(atype a, btype b, ctype c)                            \
	{                                                               \
		type __res;                                                 \
		__asm__ volatile("int $0x80"                                \
						 : "=a"(__res)                              \
						 : "0"(NR_##name), "b"(a), "c"(b), "d"(c)); \
		return __res;                                               \
	}

// Définition des fonctions provoquant les appels systèmes
int example();
int shutdown(int n);
int write(const char *s, int len);
pid_t fork(const char *name, fnptr function);
int exit();
pid_t getpid();
int sleep(int seconds);

#endif
