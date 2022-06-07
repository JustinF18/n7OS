#ifndef __TIME_H__
#define __TIME_H__

#define FREQUENCE 1193

/*
 * This is the function to initialize the timer (and scheduling)
 */
void init_timer();

/*
 * Return the current time
 */
int get_time();

#endif