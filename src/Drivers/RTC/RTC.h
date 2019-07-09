/*
 * RTC.h
 *
 *  Created on: 2019Äê6ÔÂ4ÈÕ
 *      Author: willing
 */

#ifndef DRIVERS_RTC_RTC_H_
#define DRIVERS_RTC_RTC_H_

#include "S32K144.h"

static inline void RTC_EnableRTC()
{
	RTC->SR |= RTC_SR_TCE_MASK;
}

static inline void RTC_DisableRTC()
{
	RTC->SR &= ~RTC_SR_TCE_MASK;
}

static inline uint8_t RTC_GetTOFFlag()
{
	return (uint8_t)(RTC->SR & RTC_SR_TOF_MASK);
}

static inline void RTC_ClearTOFFlag()
{
	uint8_t RTCEnableFlag = RTC->SR & RTC_SR_TCE_MASK;

	if(RTCEnableFlag)
	{
		RTC_DisableRTC();
	}

	// Write 1 to SR_TOF
	RTC->SR |= RTC_SR_TOF_MASK;

	if(RTCEnableFlag)
	{
		RTC_EnableRTC();
	}
}

static inline uint8_t RTC_GetTIFFlag()
{
	return (uint8_t)(RTC->SR & RTC_SR_TIF_MASK);
}

static inline void RTC_ClearTIFFlag()
{
	uint8_t RTCEnableFlag = RTC->SR & RTC_SR_TCE_MASK;

	if(RTCEnableFlag)
	{
		RTC_DisableRTC();
	}

	// Write 1 to SR_TIF
	RTC->SR |= RTC_SR_TIF_MASK;

	if(RTCEnableFlag)
	{
		RTC_EnableRTC();
	}
}

static inline void RTC_SetTOIE()
{
	RTC->IER |= RTC_IER_TOIE_MASK;
}

static inline void RTC_ClearTOIE()
{
	RTC->IER &= ~RTC_IER_TOIE_MASK;
}


void RTC_Init(void);

#endif /* DRIVERS_RTC_RTC_H_ */
