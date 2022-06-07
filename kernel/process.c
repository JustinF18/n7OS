#include <n7OS/process.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stddef.h>
#include <n7OS/console.h>
#include <n7OS/time.h>

extern void ctx_sw(void *ctx_old, void *ctx_new);

// Ordonancement
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

ProcessReadyFile readyList;
pid_t pid_actif = 0;

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
ProcessSleepingFile sleepingList;

// Add a process pid to the ready list
int addProcess(pid_t pid)
{
    ProcessWaiting *new_process;
    if ((new_process = (ProcessWaiting *)malloc(sizeof(ProcessWaiting))) == NULL)
        return -1;
    new_process->pid = pid;
    if (readyList.first == NULL)
    {
        // The list is empty, add the process
        readyList.first = new_process;
        readyList.last = new_process;
    }
    else
    {
        // Add the process at the end of the list
        readyList.last->next = new_process;
    }
    return 0;
}

// Get the process pid of the next process to run
pid_t depiler()
{
    pid_t pid = readyList.first->pid;
    ProcessWaiting *supp_element;
    // if (readyList == NULL)
    //     return -1;
    supp_element = readyList.first;
    readyList.first = readyList.first->next;
    if (readyList.first == NULL)
    {
        readyList.last = NULL;
    }
    free(supp_element);
    return pid;
}

// Process struct
struct process_t process_table[NB_PROC];
static const struct process_t EmptyStruct;
pid_t next_pid = 0;

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
    process_table[pid].state = PRET;

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

    addProcess(pid);

    return pid;
}

int detruire_process(pid_t pid)
{
    // Cleaning the process_table
    process_table[pid] = EmptyStruct;
    printf("Process %d deleted\n", pid);
    return 0;
}

// Get the pid of the process which is running
pid_t get_pid()
{
    return pid_actif;
}

void activer(pid_t pid)
{
    pross_state_t etat = process_table[pid].state;
    if (etat == BLOQUE)
    {
        process_table[pid].state = PRET;
    }
    addProcess(pid);
    // schedule();
}

void scheduler(int bloquer)
{
    // Changer de processus
    pid_t old = pid_actif;
    if (readyList.first != NULL)
    {
        pid_t new = depiler();

        if (bloquer == 0)
        {
            addProcess(old);
        }
        if (new == old)
        {
            return;
        }
        display_current_process(new);
        pid_actif = new;
        ctx_sw((void *)&process_table[old].ctx, (void *)&process_table[new].ctx);
    }
}

void schedule()
{
    if (sleepingList.first != NULL)
    {
        // Debloquer tous les processus qui ont fini de dormir
        int current_time = get_time();
        ProcessSleeping *current = sleepingList.first;
        while (current != NULL)
        {
            if (current_time > current->wake_up_time)
            {
                activer(current->pid);
                // Supprimer le processus de la file
                ProcessSleeping *supp_element;
                supp_element = current;
                current = current->next;
                free(supp_element);
                sleepingList.size = sleepingList.size - 1;
            }
            else
            {
                current = current->next;
            }
        }
        if (sleepingList.size == 0)
        {
            sleepingList.first = NULL;
        }
    }
    // Changer de processus
    scheduler(0);
}

// Sleep the process for a given time
int bloquer(int seconds)
{
    pid_t pid = pid_actif;
    ProcessSleeping *new_process;
    if ((new_process = (ProcessSleeping *)malloc(sizeof(ProcessSleeping))) == NULL)
        return -1;
    new_process->pid = pid;
    new_process->wake_up_time = get_time() + seconds * 1000;
    if (sleepingList.first == NULL)
    {
        sleepingList.first = new_process;
    }
    else
    {
        new_process->next = sleepingList.first;
        sleepingList.first = new_process;
    }
    sleepingList.size++;
    process_table[pid].state = BLOQUE;
    scheduler(1);
    return 0;
}