#uKernel (Micro Kernel)

## Introduction
This is a scheduler for micrcontrollers. This is not any kind of RTOS, or anything like it. Just create a funtion, create a descriptor for that funtion and added to the scheduler with a period and let the scheduler do the rest. There is no priority and I am tring to keep it realy simple due to the memory limitations of the micrcontrollers. The maximum number of task is 255 but I am sure that the memory will go out first. If anyone needs more tasks let me know.

## Versions
* V1.0 - Initial version - 03-05-2013

## Credits
This code was written by me, but I join two schedulers that I use, the tiny-kernel-microcontroller by Sébastien Pallatin ([here](https://code.google.com/p/tiny-kernel-microcontroller/)) and the leOS by Leonardo Miliani ([here](https://github.com/leomil72/leOS)).

Thank you both for the great job, all the hard work already done by you (both).

## Author
Written by Luis Maduro <luis.fmaduro@gmail.com>

