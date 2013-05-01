/* This file is part of Tasker library.
   Please check the README file and the notes
   inside the Tasker.h file
 */

//include required libraries
#include "Tasker.h"

volatile unsigned long _counterMs = 0;
volatile unsigned char _initialized = 0;
volatile unsigned char numberTasks;
TaskerCore Tasks[MAXIMUM_TASKS];

void TaskerSetTimer();
unsigned char TaskerSetTask(void (*)(void),
                            unsigned char,
                            unsigned long taskInterval);

void TaskerBegin(void)
{
    TaskerSetTimer();
    _initialized = true;
    numberTasks = 0;
}

void TaskerSetTimer(void)
{
    //Timer 0 @ 1ms to Kernel (8MHz)
    TIMER_CON.T08BIT = 0;
    TIMER_CON.T0CS = 0;
    TIMER_CON.T0SE = 0;
    TIMER_CON.PSA = 0;
    TIMER_CON.T0PS = 0b000;

    TIMER_HIGH_BYTE_REG = TIMER_HIGH_BYTE_VALUE;
    TIMER_LOW_BYTE_REG = TIMER_LOW_BYTE_VALUE;

    TIMER_INT_FLAG = 0;
    TIMER_INT_PRIORITY = 0;
    TIMER_INT_ENABLE = 1;
    TIMER_CON.TMR0ON = 1;
}

void TaskerHaltScheduler(void)
{
    TIMER_ON_BIT = 0;
}

void TaskerRestartScheduler(void)
{
    if (_initialized)
    {
        TIMER_ON_BIT = 1;
    }
}

unsigned char TaskerAddTask(void (*userTask)(void),
                            unsigned long taskInterval,
                            tTaskStatus taskStatus)
{
    if ((_initialized == false) || (numberTasks == MAXIMUM_TASKS))
    {
        //max number of allowed tasks reached
        return false;
    }

    if ((taskInterval < 1) || (taskInterval > MAX_TASK_INTERVAL))
    {
        taskInterval = 50; //50 ms by default
    }

    //check if taskStatus is valid, if not schedule
    if (taskStatus > IMMEDIATESTART)
    {
        taskStatus = SCHEDULED;
    }

    //add the task to the scheduler
    TaskerHaltScheduler(); //halt the scheduler

    Tasks[numberTasks].taskPointer = *userTask;
    //I get only the first 2 bits - I don't need the IMMEDIATESTART bit
    Tasks[numberTasks].taskIsActive = taskStatus & 0x03;
    Tasks[numberTasks].userTasksInterval = taskInterval;
    //no wait if the user wants the task up and running once added...
    //...otherwise we wait for the interval before to run the task
    Tasks[numberTasks].plannedTask =
            _counterMs + ((taskStatus & 0x04) ? 0 : taskInterval);

    numberTasks++;

    TaskerRestartScheduler(); //restart the scheduler

    return true;
}

unsigned char TaskerPauseTask(void (*userTask)(void))
{
    return (TaskerSetTask(userTask, PAUSED, NULL));
}

unsigned char TaskerResumeTask(void (*userTask)(void))
{
    return (TaskerSetTask(userTask, SCHEDULED, NULL));
}

unsigned char TaskerModifyTask(void (*userTask)(void),
                               unsigned long taskInterval,
                               tTaskStatus oneTimeTask)
{
    unsigned char tempI = 0;
    unsigned char _done = 1;

    if ((oneTimeTask < SCHEDULED) && (oneTimeTask > ONETIME))
    {
        oneTimeTask = NULL;
    }

    //modify the task into the scheduler
    TaskerHaltScheduler(); //halt the scheduler

    do
    {
        if (Tasks[tempI].taskPointer == *userTask)
        { //task found
            Tasks[tempI].userTasksInterval = taskInterval;
            if (oneTimeTask != NULL)
            {
                Tasks[tempI].taskIsActive = oneTimeTask;
            }
            Tasks[tempI].plannedTask = _counterMs + taskInterval;
            _done = 0;
            break;
        }
        tempI++;
    }
    while (tempI < numberTasks);

    TaskerRestartScheduler(); //restart the scheduler

    return _done;
}

unsigned char TaskerSetTask(void (*userTask)(void),
                            unsigned char tempStatus,
                            unsigned long taskInterval)
{
    unsigned char tempI = 0;

    if ((_initialized == 0) || (numberTasks == 0))
    {
        return false;
    }

    TaskerHaltScheduler(); //halt the scheduler

    do
    {
        if (Tasks[tempI].taskPointer == *userTask)
        {
            Tasks[tempI].taskIsActive = tempStatus;
            if (tempStatus == SCHEDULED)
            {
                if (taskInterval == NULL)
                {
                    Tasks[tempI].plannedTask =
                            _counterMs + Tasks[tempI].userTasksInterval;
                }
                else
                {
                    Tasks[tempI].plannedTask =
                            _counterMs + taskInterval;
                }
            }
            break;
        }
        else
        {
            tempI++;
        }
    }
    while (tempI < numberTasks);

    TaskerRestartScheduler(); //restart the scheduler

    return true;
}

unsigned char TaskerRemoveTask(void (*userTask)(void))
{
    unsigned char tempI = 0, tempJ;

    if ((_initialized == 0) || (numberTasks == 0))
    {
        return false;
    }

    TaskerHaltScheduler(); //halt the scheduler

    do
    {
        if (Tasks[tempI].taskPointer == *userTask)
        {
            if ((tempI + 1) == numberTasks)
            {
                numberTasks--;
            }
            else if (numberTasks > 1)
            {
                for (tempJ = tempI; tempJ < numberTasks; tempJ++)
                {
                    Tasks[tempJ].taskPointer = Tasks[tempJ + 1].taskPointer;
                    Tasks[tempJ].taskIsActive = Tasks[tempJ + 1].taskIsActive;
                    Tasks[tempJ].userTasksInterval =
                            Tasks[tempJ + 1].userTasksInterval;
                    Tasks[tempJ].plannedTask = Tasks[tempJ + 1].plannedTask;
                }
                numberTasks--;
            }
            else
            {
                numberTasks = 0;
            }
            break;
        }
        else
        {
            tempI++;
        }
    }
    while (tempI < numberTasks);

    TaskerRestartScheduler(); //restart the scheduler

    return true;
}

tTaskStatus TaskerGetTaskStatus(void (*userTask)(void))
{
    //return 255 if the task was not found (almost impossible)
    unsigned char tempJ = ERROR;
    unsigned char tempI = 0;

    if ((_initialized == false) || (numberTasks == 0))
    {
        return false;
    }


    TaskerHaltScheduler(); //halt the scheduler
    //look for the task

    do
    {
        if (Tasks[tempI].taskPointer == *userTask)
        {
            //return its current status
            tempJ = Tasks[tempI].taskIsActive;
            break;
        }
        tempI++;
    }

    while (tempI < numberTasks);

    TaskerRestartScheduler(); //restart the scheduler

    return tempJ; //return the task status
}

void TaskerTimerInterruptHandler(void)
{
    TIMER_HIGH_BYTE_REG = TIMER_HIGH_BYTE_VALUE;
    TIMER_LOW_BYTE_REG = TIMER_LOW_BYTE_VALUE;

    TIMER_INT_FLAG = 0;

    _counterMs++; //increment the ms counter
}

void TaskerScheduler(void)
{
    unsigned char tempI = 0, tempJ = 0;

    while (true)
    {
        if (Tasks[tempI].taskIsActive > 0 && TIMER_ON_BIT == 1)
        { //the task is running
            //check if it's time to execute the task
            if ((long) (_counterMs - Tasks[tempI].plannedTask) >= 0)
            { //this trick overrun the overflow of _counterMs

                //if it's a one-time task, than it has to be removed after running
                if (Tasks[tempI].taskIsActive == ONETIME)
                {
                    Tasks[tempI].taskPointer(); //call the task

                    if ((tempI + 1) == numberTasks)
                    {
                        numberTasks--;
                    }
                    else if (numberTasks > 1)
                    {
                        for (tempJ = tempI; tempJ < numberTasks; tempJ++)
                        {
                            Tasks[tempJ].taskPointer =
                                    Tasks[tempJ + 1].taskPointer;
                            Tasks[tempJ].taskIsActive =
                                    Tasks[tempJ + 1].taskIsActive;
                            Tasks[tempJ].userTasksInterval =
                                    Tasks[tempJ + 1].userTasksInterval;
                            Tasks[tempJ].plannedTask =
                                    Tasks[tempJ + 1].plannedTask;
                        }
                        numberTasks--;
                    }
                    else
                    {
                        numberTasks = 0;
                    }
                }
                else
                {
                    //let's schedule next start
                    Tasks[tempI].plannedTask =
                            _counterMs + Tasks[tempI].userTasksInterval;

                    Tasks[tempI].taskPointer(); //call the task
                }
            }
        }

        tempI++;

        if (tempI == numberTasks)
            tempI = 0;
    }
}

void TakerDelayMiliseconds(unsigned int delay)
{
    unsigned long newTime = _counterMs + delay;
    while (_counterMs <= newTime);
}