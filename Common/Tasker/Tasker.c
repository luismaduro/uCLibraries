/* This file is part of Tasker library.
   Please check the README file and the notes
   inside the Tasker.h file
 */

//include required libraries
#include "Tasker.h"

volatile uint32_t _counterMs = 0;
volatile uint8_t _initialized = 0;
volatile uint8_t _numberTasks;

TaskerCore Tasks[MAXIMUM_TASKS];

void TaskerSetTimer();
uint8_t TaskerSetTask(void (*)(void),
                      uint8_t,
                      uint32_t taskInterval);

void TaskerBegin(void)
{
    _initialized = true;
    _numberTasks = 0;
    _counterMs = 0;
}

uint8_t TaskerAddTask(void (*userTask)(void),
                      uint32_t taskInterval,
                      tTaskStatus taskStatus)
{
    if ((_initialized == false) || (_numberTasks == MAXIMUM_TASKS))
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

    Tasks[_numberTasks].taskPointer = *userTask;
    //I get only the first 2 bits - I don't need the IMMEDIATESTART bit
    Tasks[_numberTasks].taskIsActive = taskStatus & 0x03;
    Tasks[_numberTasks].userTasksInterval = taskInterval;
    //no wait if the user wants the task up and running once added...
    //...otherwise we wait for the interval before to run the task
    Tasks[_numberTasks].plannedTask =
            _counterMs + ((taskStatus & 0x04) ? 0 : taskInterval);

    _numberTasks++;

    return true;
}

uint8_t TaskerPauseTask(void (*userTask)(void))
{
    return (TaskerSetTask(userTask, PAUSED, NULL));
}

uint8_t TaskerResumeTask(void (*userTask)(void))
{
    return (TaskerSetTask(userTask, SCHEDULED, NULL));
}

uint8_t TaskerModifyTask(void (*userTask)(void),
                         uint32_t taskInterval,
                         tTaskStatus oneTimeTask)
{
    uint8_t tempI = 0;
    uint8_t _done = 1;

    if ((oneTimeTask < SCHEDULED) && (oneTimeTask > ONETIME))
    {
        oneTimeTask = NULL;
    }

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
    while (tempI < _numberTasks);

    return _done;
}

uint8_t TaskerSetTask(void (*userTask)(void),
                      uint8_t tempStatus,
                      uint32_t taskInterval)
{
    uint8_t tempI = 0;

    if ((_initialized == 0) || (_numberTasks == 0))
    {
        return false;
    }

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
    while (tempI < _numberTasks);

    return true;
}

uint8_t TaskerRemoveTask(void (*userTask)(void))
{
    uint8_t tempI = 0, tempJ;

    if ((_initialized == 0) || (_numberTasks == 0))
    {
        return false;
    }

    do
    {
        if (Tasks[tempI].taskPointer == *userTask)
        {
            if ((tempI + 1) == _numberTasks)
            {
                _numberTasks--;
            }
            else if (_numberTasks > 1)
            {
                for (tempJ = tempI; tempJ < _numberTasks; tempJ++)
                {
                    Tasks[tempJ].taskPointer = Tasks[tempJ + 1].taskPointer;
                    Tasks[tempJ].taskIsActive = Tasks[tempJ + 1].taskIsActive;
                    Tasks[tempJ].userTasksInterval =
                            Tasks[tempJ + 1].userTasksInterval;
                    Tasks[tempJ].plannedTask = Tasks[tempJ + 1].plannedTask;
                }
                _numberTasks--;
            }
            else
            {
                _numberTasks = 0;
            }
            break;
        }
        else
        {
            tempI++;
        }
    }
    while (tempI < _numberTasks);

    return true;
}

tTaskStatus TaskerGetTaskStatus(void (*userTask)(void))
{
    //return 255 if the task was not found (almost impossible)
    uint8_t tempJ = ERROR;
    uint8_t tempI = 0;

    if ((_initialized == false) || (_numberTasks == 0))
    {
        return false;
    }

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

    while (tempI < _numberTasks);

    return tempJ; //return the task status
}

void TaskerTimerInterruptHandler(void)
{
    _counterMs++; //increment the ms counter
}

void TaskerScheduler(void)
{
    uint8_t tempI = 0;

    while (1)
    {
        if (Tasks[tempI].taskIsActive > 0)
        { //the task is running
            //check if it's time to execute the task
            if ((long) (_counterMs - Tasks[tempI].plannedTask) >= 0)
            { //this trick overrun the overflow of _counterMs

                //if it's a one-time task, than it has to be removed after running
                if (Tasks[tempI].taskIsActive == ONETIME)
                {
                    Tasks[tempI].taskPointer(); //call the task
                    Tasks[tempI].taskIsActive = PAUSED;
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

        if (tempI >= _numberTasks)
            tempI = 0;
    }
}

void TaskerDelayMiliseconds(uint16_t delay)
{
    uint32_t newTime = _counterMs + delay;
    while (_counterMs < newTime);
}