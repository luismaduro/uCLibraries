/**
 *  @file           uKernel.h
 *  @author         Luis Maduro
 *  @version        1.0
 *  @date           03/05/2013
 *  @copyright		GNU General Public License
 *
 *  @brief This is a scheduler for micrcontrollers. 
 *  This is not any kind of RTOS, or anything like it. 
 *  Just create a function, create a descriptor for that function and added to 
 *  the scheduler with a period and let the scheduler do the rest. There is no 
 *	priority and I am tring to keep it really simple due to the memory 
 *  limitations of micrcontrollers. The maximum number of task is 255 but I am
 *  sure that the memory will go out first. If anyone needs more tasks let me
 *  know.
 */

#ifndef UKERNEL_H
#define	UKERNEL_H

#ifdef	__cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_TASKS_NUMBER            255

/**Set your max interval here (max 2^32-1) - default 3600000 (1 hour)*/
#define MAX_TASK_INTERVAL           3600000UL

typedef enum
{
    /**For a task that doesn't have to start immediately.*/
    UKERNEL_PAUSED = 0x00, //0b00000000
    /**For a normal task that has to start after its scheduling*/
    UKERNEL_SCHEDULED = 0x01, //0b00000001
    /**For a task that has to run only once.*/
    UKERNEL_ONETIME = 0x02, //0b00000010
    /**For a task that has to be executed once it has been added.*/
    UKERNEL_IMMEDIATESTART = 0x05, //0b00000101
    /**For the task to be executed one time as soon as it is added.*/
    UKERNEL_ONETIME_IMMEDIATESTART = 0x07, //0b00000111
    /**Error, task not found.*/
    UKERNEL_ERROR = 0xFF //0b11111111
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
bool uKernelResumeTask(uKernelTaskDescriptor *pTaskDescriptor,
                       uKernelTaskStatus taskStatus);
bool uKernelModifyTask(uKernelTaskDescriptor *pTaskDescriptor,
                                uint32_t taskInterval,
                                uKernelTaskStatus tStatus);
void uKernelScheduler(void);
void uKernelDelayMiliseconds(unsigned int delay);

#ifdef	__cplusplus
}
#endif

#endif	/* UKERNEL_H */

