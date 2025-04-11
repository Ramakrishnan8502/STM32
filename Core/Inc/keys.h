/*
 * keys.h
 *
 *  Created on: Mar 14, 2025
 *      Author: ramakrishnan
 */

#ifndef INC_KEYS_H_
#define INC_KEYS_H_

#include "main.h"
#include "i2c-lcd.h"
#include "string.h"
#include "stdio.h"

#define BTN_UP GPIO_PIN_4
#define BTN_DOWN GPIO_PIN_3
#define BTN_LEFT GPIO_PIN_2
#define BTN_RIGHT GPIO_PIN_1
#define BTN_ENTER GPIO_PIN_0
#define BTN_PORT  GPIOA

void EditDate(void);
void EditTime(void);
void setDateTimeDisplay(void);
uint8_t ReadButton(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

extern RTC_HandleTypeDef hrtc;
#endif /* INC_KEYS_H_ */
