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
}
