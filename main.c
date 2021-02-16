/*
 * CapTIM Scheduler.cpp
 *
 * Created: 2/15/2021 10:05 AM
 * Author : Hatem
 */ 

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "std_macros.h"
#include "CapTim_Sch.h"



ISR(TIMER1_COMPA_vect){
	sched_update();
}


/*--------------------------------*/
// Put functions for your tasks here
// Or you can make a separate file for them.
/*--------------------------------*/
// that's an example function for a LED
// to toggle every ONE ms.

void led_init(){
	// making the led pin an O/P pin.
	DDRC |= (1<<1);
}

void led_task(){
	// Toggle the led whenever the task is executed.
	tog_bit(PORTC,1);
}


int main(void)
{
	
	// Setup the Scheduler
	sched_init();
	
	// if task has an initialization task
	// initialize it here
	
	led_init();
	
	// Add the 'task_name' task
	// - timings are in ticks (1 ms tick interval)
	// [Example] (every ~1000 ms)
	sched_add_task(led_task, 0, 1000);
	
	// Start the Scheduler
	sched_start();
	
    while (1) 
    {
		sched_dispatch();
    }
}

