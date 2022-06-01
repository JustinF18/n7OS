#include <stdio.h>
#include <n7OS/process.h>

void processus1()
{
  printf("Hello, world from P1\n");
  // Bascule vers le processus 0
  schedule(1, 0);
  for (;;)
    ;
}
