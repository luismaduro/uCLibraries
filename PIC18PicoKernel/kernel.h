/**
 *  @file           kernel.h
 *  @author         Sébastien Pallatin
 *  @author         Luis Maduro
 *  @version        1.1
 *  @date           23/02/2013
 *  @brief          Scheduler for microcontrollers
 *  @description    This projects was written in C programming language and it
 *                  is not an RTOS with a lot functions to manage registers,
 *                  priorities and so forth for microcontroller. But this
 *                  project is like a cooperative "RTOS" focusing on a tiny
 *                  tasks scheduling for ALL microcontrollers. You need to
 *                  configure just one timer (e.g. 1 millisecond, is a good
 *                  choice) to get a tiny tasks scheduler ready. Your only limit
 *                  is the microcontroller memory. This kernel can create a task
 *                  with a period, stop, resume and change a period of a task at
 *                  any time. This kernel can delete all task at any time to
 *                  create another sequencer as you want. A TickGet function is
 *                  supplied by the kernel to manage all timer as you want.
 *                  For use this source code in microcontroller you have to
 *                  create just one timer interrupt function and increment the
 *                  variable usTickCount to get your tiny-kernel for your
 *                  own application.
 *                  It is willingly written in a general way to help people to
 *                  customise for theirs owns applications. There are no
 *                  priority between task like a round-robin task scheduling.
 *
 *  Copyright (C) 2012  Sébastien Pallatin
 *  Copyright (C) 2012  Luis Maduro
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _KERNEL_H
#define _KERNEL_H

/**Function pointer on the task body.*/
typedef void (*TaskBody)(void);

typedef struct _TaskDescriptor
{
    /**Timer of the task*/
    unsigned long usElapse;
    /**Periodicity of the task*/
    unsigned long usPeriod;
    /**Function pointer on the task body*/
    TaskBody pTask;
    /**Pointer on the next structure task*/
    struct _TaskDescriptor *pTaskNext;
    /**Task Descriptor Structor*/
} TaskDescriptor;

extern unsigned long usTickCount;

unsigned short TickGet(unsigned short usTickVal);
void AddTask(TaskDescriptor *pTaskDescriptor, unsigned short usPeriod, TaskBody pTask);
void SuspendTask(TaskDescriptor *pTaskDescriptor);
void ResumeTask(TaskDescriptor *pTaskDescriptor, unsigned short usPeriod);
void Scheduler(void);
void DeleteAllTask(void);

#endif

