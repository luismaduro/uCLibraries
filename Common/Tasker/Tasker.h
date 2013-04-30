/*
        Tasker.h - <l>ittle <e>mbedded <O>perating <S>ystem
        This is not a complete OS nor an RTOS as you usually know but
        it's a simple scheduler to schedule little works in background, so that
        you	can forget about them. It's designed for Arduino and other common
        Atmel microcontroller (for the complete list of the supported MCUs
    please refer to the README.txt file).

        Written by Leonardo Miliani <leonardo AT leonardomiliani DOT com>
    
    The latest version of this library can be found at:
    http://www.leonardomiliani.com/
    	
        Current version: 1.1.0 - 2013/03/15
    (for a complete history of the previous versions, see the README file)
    
        This library is free software; you can redistribute it and/or
        modify it under the terms of the GNU General Public
        License as published by the Free Software Foundation; either
        version 3.0 of the License, or (at your option) any later version.

        This library is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

 */


#ifndef TASKER_H
#define TASKER_H

#include <xc.h>
#include <stddef.h>
#include <stdbool.h>

/**Tasker version*/
#define TASKER_VERSION 112
/**Define here the maximum number of tasks to handle.*/
#define MAXIMUM_TASKS                   5
/**Set your max interval here (max 2^32-1) - default 3600000 (1 hour)*/
#define MAX_TASK_INTERVAL 3600000UL

#define TIMER_ON_BIT                T0CONbits.TMR0ON
#define TIMER_CON                   T0CONbits
#define TIMER_INT_ENABLE            INTCONbits.TMR0IE
#define TIMER_INT_PRIORITY          INTCON2bits.TMR0IP
#define TIMER_INT_FLAG              INTCONbits.TMR0IF
#define TIMER_HIGH_BYTE_REG         TMR0H
#define TIMER_LOW_BYTE_REG          TMR0L
#define TIMER_HIGH_BYTE_VALUE       0xF0
#define TIMER_LOW_BYTE_VALUE        0x5E

typedef enum
{
    /**For a task that doesn't have to start immediately.*/
    PAUSED = 0x00, //0b00000000
    /**For a normal task that has to start after its scheduling*/
    SCHEDULED = 0x01, //0b00000001
    /**For a task that has to run only once.*/
    ONETIME = 0x02, //0b00000010
    /**For a task that has to be executed once it has been added to the scheduler.*/
    IMMEDIATESTART = 0x05, //0b00000101
    /**Error, task not found.*/
    ERROR = 0xFF
} tTaskStatus;

typedef struct
{
    /**Used to store the pointers to user's tasks*/
    void (*taskPointer)(void);
    /**Used to store the interval between each task's run*/
    volatile unsigned long userTasksInterval;
    /**Used to store the next time a task will have to be executed*/
    volatile unsigned long plannedTask;
    /**Used to store the status of the tasks*/
    volatile unsigned char taskIsActive;
} TaskerCore;


/**
 * This funtion as to be called before doing anything with the tasker. It
 * initiates the tasker subsystems. If this funtion is not called before doing
 * anything with the tasker all funtions will return false.
 */
void TaskerBegin(void);
/**
 * This funtion is used to add tasks to your scheduler.
 * @param userTask Routine to be scheduled, and it has to be into your program.
 * @param taskInterval Scheduled interval in milliseconds at which you want your
 *                     routine to be executed.
 * @param taskStatus Status of the task to be added to the scheduler, status can
 *                   be: PAUSED, for a task that doesn't have to start immediately;
 *                   SCHEDULED, for a normal task that has to start after its
 *                   scheduling; ONETIME, for a task that has to run only once;
 *                   IMMEDIATESTART, for a task that has to be executed once it
 *                   has been added to the scheduler.
 * @return Return true if all went well, false otherwise.
 */
unsigned char TaskerAddTask(void (*userTask)(void),
                            unsigned long taskInterval,
                            tTaskStatus taskStatus);
/**
 * This funtion is used to remove the task from the scheduler.
 * @param userTask Routine to be removed.
 * @return Return true if all went well, false otherwise.
 */
unsigned char TaskerRemoveTask(void (*userTask)(void));
/**
 * This funtion is used to pause the task on the scheduler.
 * @param userTask Routine to be paused.
 * @return Return true if all went well, false otherwise.
 */
unsigned char TaskerPauseTask(void (*userTask)(void));
/**
 * This funtion is used to restart a funtion that has been paused.
 * @param userTask Routine to be restarted.
 * @return Return true if all went well, false otherwise.
 */
unsigned char TaskerResumeTask(void (*userTask)(void));
/**
 * This funtion is used to modify a task properties, for example the running
 * interval or the type of scheduling, i.e., PAUSED, SCHEDULED, ONETIME or
 * IMMEDIATESTART.
 * @param userTask Routine to be modified.
 * @param taskInterval Scheduled interval in milliseconds at which you want your
 *                     routine to be executed.
 * @param oneTimeTask Status of the task to be added to the scheduler, status can
 *                   be: PAUSED, for a task that doesn't have to start immediately;
 *                   SCHEDULED, for a normal task that has to start after its
 *                   scheduling; ONETIME, for a task that has to run only once;
 *                   IMMEDIATESTART, for a task that has to be executed once it
 *                   has been added to the scheduler.
 * @return Return true if all went well, false otherwise.
 */
unsigned char TaskerModifyTask(void (*userTask)(void),
                               unsigned long taskInterval,
                               tTaskStatus oneTimeTask);
/**
 * Funtion to check if a task is running.
 * @param userTask Task to check the status.
 * @return The status of the tasks.
 * @retval PAUSED Task is paused/not running.
 * @retval SCHEDULED Task is running.
 * @retval ONETIME Task is scheduled to run in a near future.
 * @retval ERROR There was an error (task not found)
 */
tTaskStatus TaskerGetTaskStatus(void (*userTask)(void));
/**
 * Stops the scheduler and freezes all the tasks preserving their current
 * intervals.
 */
void TaskerHaltScheduler(void);
/**
 * Restarts the scheduler resuming all the tasks that were running.
 */
void TaskerRestartScheduler(void);
/**
 * This funtion has to be called from the timer interrut routine.
 */
void TaskerTimerInterruptHandler(void);
/**
 * Scheduler of the tasker. This funtion has to be call to the tasker to work,
 * after all the initiation on the system.
 */
void TaskerScheduler(void);
/**Just a simple delay in miliseconds. Not related to the Tasker system.*/
void TakerDelayMiliseconds(unsigned int delay);
#endif
