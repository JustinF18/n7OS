#include "syscall_defs.h"

void add_syscall(int num, fn_ptr function) {
  if (num < NB_SYSCALL) {
    syscall_table[num]= function;
  }
}
