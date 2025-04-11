/*
 * rtc.h
 *
 *  Created on: Mar 9, 2025
 *      Author: ramakrishnan
 */

#ifndef INC_RTC_H_
#define INC_RTC_H_

#include "main.h"

void getDateTime(void);
void RTC_Backup_Read(void);
void RTC_Backup_Write(void);
void DisplayDateTime(void);
void RTC_setDate(void);
void RTC_setTime(void);
void MX_RTC_Init(void);


extern RTC_HandleTypeDef hrtc;

#endif /* INC_RTC_H_ */

