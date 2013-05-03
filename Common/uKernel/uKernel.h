/**
 *  @file           uKernel.h
 *  @author         Luis Maduro
 *  @version        1.0
 *  @date           03/05/2013
 *  @copyright		GNU General Public License
 *
 *  @brief Scheduler for microcontrollers. 
 *  This projects was written in C programming language and it
 *  is not an RTOS with a lot functions to manage registers,
 *  priorities and so forth for microcontroller. But this
 *  project is like a cooperative "RTOS" focusing on a tiny
 *  tasks scheduling for ALL microcontrollers. You need to
 *  configure just one timer (e.g. 1 millisecond, is a good
 *  choice) to get a tiny tasks scheduler ready. Your only limit
 *  is the microcontroller memory. This kernel can create a task
 *  with a period, stop, resume and change a period of a task at
 *  any time. This kernel can delete all task at any time to
 *  create another sequencer as you want. A TickGet function is
 *  supplied by the kernel to manage all timer as you want.
 *  For use this source code in microcontroller you have to
 *  create just one timer interrupt function and increment the
 *  variable usTickCount to get your tiny-kernel for your
 *  own application.
 *  It is willingly written in a general way to help people to
 *  customise for theirs owns applications. There are no
 *  priority between task like a round-robin task scheduling.
 */

#ifndef UKERNEL_H
#define	UKERNEL_H

#ifdef	__cplusplus
extern "C"
{
#endif

#include <xc.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_TASKS_NUMBER            255

/**Set your max interval here (max 2^32-1) - default 3600000 (1 hour)*/
#define MAX_TASK_INTERVAL           3600000UL

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
    /**For the task to be executed one time as soon as it is added.*/
    ONETIME_IMMEDIATESTART = 0x07, //0b00000111
    /**Error, task not found.*/
    ERROR = 0xFF //0b11111111
} uKernelTaskStatus;

/**Function pointer on the task body.*/
typedef void (*TaskBody)(void);

typedef struct _uKernelTaskDescriptor
{
    //    /**Pointer to the previous task in the list.*/
    //    struct _uKernelTaskDescriptor *pTaskPrevious;
    /**Used to store the pointers to user's tasks*/
    TaskBody taskPointer;
    /**Used to store the interval between each task's run*/
    uint32_t userTasksInterval;
    /**Used to store the next time a task will have to be executed*/
    uint32_t plannedTask;
    /**Used to store the status of the tasks*/
    uKernelTaskStatus taskStatus;
    /**Pointer to the next task in the list.*/
    struct _uKernelTaskDescriptor *pTaskNext;
} uKernelTaskDescriptor;

extern uint32_t _counterMs;

void uKernelInit(void);
bool uKernelAddTask(uKernelTaskDescriptor *pTaskDescriptor,
                    void (*userTask)(void),
                    uint32_t taskInterval,
                    uKernelTaskStatus taskStatus);
bool uKernelRemoveTask(uKernelTaskDescriptor *userTaskDescriptor);
bool uKernelPauseTask(uKernelTaskDescriptor *pTaskDescriptor);
bool uKernelResumeTask(uKernelTaskDescriptor *pTaskDescriptor);
uint8_t uKernelModifyTask(uKernelTaskDescriptor *pTaskDescriptor,
                                uint32_t taskInterval,
                                uKernelTaskStatus tStatus);
void uKernelScheduler(void);
void uKernelDelayMiliseconds(unsigned int delay);

#ifdef	__cplusplus
}
#endif

#endif	/* UKERNEL_H */

