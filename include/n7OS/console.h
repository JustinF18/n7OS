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

/*
 * This is the function called to update the screen time, it is displayed on the top left corner.
 */
void display_time(int h, int m, int s);

/*
 * This is the function called to clear the screen.
 * If start equals to 0 then all the screen is cleared.
 * If start equals to 1 then all the screen is cleared except row 0.
 */
void clear_display(int start);

/*
 * This is the function called to update the title of the console windows.
 */
void display_title(const char *s);

/*
 * This is the function called to display the running process on the top right corner.
 */
void display_current_process(int pid);

/*
 * This is the function called to display the bar at the top of the screen.
 */
void show_system_infobar();

#endif
