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

extern void processus1();

void idle()
{
    // detruire_process(pid1);

    // Bascule vers le processus 1
    activer(1);
    printf("IDLE\n");
    // on ne doit jamais sortir de idle (processus 0)
    while (1)
    {
        // cette fonction arrete le processeur
        hlt();
    }
}

void kernel_start(void)
{
    // Initiation du timer systeme et des appels systemes
    init_timer();
    init_syscall();

    // Activer les interruptions
    sti();

    // Nettoyage de l'ecran
    printf("\f");

    // // Test affichage
    // for (int i = 0; i < 30; i++)
    // {
    //     printf("TEST %d\n", i);
    // }

    // Interruption de test
    // init_irq_entry(50, (uint32_t)handler_IT);
    // Déclanche l'interruption de test (50)
    //__asm__("int $50" ::);

    // if (example() == 1)
    // {
    //     printf("Appel systeme example ok\n");
    // }

    // printf("Bienvenue\n");
    // printf("Bye\n");

    // write("Test write\n", 11);

    // Création du processus 0
    fork("IDLE", (fnptr)idle);

    // Création du processus 1
    fork("Processus 1", (fnptr)processus1);

    idle();
}
