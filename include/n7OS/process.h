#ifndef PROCESS_H
#define PROCESS_H

#include <inttypes.h>

#define NB_PROC 100
#define STACK_SIZE 1024

typedef uint32_t pid_t;
// typedef (void*) (*fnptr)(); // Ne marche pas?
typedef void *(*fnptr)();

typedef enum
{
    ELU,
    PRET,
    BLOQUE
} pross_state_t;

typedef enum
{
    EBX,
    ESP,
    EBP,
    ESI,
    EDI,
} registre;

typedef struct process_t process_t;
struct process_t
{
    uint32_t stack[STACK_SIZE];
    pross_state_t state;
    pid_t pid;
    const char *name;
    uint32_t ctx[5];
};

pid_t creer_process();
int detruire_process();

void schedule();

void activer(pid_t pid);

#endif