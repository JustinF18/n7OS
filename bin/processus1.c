#include <n7OS/cpu.h>
#include <n7OS/sys.h>
#include <stdio.h>
#include <n7OS/process.h>
#include <unistd.h>
#include <n7OS/time.h>

void function1()
{
  // sti() obligatoire au debut des fonctions, je n'ai pas trouvé comme le supprimer...
  sti();
  printf("Hello, world from function 1, my pid : %d\n", getpid());
  sleep(2);
  printf("(%d) J'ai dormis 2 secondes.\n", getpid());
  sleep(1);
  printf("(%d) Je vais me terminer.\n", getpid());
  exit();
  printf("ERREUR ! Je suis terminé, on ne devrait pas me voir.....\n");
  for (;;)
    ;
}
