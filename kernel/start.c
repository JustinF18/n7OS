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

extern void function1();

void idle()
{
    printf("\nDemarrage OK...\n");
    show_system_infobar();
    // Affichage du titre
    display_title("n7OS - Justin");
    printf("\nInitialisation OK...\n");
    printf("\n\nBienvenue sur mon 'petit' OS !\n\n");
    // Création du processus 1
    fork("Fct 1", (fnptr)function1);
    fork("Fct 2", (fnptr)function1);
    //   on ne doit jamais sortir de idle (processus 0)
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
    // On utilise pas printf("\f"); car on veut aussi supprimer la premiere ligne
    clear_display(0);

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

    // write("Test write\n", 11);

    // Création du processus 0
    fork("IDLE", (fnptr)idle);

    idle();
}
