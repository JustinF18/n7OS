#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <stdio.h>
#include <n7OS/irq.h>

extern void handler_IT();

void kernel_start(void) {
    // Nettoyage de l'ecran
    printf("\f");
    // // Test affichage
    // for (int i = 0; i < 30; i++)
    // {
    //     printf("TEST %d\n", i);
    // }
    
    init_irq_entry(50, (uint32_t) handler_IT);
    // Activer les interruptions
    sti();
    // Interruption 50
    __asm__("int $50"::);

    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}
