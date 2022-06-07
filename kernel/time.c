#include <n7OS/time.h>
#include <n7OS/console.h>
#include <n7OS/irq.h>
#include <n7OS/cpu.h>
#include <stdio.h>
#include <n7OS/process.h>

extern void handler_TIMER();

int compteur = 0;

void init_timer()
{
    // Initialisation du timer
    outb(0x34, 0x43);
    outb(FREQUENCE & 0xFF, 0x40);
    outb(FREQUENCE >> 8, 0x40);

    // Ajout de l'interruption
    init_irq_entry(0x20, (uint32_t)handler_TIMER);

    // Démasque l'interruption
    outb(inb(0x21) & 0xfe, 0x21);
}

void print_hhmmss()
{
    int s = compteur / 1000;
    int m = s / 60;
    int h = m / 60;

    display_time(h, m, s);
}

void handler_timer()
{
    // Masque l'interruption
    outb(inb(0x21) | 1, 0x21);

    // ACK de l'interruption
    outb(0x20, 0x20);

    // Incrémente le compteur du système
    compteur++;

    // Démasque l'interruption
    outb(inb(0x21) & 0xfe, 0x21);

    // Update the time display on the screen
    if (compteur % 1000 == 0)
    {
        print_hhmmss();
    }

    // Call the scheduler every 100ms
    if (compteur % 100 == 0)
    {
        schedule();
    }
}

int get_time()
{
    return compteur;
}