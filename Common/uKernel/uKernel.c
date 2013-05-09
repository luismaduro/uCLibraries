/**
 *  @file           uKernel.c
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
 *  sure that the memory will go out first. If anyone needs more tasks let me know.
 */
 
#include "uKernel.h"

uint8_t _initialized;
uint32_t _counterMs;
uint8_t numberTasks;
uKernelTaskDescriptor *pTaskSchedule;
static uKernelTaskDescriptor *pTaskFirst = NULL;

unsigned char uKernelSetTask(uKernelTaskDescriptor *pTaskDescriptor,
                             uint32_t taskInterval,
                             uKernelTaskStatus tStatus);

/**
 * This funtion as to be called before doing anything with the tasker. It
 * initiates the tasker subsystems. If this funtion is not called before doing
 * anything with the tasker all funtions will return false.
 */
void uKernelInit(void)
{
    _initialized = true;
    _counterMs = 0;
    numberTasks = 0;
    pTaskSchedule = NULL;
}

/**
 * Add a task into the circular linked list for the scheduler.
 * @param pTaskDescriptor   Descriptor of the task.
 * @param userTask          Function pointer on the task body
 * @param taskInterval Scheduled interval in milliseconds at which you want your
 *                     routine to be executed.
 * @param taskStatus Status of the task to be added to the scheduler, status can
 *                   be: PAUSED, for a task that doesn't have to start immediately;
 *                   SCHEDULED, for a normal task that has to start after its
 *                   scheduling; ONETIME, for a task that has to run only once;
 *                   IMMEDIATESTART, for a task that has to be executed once it
 *                   has been added to the scheduler; ONETIME_IMMEDIATESTART, for
 *                   a task that as to be executed now and it will only be
 *                   executed one time.
 * @return True or False
 * @see @uKernelTaskStatus
 */
bool uKernelAddTask(uKernelTaskDescriptor *pTaskDescriptor,
                    TaskBody userTask,
                    uint32_t taskInterval,
                    uKernelTaskStatus taskStatus)
{
    uKernelTaskDescriptor *pTaskWork = NULL;

    if ((_initialized == false) || (numberTasks == MAX_TASKS_NUMBER)
            || (userTask == NULL))
    {
        return false;
    }

    if ((taskInterval < 1) || (taskInterval > MAX_TASK_INTERVAL))
    {
        taskInterval = 50; //50 ms by default
    }

    //check if taskStatus is valid, if not schedule
    if (taskStatus > ONETIME_IMMEDIATESTART)
    {
        taskStatus = SCHEDULED;
    }

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
        // no wait if the user wants the task up and running once added...
        //...otherwise we wait for the interval before to run the task
        pTaskDescriptor->plannedTask =
                _counterMs + ((taskStatus & 0x04) ? 0 : taskInterval);

        // Set the periodicity of the task
        pTaskDescriptor->userTasksInterval = taskInterval;
        // Set the task pointer on the task body
        pTaskDescriptor->taskPointer = userTask;
        //I get only the first 2 bits - I don't need the IMMEDIATESTART bit
        pTaskDescriptor->taskStatus = taskStatus & 0x03;

        numberTasks++;

        return true;
    }
    else
    {
        // Delete all task of the scheduler
        // This case is necessary at the time of the call of the function DeleteAllTask()
        pTaskFirst = NULL;
        pTaskSchedule = NULL;

        return true;
    }
}

/**
 * This funtion is used to remove the task from the scheduler.
 * @param pTaskDescriptor Descriptor of the task to be removed.
 * @return Return true if all went well, false otherwise.
 */
bool uKernelRemoveTask(uKernelTaskDescriptor *pTaskDescriptor)
{
    uKernelTaskDescriptor *pTaskCurr = NULL;

    if ((_initialized == false) || (numberTasks == 0) ||
            pTaskDescriptor == NULL)
    {
        return false;
    }

    // Initialize the work pointer with the scheduler pointer (never mind is current state)
    pTaskCurr = pTaskFirst;

    while (pTaskCurr->pTaskNext != pTaskDescriptor)
    {
        // Set the work pointer on the next task
        pTaskCurr = pTaskCurr->pTaskNext;
    }

    if (pTaskCurr->pTaskNext == pTaskFirst)
    {
        pTaskFirst = pTaskCurr->pTaskNext->pTaskNext;
    }
    else
    {
        pTaskCurr->pTaskNext = pTaskDescriptor->pTaskNext;
    }

    numberTasks--;

    return true;
}

/**
 * This funtion is used to pause the task on the scheduler.
 * @param pTaskDescriptor Descriptor of the task to be paused.
 * @return Return true if all went well, false otherwise.
 */
bool uKernelPauseTask(uKernelTaskDescriptor *pTaskDescriptor)
{
    return (uKernelSetTask(pTaskDescriptor, NULL, PAUSED));
}

/**
 * This funtion is used to restart a funtion that has been paused.
 * @param pTaskDescriptor Descriptor of the task to be resumed.
 * @return Return true if all went well, false otherwise.
 */
bool uKernelResumeTask(uKernelTaskDescriptor *pTaskDescriptor)
{
    return (uKernelSetTask(pTaskDescriptor, NULL, SCHEDULED));
}

/**
 * Add a task into the circular linked list for the scheduler.
 * @param pTaskDescriptor   Descriptor of the task.
 * @param taskInterval Scheduled interval in milliseconds at which you want your
 *                     routine to be executed.
 * @param tStatus Status of the task to be added to the scheduler, status can
 *                   be: PAUSED, for a task that doesn't have to start immediately;
 *                   SCHEDULED, for a normal task that has to start after its
 *                   scheduling; ONETIME, for a task that has to run only once;
 *                   IMMEDIATESTART, for a task that has to be executed once it
 *                   has been added to the scheduler; ONETIME_IMMEDIATESTART, for
 *                   a task that as to be executed now and it will only be
 *                   executed one time.
 * @return True or False
 * @see @uKernelTaskStatus
 */
bool uKernelModifyTask(uKernelTaskDescriptor *pTaskDescriptor,
                          uint32_t taskInterval,
                          uKernelTaskStatus tStatus)
{
    if ((_initialized == false) || (numberTasks == MAX_TASKS_NUMBER)
            || (pTaskDescriptor == NULL))
    {
        return false;
    }

    if (tStatus > ONETIME_IMMEDIATESTART)
    {
        return false;
    }

    pTaskDescriptor->userTasksInterval = taskInterval;
    pTaskDescriptor->taskStatus = tStatus;

    if (tStatus == SCHEDULED || tStatus == ONETIME)
    {
        pTaskDescriptor->plannedTask = _counterMs + taskInterval;
    }
    else
    {
        pTaskDescriptor->plannedTask = 0;
    }

    return true;
}

/**
 * Funtion to check if a task is running.
 * @param userTask Task to check the status.
 * @return The status of the tasks.
 * @retval PAUSED Task is paused/not running.
 * @retval SCHEDULED Task is running.
 * @retval ONETIME Task is scheduled to run in a near future.
 * @retval ERROR There was an error (task not found)
 */
uKernelTaskStatus uKernelGetTaskStatus(uKernelTaskDescriptor *pTaskDescriptor)
{
    if ((_initialized == false) || (numberTasks == MAX_TASKS_NUMBER)
            || (pTaskDescriptor == NULL))
    {
        return ERROR;
    }

    return pTaskDescriptor->taskStatus;
}

/**
 * Scheduling. This runs the kernel itself.
 */
void uKernelScheduler(void)
{
    while (1)
    {
        if (pTaskSchedule != NULL && numberTasks != 0)
        {
            //the task is running
            if (pTaskSchedule->taskStatus > PAUSED)
            {
                //this trick overrun the overflow of _counterMs
                if ((long) (_counterMs - pTaskSchedule->plannedTask) >= 0)
                {
                    if (pTaskSchedule->taskStatus & ONETIME)
                    {
                        pTaskSchedule->taskPointer(); //call the task
                        pTaskSchedule->taskStatus = PAUSED; //pause the task
                    }
                    else
                    {
                        //let's schedule next start
                        pTaskSchedule->plannedTask =
                                _counterMs + pTaskSchedule->userTasksInterval;

                        pTaskSchedule->taskPointer(); //call the task
                    }
                }
            }
            // If a task has called the function DeleteAllTask() and if no
            // task are added, the pointer is null
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

/**
 * Just a simple delay in miliseconds. Not related to the Tasker system.
 */
void uKernelDelayMiliseconds(unsigned int delay)
{
    unsigned long newTime = _counterMs + delay;
    while (_counterMs < newTime);
}

unsigned char uKernelSetTask(uKernelTaskDescriptor *pTaskDescriptor,
                             uint32_t taskInterval,
                             uKernelTaskStatus tStatus)
{
    if ((_initialized == false) || (numberTasks == MAX_TASKS_NUMBER)
            || (pTaskDescriptor == NULL))
    {
        return false;
    }

    pTaskDescriptor->taskStatus = tStatus;

    if (tStatus == SCHEDULED)
    {
        if (taskInterval == NULL)
        {
            pTaskDescriptor->plannedTask =
                    _counterMs + pTaskDescriptor->userTasksInterval;
        }
        else
        {
            pTaskDescriptor->plannedTask =
                    _counterMs + taskInterval;
        }
    }



    return true;
}