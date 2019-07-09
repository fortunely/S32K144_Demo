/**
* @filename main.c
* @author Martin
* @brief Task schedule module header file
* @date 2019-07-09
* @version 0.1
*/

#ifndef SYSTEM_TASK_TASK_H_
#define SYSTEM_TASK_TASK_H_

#include <stdint.h>

typedef void Task_t(void);


extern uint32_t timer_1s;


void Task_MainCycle(void);

#endif /* SYSTEM_TASK_TASK_H_ */
