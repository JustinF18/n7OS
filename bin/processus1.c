#include <n7OS/cpu.h>
#include <n7OS/sys.h>
#include <stdio.h>
#include <n7OS/process.h>
#include <unistd.h>
#include <n7OS/time.h>

void processus1()
{
  sti();
  init_timer();
  printf("Hello, world from P1, my pid : %d\n", getpid());
  sleep(5);
  printf("(%d) J'ai dormis 5 secondes.", getpid());
  for (;;)
    ;
}
