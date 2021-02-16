/*
 * Sched.c
 *
 * Created: 1/18/2021 7:37:12 PM
 *  Author: Hatem
 */ 

#define F_CPU 8000000UL
#include <avr/interrupt.h>
#include <util/delay.h>
#include "std_macros.h"
#include "CapTim_Sch.h"

task sched_tasks_list[SCHED_MAX_TASKS];

/* ----------------------------------------------- *-
 Initialization function :
 main purpose to empty the array of tasks
 and to setup and run your timer
-*------------------------------------------------*/

void sched_init(void){
	
	char i;
	for(i=0; i<SCHED_MAX_TASKS;i++){
		sched_delete_task(i);
	}
	
	/*------------------------------------------------
	Reset the error flag as the delete function will
	generate an error when the array is empty which we
	don't need to know about (it's fine)
	------------------------------------------------*/
	err_c = 0;
	
	// we are working with AVR ATmega32 chip with a frequency of 8 MHz
	// Change the setup part if anything changed 
	
	
	// Select CTC Mode
	set_bit(TCCR1B,WGM12);
	
	// Load Value in OCR1A
	OCR1AH = 0b00011111;
	OCR1AL = 0b01000000;
	
	// Select Clock Prescaler
	set_bit(TCCR1B,CS10);
	 
	// Enable Interrupt
	set_bit(TIMSK,OCIE1A);
	
	
}


/* ----------------------------------------------- *-
 Update function :
 This is in the ISR to update functions
 that require execution.. Update happens
 every 1 ms due to Timer Settings..
-*------------------------------------------------*/

void sched_update(void){
	
	unsigned char i;
	for(i=0;i<SCHED_MAX_TASKS;i++){
		if(sched_tasks_list[i].pTast){
			if(sched_tasks_list[i].delay==0){
				//Task is due to run
				sched_tasks_list[i].tEN +=1;
				
				if(sched_tasks_list[i].period){
					sched_tasks_list[i].delay=sched_tasks_list[i].period;
				}
			}
			else{
				// not ready to run, yet !
				// decrement the delay
				sched_tasks_list[i].delay-=1;
			}
		}
	}
}


/* ----------------------------------------------- *-
 Add_Task function :
 This function is to add tasks and is used by user
 in the main function, so it's a public function.
-*------------------------------------------------*/

char sched_add_task(void(*pFunc)(),int delay,int period){
	unsigned char i=0;
	while((sched_tasks_list[i].pTast != 0) && (i < SCHED_MAX_TASKS)){
		i++;
	} 
	
	if(i == SCHED_MAX_TASKS){
		// set global error variable
		err_c = SCHED_TOO_MANY_TASKS;
		return SCHED_MAX_TASKS;
	}
	
	sched_tasks_list[i].pTast = pFunc;
	sched_tasks_list[i].delay = delay;
	sched_tasks_list[i].period = period;
	sched_tasks_list[i].tEN = 0;
	return i;
}


/* ----------------------------------------------- *-
 Dispatcher function :
 The Function is mainly one to make tasks (jobs)
 start working as it gets updates from the update
 function to start execution.. it's put in the main
 while(1) loop in the user function.
-*------------------------------------------------*/

void sched_dispatch(void){
	unsigned char i;
	for(i=0;i<SCHED_MAX_TASKS;i++){
		if(sched_tasks_list[i].tEN > 0){
			// Run Task
			(*sched_tasks_list[i].pTast)();
			sched_tasks_list[i].tEN = 0;
			
			if(sched_tasks_list[i].period == 0){
				sched_delete_task(i);
			}
		}
	}
	
	sched_report_status();
	sched_go_to_sleep();
}


/* ----------------------------------------------- *-
 Start function :
 This function is only to start working by
 enabling the global interrupt (sei).
-*------------------------------------------------*/

void sched_start(void){
	sei();
}


/* ----------------------------------------------- *-
 Delete_Task function :
 This function is to delete function and it's
 public for user for sure if there's an error
 return 1 , if not return 0.
 -*------------------------------------------------*/

char sched_delete_task(unsigned char i){
	char return_code;
	if(sched_tasks_list[i].pTast == 0){
		// No Task in this location
		err_c = SCHED_CANNOT_DELETE_TASK;
		return_code = RETURN_NORMAL;
	}
	else{
		return_code=RETURN_ERROR;
	}
	
	sched_tasks_list[i].pTast = 0;
	sched_tasks_list[i].delay = 0;
	sched_tasks_list[i].period = 0;
	sched_tasks_list[i].tEN = 0;
	
	return return_code;
}


/* ----------------------------------------------- *-
 Go To Sleep function :
 This function is to keep the CPU in the idle mode
 to save more power when it doesn't need to do any
 task at the moment..
 -*------------------------------------------------*/

void sched_go_to_sleep(void){
	
	MCUCR&=~(1<<SM0);
	MCUCR&=~(1<<SM1);
	MCUCR&=~(1<<SM2);
	
	MCUCR|=(1<<SE);	

}

/* ----------------------------------------------- *-
 Error Report function :
 This function is to tell when there's an error
 -*------------------------------------------------*/

void sched_report_status(void){
	
	/*---------------------------------------------
	There are two types of errors:
		- Add Task Error
		-  Can't Delete Error
	
	Say we have two LEDs connected to two pins.
	One is ON for 1000ms if Add Task errors occurs
	The other if ON for 1000ms for the Delete error
	---------------------------------------------*/
		
	#ifdef SCHED_REPORT_ERRORS
		// Initialization of LEDs Pins
		DDRD |= (1<<6)|(1<<7);		
		
		if(err_c == SCHED_TOO_MANY_TASKS){
			PORTD |= (1<<6);
			_delay_ms(1000);
			PORTD &= ~(1<<6);
			}
		if(err_c == SCHED_CANNOT_DELETE_TASK){
			PORTD |= (1<<7);
			_delay_ms(1000);
			PORTD &= ~(1<<7);
			}
		
	#endif
	
}

