/*
 * rtc.c
 *
 *  Created on: Mar 9, 2025
 *      Author: ramakrishnan
 */
#include "rtc.h"
#include "i2c-lcd.h"
#include "stdio.h"

extern char time[30];
extern char date[30];


void RTC_Backup_Write(void)
 {
     HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x1234);

 }

 void RTC_Backup_Read(void)
 {
    HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1);
 }

 void getDateTime(void)
  {
 	 RTC_DateTypeDef gDate = {0};
 	 RTC_TimeTypeDef gTime = {0};

 	 HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);
 	 HAL_RTC_GetDate(&hrtc, &gDate, RTC_FORMAT_BIN);

 	 sprintf((char*)time, "%02d:%02d:%02d", gTime.Hours, gTime.Minutes, gTime.Seconds);
 	 sprintf((char*)date, "%02d-%02d-%04d", gDate.Date, gDate.Month, 2000 + gDate.Year);
  }

 void RTC_setTime(void)
  {
 	  RTC_TimeTypeDef sTime = {0};
 	      sTime.Hours = 22;
 	      sTime.Minutes = 31;
 	      sTime.Seconds = 00;
 	      sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
 	      sTime.StoreOperation = RTC_STOREOPERATION_RESET;
 	      HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

  }
  void RTC_setDate(void)
  {
    RTC_DateTypeDef sDate = {0};

    sDate.WeekDay = RTC_WEEKDAY_TUESDAY;
    sDate.Month = RTC_MONTH_MARCH;
    sDate.Date = 0x11;
    sDate.Year = 0x25;

    if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
    {
 	 Error_Handler();
    }
  }


  void DisplayDateTime(void)
  {
 	 lcd_send_cmd(0x80);
	 lcd_send_string(time);
 	 lcd_send_cmd(0xC0);
 	 lcd_send_string(date);
  }

  void MX_RTC_Init(void)
  {

    /* USER CODE BEGIN RTC_Init 0 */

    /* USER CODE END RTC_Init 0 */
    /* USER CODE BEGIN RTC_Init 1 */

    /* USER CODE END RTC_Init 1 */

    /** Initialize RTC Only
    */
    hrtc.Instance = RTC;
    hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
    hrtc.Init.AsynchPrediv = 127;
    hrtc.Init.SynchPrediv = 255;
    hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
    hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
    if (HAL_RTC_Init(&hrtc) != HAL_OK)
    {
      Error_Handler();
    }

    /* USER CODE BEGIN Check_RTC_BKUP */





  }
