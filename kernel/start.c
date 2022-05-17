#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <stdio.h>
#include <n7OS/irq.h>
#include <n7OS/time.h>
#include <n7OS/sys.h>
#include <unistd.h>

extern void handler_IT();

void kernel_start(void)
{
    // Initiation du timer systeme et des appels systemes
    init_timer();
    init_syscall();

    // Nettoyage de l'ecran
    printf("\f");
    // // Test affichage
    // for (int i = 0; i < 30; i++)
    // {
    //     printf("TEST %d\n", i);
    // }

    // Interruption de test
    init_irq_entry(50, (uint32_t)handler_IT);
    // Activer les interruptions
    sti();

    // Déclanche l'interruption de test (50)
    //__asm__("int $50" ::);

    printf("Bienvenue\n");
    printf("Bye\n");

    if (example() == 1)
    {
        printf("Appel systeme example ok\n");
    }

    write("Test write", 10);

    // on ne doit jamais sortir de kernel_start
    while (1)
    {
        // cette fonction arrete le processeur
        hlt();
    }
}
