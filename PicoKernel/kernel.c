/**
 *  @file           kernel.c
 *  @author         Sebastien Pallatin
 *  @author         Luis Maduro
 *  @version        1.1
 *  @date           23/02/2013
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
#include <p18cxxx.h>
#include <stdio.h>
#include <limits.h>
#include "kernel.h"

/**Pointer of the scheduler*/
static TaskDescriptor *pTaskSchedule = NULL;
/**Timer for the scheduler*/
unsigned long usTickCount = 0;

/**
 * Return a delta of time
 * @param usTickVal Value of usTickCount when timer is started. To start timer,
 *                  call TickGet(0) with usTickVal=0 to set your timer variable
 *                  with the current usTickCount
 * @return Time elapsed since the timer starts
 */
unsigned short TickGet(unsigned short usTickVal)
{
    unsigned long usTmpResult;

    usTmpResult = usTickCount;

    if (usTmpResult >= usTickVal)
    {
        return (usTmpResult - usTickVal);
    }
    else
    {
        return (ULONG_MAX - usTickVal + usTmpResult);
    }
}

/**
 * Delete all task of the scheduler in the offing to reconfigure it. This
 * function call should be follow by AddTask() function to configure at least
 * one task, otherwise the scheduler will be blocked indefinitely with no task.
 */
void DeleteAllTask(void)
{
    AddTask(NULL, 0, NULL);
}

/**
 * Add a task into the circular linked list for the scheduler.
 * @param pTaskDescriptor   Descriptor of the task
 * @param usPeriod          Periodicity of the task
 * @param pTask             Function pointer on the task body
 */
void AddTask(TaskDescriptor *pTaskDescriptor, unsigned short usPeriod, TaskBody pTask)
{
    static TaskDescriptor *pTaskFirst = NULL;
    TaskDescriptor *pTaskWork = NULL;

    if (pTaskDescriptor != NULL)
    {
        if (pTaskFirst != NULL)
        {
            // Initialize the work pointer with the scheduler pointer (never mind is current state)
            pTaskWork = pTaskSchedule;
            // Research the last task from the circular linked list : so research the adresse of the
            // first task in testing the pTaskNext fields of each structure
            while (pTaskWork->pTaskNext != pTaskFirst)
            {
                // Set the work pointer on the next task
                pTaskWork = pTaskWork->pTaskNext;
            }
            pTaskWork->pTaskNext = pTaskDescriptor; // Insert the new task at the end of the circular linked list

            pTaskDescriptor->pTaskNext = pTaskFirst; // The next task is feedback at the first task
        }
        else
        {
            // There is no task in the scheduler, this task become the First task in the circular linked list
            pTaskFirst = pTaskDescriptor; // The pTaskFirst pointer is initialize with the first task of the scheduler
            pTaskSchedule = pTaskFirst; // Initialize the scheduler pointer at the first task
            pTaskDescriptor->pTaskNext = pTaskDescriptor; // The next task is itself because there is just one task in the circular linked list
        }
        // Common initialization for all tasks
        pTaskDescriptor->usElapse = TickGet(0u); // Initialize the task timer with the current value of usTickCount
        pTaskDescriptor->usPeriod = usPeriod; // Set the periodicity of the task
        pTaskDescriptor->pTask = pTask; // Set the task pointer on the task body
    }
    else
    {
        // Delete all task of the scheduler
        // This case is necessary at the time of the call of the function DeleteAllTask()
        pTaskFirst = NULL;
        pTaskSchedule = NULL;
    }
}

/**
 * Set the periodicity of the task
 * @param pTaskDescriptor   Descriptor of the task
 * @param usPeriod          Periodicity of the task
 */
void ResumeTask(TaskDescriptor *pTaskDescriptor, unsigned short usPeriod)
{
    pTaskDescriptor->usElapse = TickGet(0u); // Initialize the task timer with the current value of usTickCount
    pTaskDescriptor->usPeriod = usPeriod;
}

/**
 * Suspend a task
 * @param pTaskDescriptor Descriptor of the task
 */
void SuspendTask(TaskDescriptor *pTaskDescriptor)
{
    pTaskDescriptor->usPeriod = ULONG_MAX;
}

/**
 * Scheduling. This runs the kernel itself.
 */
void Scheduler(void)
{
    while (1)
    {
        if (pTaskSchedule != NULL)
        {
            if ((pTaskSchedule->usPeriod != 0xFFFFu) && (TickGet(pTaskSchedule->usElapse) >= pTaskSchedule->usPeriod))
            {
                // Initialize the task timer with the current value of usTickCount
                pTaskSchedule->usElapse = TickGet(0u);
                pTaskSchedule->pTask(); // Call the task body
            }
            // If a task has called the function DeleteAllTask() and if no task are added, the pointer is null
            if (pTaskSchedule != NULL)
            {
                // Set the scheduler pointer on the next task
                pTaskSchedule = pTaskSchedule->pTaskNext;
            }
        }
#ifdef USE_SLEEP
        SLEEP();
#endif
    }
}


