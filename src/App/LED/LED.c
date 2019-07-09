/**
* @filename main.c
* @author Martin
* @brief Task schedule module header file
* @date 2019-06-21
* @version 0.1
* @note initial draft
* EVB: S32K144EVB-Q100
*/
#include "LED.h"
#include "S32K144.h"
#include "GPIO.h"

void LED_Init()
{

}

void LED_On()
{
	Port_Clear(PTD, PORTD16_IDX);
}

void LED_Off()
{
	Port_Set(PTD, PORTD16_IDX);
}

void LED_Toggle()
{
	Port_Toggle(PTD, PORTD16_IDX);
}

void LED_Task_1s()
{
	LED_Toggle();
}
