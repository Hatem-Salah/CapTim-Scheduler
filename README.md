# Cooperative-Scheduler

This is a general cooperative scheduler suitable for the majority of
ordinary AVR microcontrollers. That project is made essentially for 
Real-Time Systems that needs task execution with some particular timing.

## Project Files

It consists of four files:

### 1. std_macros.h : 
  * It's a header file contains macros for C language
    operations like set, clear, toggle bit,... etc.

### 2. main.c :
  * It's the user part where the programmer can add tasks' functions
    and the (add task instructions) to go.
    
### 3. CapTim_Sch.h :
  * Header file that contains data and prototypes about the project
  
### 4. CapTim_Sch.c :
  * That file contains funtions for the scheduler.
  * It's fuly commented, so you can go and check it out.
  




Reference : Patterns for Time-Triggered Embedded Systems Book
