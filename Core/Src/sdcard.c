/*
 * sdcard.c
 *
 *  Created on: Mar 9, 2025
 *      Author: ramakrishnan
 */

#include "sdcard.h"
#include "i2c-lcd.h"
#include "rtc.h"
#include "app.h"
extern char time[30];
extern char date[30];
extern uint8_t messageReceived;
extern char filename[20];
extern uint8_t sdCardsts;
extern IWDG_HandleTypeDef hiwdg;

void log_to_sdcard(char *data)
{
//	if(sdCardsts == 0)return;
    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef sDate;
    char timestamp[50];

  static  int oldTime,currentTime;

    HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

    sprintf(timestamp, "[%02d-%02d-%04d %02d:%02d:%02d] ",
            sDate.Date, sDate.Month, 2000 + sDate.Year,
            sTime.Hours, sTime.Minutes, sTime.Seconds);

    fresult = f_mount(&fs, "", 1);
    fresult = f_open(&fil, filename, FA_OPEN_ALWAYS | FA_WRITE);
    f_lseek(&fil, f_size(&fil));
    f_write(&fil, timestamp, strlen(timestamp), NULL);
    f_write(&fil, data, strlen(data), NULL);
    f_write(&fil, "\r\n", 2, NULL);
    f_close(&fil);
    f_mount(NULL, "", 1);

    currentTime =  sTime.Minutes;
    if(currentTime != oldTime)
    {
        create_new_file();
        oldTime = currentTime;

    }

}

void create_new_file()
{
//    static uint8_t file_index = 1;
//    file_index++;
//

    RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;

	  HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	  HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	//  sprintf(filename, "data%d.txt", file_index);

	  sprintf(filename, "%02d_%02d_%04d %02d_%02d_%02d.txt",
	  sDate.Date, sDate.Month, 2000 + sDate.Year,
	  sTime.Hours, sTime.Minutes, sTime.Seconds);
}

uint8_t check_sdcard_status(void)
{
    fresult = f_mount(&fs, "", 1);
    if(fresult != FR_OK)
    {
    	 f_mount(NULL, "", 1);  // Unmount SD card
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
        lcd_send_string("NO SD CARD");
        return 0;

    }
    else
    {
    	f_mount(NULL, "", 1);  // Unmount SD card
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
        return 1;
    }
}

void ReadsdCardStorage(void)
{

		  FATFS fs;
		  FATFS *pfs;
		  DWORD fre_clust;
		  uint32_t free_space, total;
		  FRESULT fresult;
		  uint8_t editing = 1;
		  lcd_clear();

  while(editing)
  {

		  // Mount the SD card
		  fresult = f_mount(&fs, "", 1);
		  if (fresult != FR_OK)
		  {
			  lcd_send_string("No SD Card");
	//		  return; // Exit if no SD card is found
		  }

		  // Get free clusters
		  fresult = f_getfree("", &fre_clust, &pfs);
		  if (fresult != FR_OK)
		  {
			  lcd_send_string("Error reading SD");
			//  return;
		  }

		  // Calculate free and total space in KB
		  free_space = (uint32_t)(fre_clust * pfs->csize * 512) / 1024;  // KB
		  total = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 512) / 1024;  // KB

		  // Check if SD card is available
		  if (total == 0)
		  {
			  lcd_send_string("No SD Card");
			//  return;
		  }

		  // Display space info
		  lcd_put_cur(0, 0);
		  char buffer[16];
		  sprintf(buffer, "Free: %lu KB", free_space);
		  lcd_send_string(buffer);
		  lcd_put_cur(1, 0);
		  memset(buffer,0,sizeof(buffer));
		  sprintf(buffer, "total:%lu KB", total);
		  lcd_send_string(buffer);

	  if ((HAL_GPIO_ReadPin(BTN_PORT, BTN_ENTER) == GPIO_PIN_SET) &&
	     (HAL_GPIO_ReadPin(BTN_PORT, BTN_RIGHT) == GPIO_PIN_RESET))
		   {
			   HAL_Delay(50); // Debounce
			   if ((HAL_GPIO_ReadPin(BTN_PORT, BTN_ENTER) == GPIO_PIN_SET) &&
				   (HAL_GPIO_ReadPin(BTN_PORT, BTN_RIGHT) == GPIO_PIN_RESET))
			   {
				   editing = 0; // Exit loop
			   }
		   }
	       HAL_IWDG_Refresh(&hiwdg);
		  }
		  lcd_clear();
	      HAL_Delay(200); // Ensure stable exit
}
