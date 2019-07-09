/*
 * PMC.c
 * @note Power manager controller module
 *  Created on: 2019��6��3��
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
 * @note S32K11x����clock monitor(CMU)
 */
void disable_clock_monitors()
{
	SCG->SPLLCSR &= ~SCG_SPLLCSR_SPLLCM_MASK;  // disable spll clock monitor
}

void enable_clock_monitors()
{
	SCG->SPLLCSR |= SCG_SPLLCSR_SPLLCM_MASK;  // enable spll clock monitor
}

// ����SPLL 80M
void scg_configure_spll()
{
}

/**
 * ����VLPRϵͳʱ�� 4M
 * @require RUNģʽ�½�������
 */

void scg_configure_freq_for_VLPR()
{
	Clock_InitVLPRInRUN(); // ����VLPRģʽ�µ�ʱ�Ӳ���

	Clock_InitSIRC(); // ����SIRC

//	Clock_Out_SIRC();  // ����CLKOUT pin���SIRC DIV2 CLK

	LPIT0_SwitchToSIRCInRUN();

	// disable FIRC
//	Clock_DisableFIRCInRUN();

	// disable SOSC
	// disable SPLL
}

/*
 * ����HSRUNģʽ��core clockƵ��, ���л���RUN
 * @note HSRUN core clock freq != RUN core clock freqʱ, ���б�Ҫ��HSRUN��Ƶ
 */
static void scg_configure_freq_for_RUN()
{
#if HSRUN_CORE_FREQ == HSRUN_112M  // ���HSRUN core clock freq = 112M, ����HSRUN core clock
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
 * �ر�SPLL��FIRC
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
	// �ر�SIRC, FIRC, PLLƫ�õ���. ����VLPS��һ�����͹���, ��Ӧʱ��ģ�����ر�, ����ᱨ����
//		PMC->REGSC &= ~PMC_REGSC_CLKBIASDIS_MASK;

	// ����VLPSǰ�ر�pll��firc
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
 * RUN�л���HSRUN
 * @note ��HSRUN@80M == RUN@80Mʱ, ������������SPLL; ��HSRUN@112M != RUN@80ʱ, ��Ҫ��������SPLL, ��ʱ��FIRC@48M��Ϊsys clockʱ��Դ
 */
void PMC_RUNToHSRUN(void)
{
	scg_configure_freq_for_HSRUN();  // ����HSRUNʱ��Դ�ͷ�Ƶ, ʱ��Դѡ��SPLL_CLK@160M, ����sys clock@80M

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
