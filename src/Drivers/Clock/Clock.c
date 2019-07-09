/**
* @filename Clock.c
* @author Martin
* @brief Clock module source file
* @date 2019-07-09
* @version 0.1
* @note initial draft
*/

#include "Clock.h"
#include "S32K144.h"

void Clock_InitSPLL_160M(void);
void Clock_InitSOSC_8M(void);


/**
 * ����MCUʱ��, ʹSYS_CLK=80MHz, BUS_CLK=40MHz, FLASH_CLK=20MHz, ���ⲿ����f_osc=8MHz
 * SPLL_CLK=160MHz, VCO_CLK=320MHz
 * @note
 * ���㹫ʽ:
 * BUS_CLK=SYS_CLK / DIVBUS
 * SYS_CLK=SPLL_CLK / DIVCORE
 * SPLL_CLK=(VCO_CLK) / 2
 * VCO_CLK=SPLL_SOURCE / (PREDIV + 1) x (MULT + 16)
 * ѡ���ⲿ������ΪSPLL_SOURCE, SPLL_SOURCE=8MHz
 */
void Clock_Init(void)
{
	// ���³�ʼ��˳�򲻿ɸı�

	Clock_InitSOSC_8M(); // SOSC&SPLL_SOURCE��ʼ��Ϊ���8M

	Clock_InitSPLL_160M(); // SPLL&SCG��ʼ��Ϊ���160M

	Clock_InitRUN_80M(); // sys clock = 80M, bus clock = 40M, flash = 20M

	Clock_InitSPLLDIV_80M(); // spll div1 clock = 80M, spll div2 clock = 80M

}


/**
 * CLKOUT�������
 */
void Clock_ConfigureOut(SIM_CLKOUTSEL_Type clkoutsel)
{
	if(clkoutsel <= SIM_CLKOUTSEL_RTC_CLK)
	{
		SIM->CHIPCTL &= ~SIM_CHIPCTL_CLKOUTEN_MASK; // disable CLKOUT
		SIM->CHIPCTL &= ~SIM_CHIPCTL_CLKOUTSEL_MASK; // clear CLKOUT

		SIM->CHIPCTL |= SIM_CHIPCTL_CLKOUTSEL(clkoutsel); // select CLKOUT source = clkoutsel

		SIM->CHIPCTL |= SIM_CHIPCTL_CLKOUTEN_MASK; // enable CLKOUT
	}
}


/**
 * CLKOUT pin���Bus CLK
 */
void Clock_OutBus(void)
{
	Clock_ConfigureOut(SIM_CLKOUTSEL_BUS_CLK);
}

/**
 * CLKOUT pin���SPLL DIV2 CLK
 */
void Clock_OutSPLL(void)
{
	Clock_ConfigureOut(SIM_CLKOUTSEL_SPLL_DIV2_CLK);
}

/**
 * CLKOUT pin���FIRC DIV2 CLK
 */
void Clock_OutFIRC(void)
{
	Clock_ConfigureOut(SIM_CLKOUTSEL_FIRC_DIV2_CLK);
}

/**
 * CLKOUT pin���SIRC DIV2 CLK
 */
void Clock_OutSIRC(void)
{
	Clock_ConfigureOut(SIM_CLKOUTSEL_SIRC_DIV2_CLK);
}

/**
 * ����SOSC_CLK=8M
 * @output SOSCDIV1_CLK
 * @output SOSCDIV2_CLK
 * @note
 * �ⲿ����8M
 * ���㹫ʽ
 * SOSCDIV1_CLK=SOSC_CLK / SOSCDIV1
 * SOSCDIV2_CLK=SOSC_CLK / SOSCDIV2
 */
void Clock_InitSOSC_8M(void)
{
	// Frequency division for SOSCDIV1_CLK, SOSCDIV2_CLK
	SCG->SOSCDIV=SCG_SOSCDIV_SOSCDIV1(1) // SOSCDIV1=1 : Divide by 1
			| SCG_SOSCDIV_SOSCDIV2(1);   // SOSCDIV2=1 : Divide by 1

	SCG->SOSCCFG=SCG_SOSCCFG_RANGE(0b11) // RANGE=3: 8-40 MHz SOSC
			// RANGE=3: 8-40 MHz SOSC
			| SCG_SOSCCFG_HGO(1)    // HGO=0: Configure crystal oscillator for high-gain operation
			| SCG_SOSCCFG_EREFS(1); // EREFS=1�� Internal crystal oscillator of OSC selected

	while(SCG->SOSCCSR & SCG_SOSCCSR_LK_MASK); /* Ensure SOSCCSR unlocked */
	SCG->SOSCCSR = SCG_SOSCCSR_SOSCEN(1);      // SOSC Enable
}

/**
 * ����MCUʱ��SPLL_CLK=160M
 * @require SOSC_CLK = 8M
 * @output SPLL_CLK
 */
void Clock_InitSPLL_160M(void)
{
	while(SCG->SPLLCSR & SCG_SPLLCSR_LK_MASK); /* Ensure SPLLCSR unlocked */

	// �ر�SPLL
	SCG->SPLLCSR = 0; /* SPLLEN=0: SPLL is disabled (default) */

	// ����SCG_SPLLCFG, ʹVCO_CLK=320MHz, SPLL_CLK=160MHz
	SCG->SPLLCFG = SCG_SPLLCFG_PREDIV(0) | SCG_SPLLCFG_MULT(0b11000);

	SCG->RCCR = SCG_RCCR_SCS(0b0110)  // System Clock Source: System PLL (SPLL_CLK)
			| SCG_RCCR_DIVCORE(0)   // DIVCORE=0
			| SCG_RCCR_DIVBUS(1)    // DIVBUS=1
			| SCG_RCCR_DIVSLOW(1);  // DVISLOW=1

	// ʹ��SPLL
	SCG->SPLLCSR |= SCG_SPLLCSR_SPLLEN_MASK;
	// Lock SPLL
	SCG->SPLLCSR |= SCG_SPLLCSR_LK_MASK;
}


/**
 * SYS_CLK = 80M, BUS_CLK = 40M, FLASH_CLK=20M
 * @require SPLL_CLK=160M
 * @note SYS_CLK=80MHz,BUS_CLK=40MHz, ʹ���ⲿ����f_osc=8MHz
 * ǰ������ʹ��SPLL_SOURCE=160M��Ϊsystem clock
 * ���㹫ʽ:
 * SYS_CLK=SPLL_CLK / DIVCORE
 * BUS_CLK=SYS_CLK / DIVBUS
 * FLASH_CLK=SYS_CLK / DIVSLOW
 * SPLL_CLK=(VCO_CLK) / 2
 * VCO_CLK=SPLL_SOURCE / (PREDIV + 1) x (MULT + 16)
 *
 * =>DIVCORE=1, DIVBUS=1,DIVSLOW=3
 */
void Clock_InitRUN_80M(void)
{
	// ����system clockԴ, DIVCORE, DIVBUS, DIVFLASH
	SCG->RCCR = SCG_RCCR_SCS(0b0110)// Select System Clock Source: System PLL (SPLL_CLK)
			| SCG_RCCR_DIVCORE(1)
			| SCG_RCCR_DIVBUS(1)
			| SCG_RCCR_DIVSLOW(3);

	// ���Clock Status(SCG_CSR) �Ƿ�ΪRCCR����
	while(0b0110 != (SCG->CSR & SCG_CSR_SCS_MASK) >> SCG_CSR_SCS_SHIFT) { }
}

/**
 * ����SPLLDIV1_CLK=80M, SPLLDIV2_CLK=80M
 * @require SPLL_CLK=160M
 */
void Clock_InitSPLLDIV_80M(void)
{
	SCG->SPLLDIV = SCG_SPLLDIV_SPLLDIV1(0b010)   // SPLLDIV1 = 2: System PLL Clock Divide 1 2��Ƶ
			| SCG_SPLLDIV_SPLLDIV2(0b010);       // SPLLDIV2 = 2: System PLL Clock Divide 2 2��Ƶ
}

/**
 * ����SYS_CLK = 4M, BUS_CLK = 4M, FLASH_CLK = 1M
 * @note ��ʱsystem clock source������Slow IRC(SIRC_CLK = 8M)
 */
void Clock_InitVLPRInRUN()
{
	// S1 Configure VCCR for VLPR using SPLL as source clock
	SCG->VCCR = SCG_VCCR_SCS(0b0010)        // VLPRģʽ��ʱ��Դѡ��Slow IRC
			| SCG_VCCR_DIVCORE(0b001)       // ��Ƶ = 2
			| SCG_VCCR_DIVBUS(0)            // ��Ƶ = 1
			| SCG_VCCR_DIVSLOW(0b0011);     // ��Ƶ = 4
}

/**
 * ����SYS_CLK = 80M, BUS_CLK = 40M, FLASH_CLK = 20M
 * @require SPLL_CLK = 160M
 */
void Clock_InitForHSRUNInRUN()
{
	// S1 Configure HCCR for HSRUN using SPLL as source clock
	SCG->HCCR = SCG_HCCR_SCS(0b0110) // HSRUNģʽ��sysʱ��Դѡ��SPLL
			| SCG_RCCR_DIVCORE(1) // core clock��Ƶ = 2
			| SCG_RCCR_DIVBUS(1)  // ��core clock������, bus clock��Ƶ = 2
			| SCG_RCCR_DIVSLOW(3); // core clock������, flash clock��Ƶ = 4
}

/**
 * ����SIRCDIV1_CLK=4M, SIRCDIV2_CLK=4M
 * @require SIRC=8M(SIRCĬ��8M�� ��ѡ2M, 8M)
 */
void Clock_InitSIRC()
{
	SCG->SIRCDIV = SCG_SIRCDIV_SIRCDIV1(0b010)   //SIRCDIV1=0b010 -- 2��Ƶ(4M)
			| SCG_SIRCDIV_SIRCDIV2(0b010);       //SIRCDIV2=0b010 -- 2��Ƶ(4M)

	scg_sirc_disable();  // Slow IRC disable

	SCG->SIRCCSR = SCG_SIRCCSR_SIRCLPEN_MASK  // Slow IRC is enabled in VLP modes
			| SCG_SIRCCSR_SIRCSTEN_MASK;      // Slow IRC is enabled in supported Stop modes

	SCG->SIRCCFG = SCG_SIRCCFG_RANGE_MASK;    // Slow IRC = 8M

	scg_sirc_enable(); // Slow IRC Enable

	// Wait until clock is valid
	while(1 != (SCG->SIRCCSR & SCG_SIRCCSR_SIRCEN_MASK)) {};

	// lock Control Status Register
	SCG->SIRCCSR |= SCG_SIRCCSR_LK_MASK; // LK=1 -- lock SCG_SIRCCSR
}

/**
 * �ر�SPLL
 */
void Clock_DisableSPLL(void)
{
	// When entering VLPR/VLPS mode, the system clock should be SIRC. The FIRC, SOSC,
	// and SPLL must be disabled by software in RUN mode before making any mode
	// transition.

	if(!(SCG->SPLLCSR & SCG_SPLLCSR_SPLLSEL_MASK))
	{//SPLLSEL, if SPLL is not the system clock source

		// SPLLEN = 0, SPLL disabled
		SCG->SPLLCSR &= ~SCG_SPLLCSR_SPLLEN_MASK;
		// SPLLCM = 0, disable spll clock monitor
		SCG->SPLLCSR &= ~SCG_SPLLCSR_SPLLCM_MASK;

		// SPLLVLD = 0, Wait until SPLL is not enabled or clock is not valid
		while(SCG->SPLLCSR & SCG_SPLLCSR_SPLLVLD_MASK){};
	}
}

/**
 * RUNģʽ�¹ر�FIRC
 */
void Clock_DisableFIRC(void)
{
    // When entering VLPR/VLPS mode, the system clock should be SIRC. The FIRC, SOSC,
    // and SPLL must be disabled by software in RUN mode before making any mode
    // transition.

    if(!(SCG->FIRCCSR & SCG_FIRCCSR_FIRCSEL_MASK))
    {   // [25] FIRCSEL, if FIRC is not the system clock source

    	// [0] FIRCEN = 0 FIRC disabled
        SCG->FIRCCSR &= ~SCG_FIRCCSR_FIRCEN_MASK;

        // [24] FIRCVLD = 0 Wait until FIRC is not enabled or clock is not valid
        while(SCG->FIRCCSR & SCG_FIRCCSR_FIRCVLD_MASK) {};
    }
}

/**
 * RUN mode�¹ر�SOSC
 */
void Clock_DisableSOSC(void)

{
    // When entering VLPR/VLPS mode, the system clock should be SIRC. The FIRC, SOSC,
    // and SPLL must be disabled by software in RUN mode before making any mode
    // transition.

	if(!(SCG->SOSCCSR & SCG_SOSCCSR_SOSCSEL_MASK))
	{// SOSCSEL, if the SOSC is not the system clock source

		// unlock SOSCCSR
		if(SCG->SOSCCSR & SCG_SOSCCSR_LK_MASK)
			SCG->SOSCCSR &= ~SCG_SOSCCSR_LK_MASK;

		// SCG_SOSCCSR[SOSCEN] = 0, System OSC is enabled
		SCG->SOSCCSR &= ~SCG_SOSCCSR_SOSCEN_MASK;

		// SCG_SOSCCSR[SOSCVLD] =0, Wait until System OSC is not enabled or clock is not valid
		while(SCG->SOSCCSR & SCG_SOSCCSR_SOSCVLD_MASK) {};
	}
}
