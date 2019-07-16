/*
 * Init.c
 *
 *  Created on: 2019��5��29��
 *      Author: willing
 */

#include "GPIO.h"
#include "Init.h"
#include "Clock.h"

/**
 * System initialization
 */
void Init()
{
	Clock_Init();
	Port_Init();
}
