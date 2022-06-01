#include "n7OS/process.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

struct process_t process_table[NB_PROC];
// uint32_t stack_table[NB_PROC * STACK_SIZE];

static const struct process_t EmptyStruct;

pid_t next_pid = 0;

void ctx_sw(void *ctx_old, void *ctx_new);

int allouer_pid()
{
    if (next_pid + 1 < NB_PROC)
    {
        return next_pid;
    }
    else
    {
        return -1;
    }
}

// void *init_stack()
// {
//     // allouer_pid à déjà été appelé, next_pid est correct
//     return &stack_table[next_pid * STACK_SIZE];
// }

pid_t creer_process(const char *name, fnptr function)
{
    // Get a new pid
    pid_t pid;
    if ((pid = allouer_pid()) == -1)
    {
        printf("Cannot create new process \n");
        // exit(1);
        return -1;
    }

    // Create an entry in process_table
    process_table[pid].pid = pid;
    // process_table[pid].stack = init_stack();
    process_table[pid].state = PRET_SUSPENDU;

    // Add the function address in stack
    process_table[pid].stack[STACK_SIZE - 1] = (uint32_t)function;

    // Create context switch
    process_table[pid].ctx[EBX] = 0;
    process_table[pid].ctx[ESP] = (uint32_t)&process_table[pid].stack[STACK_SIZE - 1];
    process_table[pid].ctx[EBP] = 0;
    process_table[pid].ctx[ESI] = 0;
    process_table[pid].ctx[EDI] = 0;

    // Process created
    printf("Process %d created\n", pid);
    next_pid++;
    return pid;
}

int detruire_process(pid_t pid)
{
    // Cleaning the process_table
    process_table[pid] = EmptyStruct;
    // Cleaning the stack
    // for (int i = 0; i < STACK_SIZE; i++)
    // {
    //     stack_table[pid * STACK_SIZE + i] = 0;
    // }
    // memset(&stack_table[pid * STACK_SIZE], 0, STACK_SIZE);
    // Remove an entry in process_table
    // process_table[pid].pid = 0;
    // process_table[pid].stack = NULL;
    printf("Process %d deleted\n", pid);
    return 0;
}

void schedule(pid_t old, pid_t new)
{
    printf("Switching from %d to %d\n", old, new);
    ctx_sw((void *)&process_table[old].ctx, (void *)&process_table[new].ctx);
}