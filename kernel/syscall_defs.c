#include "syscall_defs.h"

fn_ptr syscall_table[NB_SYSCALL];

void add_syscall(int num, fn_ptr function) {
  if (num < NB_SYSCALL) {
    syscall_table[num]= function;
  }
}
