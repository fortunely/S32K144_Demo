/*
 * PMC.h
 *
 *  Created on: 2019Äê6ÔÂ3ÈÕ
 *      Author: willing
 */

#ifndef DRIVERS_PMC_PMC_H_
#define DRIVERS_PMC_PMC_H_

void PMC_Init();

void PMC_VLPRToRUN(void);
void PMC_RUNToVLPR(void);

void PMC_RUNToVLPS();
void PMC_VLPSToRUN();

void PMC_HSRUNToRUN(void);
void PMC_RUNToHSRUN(void);

#endif /* DRIVERS_PMC_PMC_H_ */
