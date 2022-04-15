#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <stdio.h>


void kernel_start(void) {
    printf("\f");
    for (int i = 0; i < 30; i++)
    {
        printf("TEST %d\n", i);
    }

    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}
