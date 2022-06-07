#include <n7OS/console.h>
#include <n7OS/cpu.h>
#include "inttypes.h"
#include <stdio.h>

static uint16_t pos = 0;
uint8_t *scr_tab = (uint8_t *)0xB8000;
int infobar_is_displayed = 0;
int default_style = 0x0F;
int infobar_style = 0x1F;
int style = 0x0F;

void update_cursor()
{
    outb(0xF, 0x3D4);
    outb(pos & 0xFF, 0x3D5);
    outb(0xE, 0x3D4);
    outb(pos >> 8, 0x3D5);
}

void set_pos(uint16_t newpos)
{
    pos = newpos;
}

void set_cursor(uint8_t row, uint8_t col)
{
    pos = NCOL * row + col;
    update_cursor();
}

void console_putchar(const char c)
{
    // Check if the screen is full
    if (pos > NROW * NCOL)
    {
        // Move all content up
        // Only the first row stay
        for (int i = NCOL; i < NROW * NCOL; i++)
        {
            scr_tab[i * 2] = scr_tab[(i + NCOL) * 2];
            scr_tab[i * 2 + 1] = scr_tab[(i + NCOL) * 2 + 1];
        }
        pos = (NROW - 1) * NCOL + 1;
    }

    if (c > 31 && c < 127)
    {
        // Print character on screen
        scr_tab[pos * 2] = c;
        // Color brackets
        if (c == '[' || c == ']' || c == '{' || c == '}')
        {
            scr_tab[pos * 2 + 1] = 0x0E;
        }
        else if (c == '(' || c == ')')
        {
            scr_tab[pos * 2 + 1] = 0x04;
        }
        else
        {
            scr_tab[pos * 2 + 1] = style;
        }
        pos += 1;
    }
    else if (c == '\b')
    {
        // Move cursor back
        pos -= 1;
    }
    else if (c == '\t')
    {
        // Add a tab : 8 character space
        pos += 8;
    }
    else if (c == '\n')
    {
        // Move the cursor to the next line, first column
        pos = ((pos / NCOL) + 1) * NCOL;
    }
    else if (c == '\f')
    {
        // Clean the screen and move the cursor to second row and first column
        // We don't clear the first row because it's reserved to display system info
        // like time, scheduler info, ...
        clear_display(1);
    }
    else if (c == '\r')
    {
        // Move the cursor at the row start
        pos = (pos / NCOL) * NCOL;
    }
}

void console_putbytes(const char *s, int len)
{
    for (int i = 0; i < len; i++)
    {
        console_putchar(*(s + i));
    }

    // The cursor position has changed
    update_cursor();
}

void clear_block_display(int start, int end)
{
    // Reset display
    for (int i = start * NCOL; i < NCOL * end; i++)
    {
        scr_tab[i * 2] = ' ';
        scr_tab[i * 2 + 1] = style;
    }
}

void clear_display(int start)
{
    clear_block_display(start, NROW);
    pos = NCOL;
}

void display_time(int h, int m, int s)
{
    if (infobar_is_displayed == 1)
    {
        style = infobar_style;
        // Save the current cursor position
        uint16_t old_pos = pos;
        // Put the cursor a the top right corner
        set_pos(72);
        // Print hour
        printf("%02d:%02d:%02d", h % 24, m % 60, s % 60);
        // Return at the old position
        set_cursor(old_pos / NCOL, old_pos % NCOL);
        style = default_style;
    }
}

void display_title(const char *s)
{
    if (infobar_is_displayed == 1)
    {
        style = infobar_style;
        // Save the current cursor position
        uint16_t old_pos = pos;
        // Put the cursor a the middle on the top
        set_pos(30);
        // Print hour
        printf(s);
        // Return at the old position
        set_cursor(old_pos / NCOL, old_pos % NCOL);
        style = default_style;
    }
}

void display_current_process(int pid)
{
    if (infobar_is_displayed == 1)
    {
        style = infobar_style;
        // Save the current cursor position
        uint16_t old_pos = pos;
        // Put the cursor a the middle on the top
        set_pos(0);
        // Print hour
        printf("Processus actif : %d", pid);
        // Return at the old position
        set_cursor(old_pos / NCOL, old_pos % NCOL);
        style = default_style;
    }
}

void show_system_infobar()
{
    // Clear bar and set style
    style = infobar_style;
    clear_block_display(0, 1);
    style = default_style;
    // Show bar
    infobar_is_displayed = 1;
}