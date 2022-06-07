#include <n7OS/process.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stddef.h>
#include <n7OS/console.h>
#include <n7OS/time.h>

extern void ctx_sw(void *ctx_old, void *ctx_new);

// Scheduling
ProcessReadyFile readyList;       // Store the processes that are waiting for CPU
ProcessSleepingFile sleepingList; // Store the processes that are sleeping
ProcessOverFile endedList;        // Store the processes that are ended and waiting for cleaning
pid_t pid_actif = 0;              // PID of procces which is running

// Add a process pid to the ready list
int addProcess(pid_t pid)
{
    ProcessWaiting *new_process;
    // Allocation of ressources
    if ((new_process = (ProcessWaiting *)malloc(sizeof(ProcessWaiting))) == NULL)
        return -1;
    new_process->pid = pid;
    new_process->next = NULL;
    // Add the element to our FIFO
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
        readyList.last = new_process;
    }
    return 0;
}

// Get the process pid of the next process to run,
// before calling this function, check that the list of waiting processes isn't void.
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
    printf("[INFO] - Process %d created\n", pid);
    next_pid++;

    addProcess(pid);

    return pid;
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

// If "bloquer" equals to 0 we have to add the old process to the list of waiting users
void scheduler(int bloquer)
{
    // Changer de processus
    pid_t old = pid_actif;
    if (readyList.first != NULL)
    {
        pid_t new = depiler();
        if (bloquer == 0)
        {
            // printf("Save old process %d (new is %d)...\n", old, new);
            addProcess(old);
        }
        if (new == old)
        {
            return;
        }
        display_current_process(new);
        pid_actif = new;
        // printf("Switching from %d to %d\n", old, new);
        ctx_sw((void *)&process_table[old].ctx, (void *)&process_table[new].ctx);
    }
}

void schedule()
{
    // Check for process to wake up
    if (sleepingList.first != NULL)
    {
        int current_time = get_time();
        ProcessSleeping *current = sleepingList.first;
        while (current != NULL)
        {
            if (current_time > current->wake_up_time)
            {
                // Need to wake up this process
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
    // Check for process to removed from process table
    if (endedList.first != NULL)
    {
        // There are processes to remove
        ProcessWaiting *current = endedList.first;
        while (current != NULL)
        {
            // Cleaning the process_table
            process_table[current->pid] = EmptyStruct;
            printf("[INFO] - Process %d deleted\n", current->pid);
            // Supprimer le processus de la file
            ProcessWaiting *supp_element;
            supp_element = current;
            current = current->next;
            free(supp_element);
            endedList.size = endedList.size - 1;
        }
        endedList.first = NULL;
    }
    // Changer de processus
    scheduler(0);
}

int detruire_process()
{
    pid_t pid = pid_actif;
    ProcessWaiting *new_process;
    // Allocate ressources
    if ((new_process = (ProcessWaiting *)malloc(sizeof(ProcessWaiting))) == NULL)
        return -1;
    new_process->pid = pid;
    new_process->next = NULL;
    // Add the process to delete in our list
    if (endedList.first == NULL)
    {
        endedList.first = new_process;
    }
    else
    {
        new_process->next = endedList.first;
        endedList.first = new_process;
    }
    endedList.size++;
    scheduler(1);
    return 0;
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
    new_process->next = NULL;
    // Add the process to the sleeping users list
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
    printf("[INFO] - Process %d will sleep for %d s.\n", pid, seconds);
    scheduler(1);
    return 0;
}