#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "inttypes.h"

#define NROW 25
#define NCOL 80

/*
 * This is the function called by printf to send its output to the screen. You
 * have to implement it in the kernel and in the user program.
 */
void console_putbytes(const char *s, int len);

void set_pos(uint16_t pos);

uint16_t get_pos();

void set_cursor(uint8_t row, uint8_t col);

void display_time(int h, int m, int s);

void clear_display(int start);

void display_title(const char *s);

void display_current_process(int pid);

#endif
