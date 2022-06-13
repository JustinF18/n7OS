#include <n7OS/cpu.h>
#include <stdio.h>
#include <unistd.h>

// extern void extinction();

void function1()
{
  // sti() obligatoire au debut des fonctions, je n'ai pas trouvé comme le supprimer...
  sti();
  pid_t my_pid = getpid();
  printf("(%d) Hello, world from function 1, my pid : %d\n", my_pid, my_pid);
  sleep(2);
  printf("(%d) J'ai dormis 2 secondes.\n", my_pid);
  sleep(1);
  // fork("Extinction", (fnptr)extinction);
  printf("(%d) Je vais me terminer.\n", my_pid);
  exit();
  printf("ERREUR ! Je suis terminé, on ne devrait pas me voir.....\n");
  for (;;)
    ;
}
