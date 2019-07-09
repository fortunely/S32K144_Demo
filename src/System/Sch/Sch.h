/*
 * Sch.h
 *
 *  Created on: 2019Äê6ÔÂ3ÈÕ
 *      Author: willing
 */

#ifndef SYSTEM_SCH_SCH_H_
#define SYSTEM_SCH_SCH_H_

#include <stdint.h>
//#include <stddef.h>

typedef void (* const Sch_Task_t)(void);
typedef struct Sch_PolledTask_st
{
	void (* Task)(void);
	struct Sch_PolledTask_st * Next;
}Sch_PolledTask_t;

extern const Sch_Task_t *const APP_TRANSITION_LISTS[];// Declaration App.c, maximum 32 channel
extern const Sch_Task_t *const APP_CALLBACK_LISTS[];// Declaration App.c, maximum 32 channel

extern uint32_t Sch_TransitionEvent;// manual add in transistion flag

extern void Sch_1msTask(void);//for 1KHz timer to add new event only

extern uint32_t App_StateExec_1ms(void);// Declaration App.c, maximum 32 channel

extern void Sch_Main(void);
extern void Sch_AddPolledTask(Sch_PolledTask_t * Thread);//LIFO format
extern void Sch_EndPolledTask(Sch_PolledTask_t * Thread);

extern void Sch_AddEvent(void (*Task)(void));//1 time spwaning event, required to adjust max limit

#endif /* SYSTEM_SCH_SCH_H_ */
