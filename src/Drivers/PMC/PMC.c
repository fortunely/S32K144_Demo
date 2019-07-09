/*
 * PMC.c
 * @note Power manager controller module
 *  Created on: 2019年6月3日
 *      Author: willing
 */
#include "PMC.h"
#include "S32K144.h"
#include "Clock.h"
#include "LPIT.h"

#define HSRUN_112M              1
#define HSRUN_80M               2
#define HSRUN_CORE_FREQ         HSRUN_80M

/**
 * disable clock monitor
 * @note S32K11x才有clock monitor(CMU)
 */
void disable_clock_monitors()
{
	SCG->SPLLCSR &= ~SCG_SPLLCSR_SPLLCM_MASK;  // disable spll clock monitor
}

void enable_clock_monitors()
{
	SCG->SPLLCSR |= SCG_SPLLCSR_SPLLCM_MASK;  // enable spll clock monitor
}

// 配置SPLL 80M
void scg_configure_spll()
{
}

/**
 * 配置VLPR系统时钟 4M
 * @require RUN模式下进行配置
 */

void scg_configure_freq_for_VLPR()
{
	Clock_InitVLPRInRUN(); // 配置VLPR模式下的时钟参数

	Clock_InitSIRC(); // 配置SIRC

//	Clock_Out_SIRC();  // 设置CLKOUT pin输出SIRC DIV2 CLK

	LPIT0_SwitchToSIRCInRUN();

	// disable FIRC
//	Clock_DisableFIRCInRUN();

	// disable SOSC
	// disable SPLL
}

/*
 * 配置HSRUN模式下core clock频率, 以切换到RUN
 * @note HSRUN core clock freq != RUN core clock freq时, 才有必要对HSRUN降频
 */
static void scg_configure_freq_for_RUN()
{
#if HSRUN_CORE_FREQ == HSRUN_112M  // 如果HSRUN core clock freq = 112M, 调节HSRUN core clock
	Clock_InitForHSRUNInRUN();
#endif
}

void scg_configure_freq_for_HSRUN()
{
#if HSRUN_CORE_FREQ == HSRUN_80M
	Clock_InitForHSRUNInRUN();
#endif
}


/**
 * 关闭SPLL和FIRC
 */
static void scg_disable_spll_and_firc(void)
{
	Clock_DisableSPLL();
	Clock_DisableFIRC();
}


void PMC_RUNToVLPR(void)
{
	// disable clock monitors on SCG module
	disable_clock_monitors();

	// Adjust SCG settings to meet maximum frequencies value
	scg_configure_freq_for_VLPR();

	// Allow very low power run mode
	SMC->PMPROT |= SMC_PMPROT_AVLP_MASK;

	// Check if current mode is RUN mode
	if(SMC->PMSTAT == 0x01)
	{
		// Reduce MCU power consumption in VLPS mode
//		PMC->REGSC |= PMC_REGSC_BIASEN_MASK;

		// Move to VLPR mode
		SMC->PMCTRL = SMC_PMCTRL_RUNM(0b10);

		// Wait for Transition
		while(SMC->PMSTAT != 0x04);
	}
}

/**
 * Switch mode from VLPR  to RUN
 */
void PMC_VLPRToRUN(void)
{
	// Check if current mode is VLPR mode
	if(SMC->PMSTAT == 0x04)
	{
		// Move to RUN mode
		SMC->PMCTRL = SMC_PMCTRL_RUNM(0b00);

		 // Biasing disabled, core logic can run in full performance
		PMC->REGSC |= PMC_REGSC_BIASEN_MASK;

		// Wait for transition
		while(SMC->PMSTAT != 0x01);
	}
}

/**
 * Switch mode from RUN  to VLPS
 */
void PMC_RUNToVLPS()
{
	// 关闭SIRC, FIRC, PLL偏置电流. 用于VLPS进一步降低功耗, 相应时钟模块必须关闭, 否则会报故障
//		PMC->REGSC &= ~PMC_REGSC_CLKBIASDIS_MASK;

	// 进入VLPS前关闭pll和firc
	scg_disable_spll_and_firc();

	// Allow very low run mode
	SMC->PMPROT |= SMC_PMPROT_AVLP_MASK;

	// Select VLPS mode
	SMC->PMCTRL = SMC_PMCTRL_STOPM(0b010);

	// Reduce MCU power consumption in VLPS mode
	PMC->REGSC |= PMC_REGSC_BIASEN_MASK; // The bit must be set to 1 when using VLPS mode

	// Check if current mode is RUN mode
	if(0x1 == SMC->PMSTAT)
	{
		// Go to sleep mode using WFI instruction
		asm("WFI"); // WFI - Wait for interrupt
	}
}

void PMC_VLPSToRUN()
{

}

void PMC_RUNToSTOP1()
{
	// Enable SLEEPDEEP bit in the core
//	FSL_sc
}

void PMC_RUNToSTOP2()
{

}

/**
 * RUN切换到HSRUN
 * @note 当HSRUN@80M == RUN@80M时, 无需重新配置SPLL; 当HSRUN@112M != RUN@80时, 需要重新配置SPLL, 临时用FIRC@48M作为sys clock时钟源
 */
void PMC_RUNToHSRUN(void)
{
	scg_configure_freq_for_HSRUN();  // 配置HSRUN时钟源和分频, 时钟源选择SPLL_CLK@160M, 配置sys clock@80M

	// Allow high speed run mode
	SMC->PMPROT |= SMC_PMPROT_AHSRUN_MASK; // Allow High Speed Run mode

	// check if current mode is RUN mode
	if(SMC->PMSTAT == 0x1)
	{
		// Move to HSRUN mode
		SMC->PMCTRL = SMC_PMCTRL_RUNM(0b11);

		// Wait for transition
		while(SMC->PMSTAT != 0x80);
	}
}


void PMC_HSRUNToRUN(void)
{
	// Adjust SCG module settings to meet frequencies value at RUN mode
	scg_configure_freq_for_RUN();

	// Check if current mode is RUN mode
	if(SMC->PMSTAT == 0x80)
	{
		// Move to RUN mode
		SMC->PMCTRL = SMC_PMCTRL_RUNM(0b00);

		// Wait for transition
		while(SMC->PMSTAT != 0x01);
	}
}
