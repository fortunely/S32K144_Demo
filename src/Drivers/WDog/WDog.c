/*
 * WDog.c
 *
 *  Created on: 2019Äê6ÔÂ5ÈÕ
 *      Author: willing
 */
#include "WDog.h"
#include "S32K144.h"

void WDog_Disable (void)
{
  WDOG->CNT=0xD928C520;    /*Unlock watchdog*/
  WDOG->TOVAL=0x0000FFFF;  /*Maximum timeout value*/
  WDOG->CS = 0x00002100;   /*Disable watchdog*/
}

