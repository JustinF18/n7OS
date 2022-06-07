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

/*
 * This is the function to create a process
 */
pid_t creer_process(const char *name, fnptr function);

/*
 * This is the function to destroy a process
 */
int detruire_process();

/*
 * Return the pid of the process which is running
 */
pid_t get_pid();

/*
 * This is the function that do the scheduling and which check the sleeping List or ended list.
 */
void schedule();

/*
 * This is the function that block the process which is running for a given time
 */
int bloquer(int time);

// SCHEDULING
typedef struct ProcessWaiting ProcessWaiting;
struct ProcessWaiting
{
    pid_t pid;
    ProcessWaiting *next;
};
typedef struct ProcessReadyFile
{
    ProcessWaiting *first;
    ProcessWaiting *last;
    int size;
} ProcessReadyFile;

typedef struct ProcessSleeping ProcessSleeping;
struct ProcessSleeping
{
    pid_t pid;
    int wake_up_time;
    ProcessSleeping *next;
};
typedef struct ProcessSleepingFile
{
    ProcessSleeping *first;
    int size;
} ProcessSleepingFile;

typedef struct ProcessOverFile
{
    ProcessWaiting *first;
    int size;
} ProcessOverFile;

#endif