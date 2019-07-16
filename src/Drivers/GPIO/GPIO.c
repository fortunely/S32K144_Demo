/**
* @filename GPIO.c
* @author Martin
* @brief Port configure source file
* @date 2019-07-09
* @version 0.1
* @note initial draft
*/

#include "S32K144.h"
#include "GPIO.h"

#define PORT_UJA_SPI_SCK_IDX       	14
#define PORT_UJA_SPI_SIN_IDX       	15
#define PORT_UJA_SPI_SOUT_IDX       16
#define PORT_UJA_SPI_PCS3_IDX       17


#define PORT_UJA_CAN_RX_IDX     	4
#define PORT_UJA_CAN_TX_IDX     	5

/**
 * Port init for SPI1
 */
void Port_SPI1Init(void)
{
	/* Enable clock to SPI1 */
	if(PCC->PCCn[PCC_LPSPI1_INDEX] & PCC_PCCn_PR_MASK)
	{// Peripheral SPI1 is supported
		PCC->PCCn[PCC_LPSPI1_INDEX] |= PCC_PCCn_CGC(1);
	}

	// Set MUX
	/*
	 * PTB14 - LPSPI1_SCK - ALT = 0000_0011
	 * PTB15 - LPSPI1_SIN - ALT = 0000_0011
	 * PTB16 - LPSPI1_SOUT - ALT = 0000_0011
	 * PTB17 - LPSPI1_PCS3 - ALT = 0000_0011
	 */
	PORTB->PCR[PORT_UJA_SPI_SCK_IDX] = PORT_PCR_MUX(0b00000011);
	PORTB->PCR[PORT_UJA_SPI_SIN_IDX] = PORT_PCR_MUX(0b00000011);
	PORTB->PCR[PORT_UJA_SPI_SOUT_IDX] = PORT_PCR_MUX(0b00000011);
	PORTB->PCR[PORT_UJA_SPI_PCS3_IDX] = PORT_PCR_MUX(0b00000011);
}

/**
 * Port init for CAN0
 */
void Port_Can0Init(void)
{
	/* Enable clock to CAN0 */
	if(PCC->PCCn[PCC_FlexCAN0_INDEX] & PCC_PCCn_PR_MASK)
	{// Peripheral CAN0 is supported
		PCC->PCCn[PCC_FlexCAN0_INDEX] |= PCC_PCCn_CGC(1);
	}

	// Set MUX
	/*
	 * PTE4 - CAN0_RX - ALT = 0000_0101
	 * PTE5 - CAN0_TX - ALT = 0000_0101
	 */
	PORTE->PCR[PORT_UJA_CAN_RX_IDX] = PORT_PCR_MUX(0b00000101);
	PORTE->PCR[PORT_UJA_CAN_TX_IDX] = PORT_PCR_MUX(0b00000101);
}

/**
 * PTD14 - CLKOUT
 * PTD16 - GPO for LED
 */
void Port_Init(void)
{
	// Enable clock to PORTD
	if(PCC->PCCn[PCC_PORTD_INDEX] & PCC_PCCn_PR_MASK)
	{
		PCC->PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_CGC(1); // Clock enabled to PORTD
	}

	// Set Port Data Direction
	PTD->PDDR |= 1 << PORT_LED_GREEN_IDX; // Port Data Direction: output

	// set/clear PortD's value
//	PTD->PTSO |= 1 << PORT_LED_GREEN_IDX; // Port Set Output
	PTD->PCOR |= 1 << PORT_LED_GREEN_IDX; // Port Clear Output

#if 0
	// Set MUX
	if(! (PORTD->PCR[PORT_LED_GREEN_IDX] & PORT_PCR_LK_MASK) )
	{//Port D16's Pin Control Register not lock
		// PTD16 - GPO
		PORTD->PCR[PORT_LED_GREEN_IDX] = PORT_PCR_MUX(0b0000001); // Port D16: MUX=ALT1, GPIO(to green LED on EVB/PCB)
	}
#endif

	// Set MUX
	// PTD16 - GPO
	PORTD->PCR[PORT_LED_GREEN_IDX] = PORT_PCR_MUX(0b0000001); // Port D16: MUX=ALT1, GPIO(to green LED on EVB/PCB)
	// PTD14 - CLKOUT
	PORTD->PCR[PORT_CLKOUT_IDX] = PORT_PCR_MUX(0b00000111);  // Port D14: MUX=ALT4, CLKOUT

	Port_Toggle(PTD, PORT_LED_GREEN_IDX);

	Port_SPI1Init();
	Port_Can0Init();
}
