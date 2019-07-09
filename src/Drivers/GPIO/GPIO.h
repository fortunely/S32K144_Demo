/**
* @filename GPIO.h
* @author Martin
* @brief Port configure header file
* @date 2019-07-09
* @version 0.1
* @note initial draft
*/

#ifndef DRIVERS_GPIO_GPIO_H_
#define DRIVERS_GPIO_GPIO_H_

#include "S32K144.h"


#define PORTD14_IDX              14
#define PORTD16_IDX              16

#define PORT_CLKOUT_IDX          PORTD14_IDX
#define PORT_LED_GREEN_IDX       PORTD16_IDX

typedef enum{
	PORT_GPIO_INPUT,
	PORT_GPIO_OUTPUT
}PORT_GPIO_t;


/**
 * Port set output high
 */
static inline void Port_Set(GPIO_Type *pPortGroup, int pinIndex)
{
	pPortGroup->PSOR |= 1 << pinIndex; // Port Set Output
}

/**
 * Port set output low
 */
static inline void Port_Clear(GPIO_Type *pPortGroup, int pinIndex)
{
	pPortGroup->PCOR |= 1 << pinIndex; // Port Clear Output
}

/**
 * Port toggle output
 */
static inline void Port_Toggle(GPIO_Type *pPortGroup, int pinIndex)
{
	pPortGroup->PTOR |= 1 << pinIndex;
}


void Port_Init(void);

#endif /* DRIVERS_GPIO_GPIO_H_ */
