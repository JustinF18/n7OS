#ifndef __SYSCALL_DEFS_H__
#define __SYSCALL_DEFS_H__

#include <n7OS/process.h>

#define NB_SYSCALL 7

int sys_example();
int sys_shutdown();
int sys_write();

pid_t sys_fork();
int sys_exit();
pid_t sys_getpid();
int sys_sleep();

typedef int (*fn_ptr)();
extern fn_ptr syscall_table[NB_SYSCALL];

void add_syscall(int num, fn_ptr function);

#endif
