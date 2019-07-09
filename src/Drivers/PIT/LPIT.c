/*
 * PIT.c
 *
 *  Created on: 2019年6月5日
 *      Author: willing
 */
#include "S32k144.h"
#include "GPIO.h"
#include "interrupt_manager.h"
#include "LPIT.h"

static inline void LPIT0_ResetBySoftware()
{
	LPIT0->MCR |= LPIT_MCR_SW_RST_MASK;
}

static inline void LPIT0_ClearResetBySoftware()
{
	LPIT0->MCR &= ~LPIT_MCR_SW_RST_MASK;
}

static inline void LPIT0_Disable()
{
	LPIT0->MCR &= ~LPIT_MCR_M_CEN_MASK;
}

static inline void LPIT0_Enable()
{
	LPIT0->MCR |= ~LPIT_MCR_M_CEN_MASK;
}
/**
 * 初始化RUN模式下LPIT0
 * @require SPLLDIV2_CLK = 80M
 * @note 1ms定时中断
 */
void LPIT0_Init(void)
{
//	LPIT0_Disable();
//	LPIT0_ResetBySoftware();

	INT_SYS_ClearPending(LPIT0_Ch0_IRQn);
	INT_SYS_EnableIRQ(LPIT0_Ch0_IRQn);
	INT_SYS_SetPriority(LPIT0_Ch0_IRQn, 0xA0);

	/* init LPIT0 module*/
	if(PCC->PCCn[PCC_LPIT_INDEX] & PCC_PCCn_PR_MASK)
	{// the peripheral is present on this device
		PCC->PCCn[PCC_LPIT_INDEX] = PCC_PCCn_PCS(0b110); // 0b110: LPIT clock source = SPLLDIV2_CLK
		PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_CGC_MASK;  //1: Clock enabled and locked

		LPIT0->MCR = 0x00000001; // Enable module LPIT0
		LPIT0->MIER = 0x00000001; //Channel 0 Timer Interrupt Enable
		LPIT0->TMR[0].TVAL = 80000; // TVAL0=80K//TVAL0=80M: timer ch0 start value = 80M
		LPIT0->TMR[0].TCTRL = 0x00000001;//T_EN=1: Enables the Timer Channel
	}
}


/**
 * 配置VLPR模式下的参数, 时钟源必须选择SIRC
 * @require SIRCDIV2_CLK = 4M
 * @note 1s定时中断
 */
void LPIT0_SwitchToSIRCInRUN(void)
{
	INT_SYS_ClearPending(LPIT0_Ch0_IRQn);
	INT_SYS_EnableIRQ(LPIT0_Ch0_IRQn);
	INT_SYS_SetPriority(LPIT0_Ch0_IRQn, 0xA0);

	/* init LPIT0 module*/
	if(PCC->PCCn[PCC_LPIT_INDEX] & PCC_PCCn_PR_MASK)
	{// the peripheral is present on this device
		PCC->PCCn[PCC_LPIT_INDEX] &= ~PCC_PCCn_CGC_MASK; // clear Clock Gate Control
		PCC->PCCn[PCC_LPIT_INDEX] = PCC_PCCn_PCS(0b010); // 0b010: LPIT clock source = SIRCDIV2_CLK@4M, require CGS=0(disabled)
		PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_CGC_MASK;  // set Clock Gate Control

//		LPIT0_ResetBySoftware();

		LPIT0->MCR = 0x00000001; // Enable module LPIT0, M_CEN使能必须在访问LPIT0其他寄存器之前
		LPIT0->MIER = 0x00000001; //Channel 0 Timer Interrupt Enable
		LPIT0->TMR[0].TVAL = 2000; //2K -> PIT_timer_freq = CLK_SOURCE / TVAL = 4M / 2K = 2K Hz

		// disable timer channel 0
		LPIT0->CLRTEN |= LPIT_CLRTEN_CLR_T_EN_0_MASK; // Write 1 to LPIT_CLRTEN to clear Clear Timer 0 Enable

		// write LPIT0_TCTRL0 when disable timer channel
		LPIT0->TMR[0].TCTRL = 0x00000001;//T_EN=1: Enables the Timer Channel

		// enable timer channel 0
		LPIT0->SETTEN |= LPIT_SETTEN_SET_T_EN_0_MASK; // Write 1 to LPIT_SETTEN to set Timer 0 Enable

//		LPIT0_ClearResetBySoftware();
	}
}

uint32_t lpitCounter = 0;
void LPIT0_Ch0_IRQHandler(void)
{
	extern uint32_t timer_1s;

	LPIT0->MSR |= LPIT_MSR_TIF0_MASK; //LPIT0_MSR[TIF0]: Clear TIF0

	lpitCounter ++;

	timer_1s ++;

//	Port_Toggle(PTD, PORT_LED_GREEN_IDX);
}
