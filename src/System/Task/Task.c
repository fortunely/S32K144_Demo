/**
* @filename main.c
* @author Martin
* @brief Task schedule module source file
* @date 2019-07-09
* @version 0.1
* @note initial draft
*/

#include "Task.h"
#include "S32K144.h"
#include "GPIO.h"
#include "LED.h"

// timer unit 1ms
#define TIMEROUT_1s           (1000)
#define LED_TIMEROUT          TIMEROUT_1s


uint32_t timer_1s = 0;

void Task_Led(void)
{
	Port_Toggle(PTD, PORT_LED_GREEN_IDX);
}

void Task_MainCycle(void)
{
	while(1)
	{
		if(timer_1s >= TIMEROUT_1s)
		{
			timer_1s = 0;

			LED_Task_1s();
		}
	}
}
