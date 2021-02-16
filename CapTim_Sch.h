/*
 * Sched.h
 *
 * Created: 1/18/2021 7:37:27 PM
 *  Author: Hatem
 */ 


#ifndef SCHED_H_
#define SCHED_H_

// Comment this line out if error reporting is NOT required
#define SCHED_REPORT_ERRORS

#define SCHED_MAX_TASKS 1
/*----------------------------------------------------------*/
/*----------------------------------------------------------*/
// Important Macros

#define RETURN_NORMAL 0
#define RETURN_ERROR  1
#define SCHED_TOO_MANY_TASKS 0
#define SCHED_CANNOT_DELETE_TASK 1

/*----------------------------------------------------------*/
/*----------------------------------------------------------*/
// Data and Variables needed

unsigned char err_c;

typedef struct tasks{
	
	// Pointer to the task function (Must be void)
	void (*pTast)(void);
	
	int delay;
	int period;
	char tEN; // Task Enable (RUN ME)
	
	}task;
	
/*---------------------------------------------------------*/
/*---------------------------------------------------------*/
// Functions used for scheduling	
	
void sched_init(void);
void sched_update(void);
char sched_add_task(void(*pFunc)(),int delay,int period);
void sched_dispatch(void);
void sched_start(void);
char sched_delete_task(unsigned char i);
void sched_go_to_sleep(void);
void sched_report_status(void);


#endif /* SCHED_H_ */