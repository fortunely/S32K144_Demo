/*
 * RTC.c
 *
 *  Created on: 2019年6月4日
 *      Author: willing
 */
#include "RTC.h"
#include "S32K144.h"
//#include "interrupt_manager.h"
#include "../Interrupt/interrupt_manager.h"
#include "null.h"

void RTC_Interrupt(void);

/**
 * RTC初始化
 * @note 使能RTC时钟和中断, 选择
 */
void RTC_Init(void)
{

//	RTC_ClearTOFFlag();
//	RTC_ClearTIFFlag();

	// LPO Select -- 1 kHz LPO
//	RTC->CR |= RTC_CR_LPOS(0b1);
	RTC->CR &= ~RTC_CR_LPOS_MASK;

	// Time Overflow Interrupt Enable
	RTC_SetTOIE();

	// NVIC使能中断响应
	INT_SYS_EnableIRQ(RTC_IRQn);
//	INT_SYS_InstallHandler(RTC_IRQn, RTC_Interrupt, NULL);


	// 32 kHz clock source select -- 32 kHz LPO_CLK
	SIM->LPOCLKS |= SIM_LPOCLKS_RTCCLKSEL(0b01);

	// LPO clock source select -- 128 kHz LPO_CLK
	SIM->LPOCLKS |= SIM_LPOCLKS_LPOCLKSEL(0b00);

	// Enable RTC Counter
	RTC_EnableRTC();

}

volatile uint32_t rtcTIFCnt = 0;
volatile uint32_t rtcTOFCnt = 0;

void RTC_IRQHandler(void)
{
	if(RTC_GetTIFFlag())
	{
		RTC_ClearTIFFlag();

		rtcTIFCnt ++;
	}

	if(RTC_GetTOFFlag())
	{
		RTC_ClearTOFFlag();

		rtcTOFCnt ++;
	}
}
