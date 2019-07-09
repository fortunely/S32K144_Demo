/**
* @filename Clock.c
* @author Martin
* @brief Clock module header file
* @date 2019-07-09
* @version 0.1
* @note initial draft
*/

#ifndef DRIVERS_CLOCK_CLOCK_H_
#define DRIVERS_CLOCK_CLOCK_H_
#include "S32K144.h"

typedef enum
{
	SIM_CLKOUTSEL_SCG_CLKOUT = 0b0000,
	SIM_CLKOUTSEL_SOSC_DIV2_CLK = 0b0010,
	SIM_CLKOUTSEL_SIRC_DIV2_CLK = 0b0100,
	SIM_CLKOUTSEL_FIRC_DIV2_CLK = 0b0110,
	SIM_CLKOUTSEL_HCLK = 0b0111,
	SIM_CLKOUTSEL_SPLL_DIV2_CLK = 0b1000, //For S32K14x
	SIM_CLKOUTSEL_BUS_CLK = 0b1001,
	SIM_CLKOUTSEL_LPO128K_CLK = 0b1010,
	SIM_CLKOUTSEL_LPO_CLK = 0b1100,
	SIM_CLKOUTSEL_RTC_CLK = 0b1110,
}SIM_CLKOUTSEL_Type;

static inline void scg_sirc_enable()
{
	SCG->SIRCCSR |= SCG_SIRCCSR_SIRCEN_MASK;  // Slow IRC Enable
}
static inline void scg_sirc_disable()
{
	SCG->SIRCCSR &= ~SCG_SIRCCSR_SIRCEN_MASK;  // Slow IRC disable
}

static inline void scg_firc_enable()
{
	SCG->FIRCCSR |= SCG_FIRCCSR_FIRCEN_MASK;  // Fast IRC Enable
}

static inline void scg_firc_disable()
{
	SCG->FIRCCSR &= ~SCG_FIRCCSR_FIRCEN_MASK;  // Fast IRC disable
}

static inline void scg_spll_enable()
{
	SCG->SPLLCSR |= SCG_SPLLCSR_SPLLEN_MASK;  // SPLL Enable
}

static inline void scg_spll_disable()
{
	SCG->SPLLCSR &= ~SCG_SPLLCSR_SPLLEN_MASK;  // Fast IRC disable
}

static inline void scg_sosc_enable()
{
	SCG->SOSCCSR |= SCG_SOSCCSR_SOSCEN_MASK;  // System OSC Enable
}

static inline void scg_sosc_disable()
{
	SCG->SOSCCSR &= ~SCG_SOSCCSR_SOSCEN_MASK;  // Fast IRC disable
}

void Clock_Init(void);
void Clock_InitRUN_80M(void);
void Clock_InitForHSRUNInRUN(void);
void Clock_InitVLPRInRUN(void);
void Clock_InitSPLLDIV_80M(void);
void Clock_InitSIRC(void);
void Clock_OutBus(void);
void Clock_OutSPLL(void);
void Clock_OutFIRC(void);
void Clock_OutSIRC(void);
void Clock_DisableSPLL(void);
void Clock_DisableFIRC(void);
void Clock_DisableSOSCInRUN();


#endif /* DRIVERS_CLOCK_CLOCK_H_ */
