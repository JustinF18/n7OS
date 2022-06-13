#include <n7OS/cpu.h>
#include <stdio.h>
#include <unistd.h>

void extinction()
{
    // sti() obligatoire au debut des fonctions, je n'ai pas trouvé comme le supprimer...
    sti();
    pid_t my_pid = getpid();
    printf("(%d) Extinction du système dans 1 min...", my_pid);
    for (int i = 0; i < 60; i++)
    {
        printf("(%d) %d...\n", my_pid, 60 - i);
        sleep(1);
    }
    shutdown(1);
    printf("ERREUR ! Je suis terminé, on ne devrait pas me voir.....\n");
    for (;;)
        ;
}
