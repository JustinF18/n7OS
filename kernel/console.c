#include <n7OS/console.h>
#include <n7OS/cpu.h>
#include "inttypes.h"
#define NROW 25
#define NCOL 80

static uint16_t pos = 0;

void update_cursor() {
    outb(0xF, 0x3D4);
    outb(pos & 0xFF, 0x3D5);
    outb(0xE, 0x3D4);
    outb(pos >> 8, 0x3D5);
}

void set_cursor(uint8_t row, uint8_t col) {
    pos = NCOL*row + col;
    update_cursor();
}

void console_putchar(const char c) {
    uint8_t *scr_tab= (uint8_t *) 0xB8000;

    // Check if the screen is full
    if (pos > NROW * NCOL) {
        for (int i = 0; i < NROW * NCOL; i++)
        {
            scr_tab[i*2] = scr_tab[(i + NCOL) * 2];
            scr_tab[i*2 + 1] = scr_tab[(i + NCOL) * 2 + 1];
        }
        pos = (NROW - 1) * NCOL + 1;
    }

    if (c > 31 && c < 127) {
        // Print character on screen
        scr_tab[pos*2] = c;
        scr_tab[pos*2 + 1]= 0x0F;
        pos += 1;
    } else if (c == '\b') {
        // Move cursor back
        pos -= 1;
    } else if (c == '\t') {
        // Add a tab : 8 character space
        pos += 8;
    } else if (c == '\n') {
        // Move the cursor to the next line, first column
        pos = ((pos / NCOL) + 1) * NCOL;
    } else if (c == '\f') {
        // Clean the screen and move the cursor to first row and column
        for (int i = 0; i < NCOL*NROW; i++)
        {
            scr_tab[i*2] = ' ';
            scr_tab[i*2+1] = 0x0F;
        }
        pos = 0;
    } else if (c == '\r') {
        // Move the cursor at the row start
        pos = (pos / NCOL) * NCOL;
    }
}

// Print a string on the screen
void console_putbytes(const char *s, int len) {
    for (int i = 0; i < len; i++)
    {
       console_putchar(*(s+i)); 
    }

    // The cursor position has changed
    update_cursor();
}
