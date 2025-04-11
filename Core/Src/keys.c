/*
 * keys.c
 *
 *  Created on: Mar 14, 2025
 *      Author: ramakrishnan
 */

#include "keys.h"
#include "app.h"

RTC_TimeTypeDef sTime = {0};
RTC_DateTypeDef sDate = {0};

char etime[30];
char edate[30];
extern IWDG_HandleTypeDef hiwdg;

void EditDate()
{
    uint8_t editing = 1;
    uint8_t curser_pos = 0; // Track current editing field
    lcd_clear();

    // Define cursor positions (two-digit fields)
    const uint8_t cursor_pos[][2] = {
        {0, 1},   // Hours (HH)
        {3, 4},   // Minutes (MM)
        {6, 7},   // Seconds (SS)
        {0x40, 0x41},  // Day (DD)
        {0x43, 0x44},  // Month (MM)
        {0x46, 0x47}   // Year (YY)
    };

    while (editing)
    {
        setDateTimeDisplay(); // Update LCD
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_9);


        // Move cursor to first digit of the selected field
        lcd_send_cmd(0x80 | cursor_pos[curser_pos][0]);

        // Cursor Blink Logic (500ms Interval)
        static uint32_t lastToggle = 0;
           if (HAL_GetTick() - lastToggle > 500)
           {
               lcd_send_cmd(0x0E);  // Cursor ON, Blink OFF
               HAL_Delay(250);
               lcd_send_cmd(0x0F);  // Cursor ON, Blink ON
               lastToggle = HAL_GetTick();
           }
        // Navigation between fields
        if (ReadButton(BTN_PORT, BTN_LEFT)) curser_pos = (curser_pos + 5) % 6;
        if (ReadButton(BTN_PORT, BTN_RIGHT)) curser_pos = (curser_pos + 1) % 6;

        // Increase Selected Field
        if (ReadButton(BTN_PORT, BTN_UP))
        {
            if (curser_pos == 0) sTime.Hours = (sTime.Hours + 1) % 24;
            if (curser_pos == 1) sTime.Minutes = (sTime.Minutes + 1) % 60;
            if (curser_pos == 2) sTime.Seconds = (sTime.Seconds + 1) % 60;
            if (curser_pos == 3) sDate.Date = (sDate.Date % 31) + 1;
            if (curser_pos == 4) sDate.Month = (sDate.Month % 12) + 1;
            if (curser_pos == 5) sDate.Year = (sDate.Year + 1) % 100;
        }

        // Decrease Selected Field with Proper Wrap-around
        if (ReadButton(BTN_PORT, BTN_DOWN))
        {
            if (curser_pos == 0) sTime.Hours = (sTime.Hours + 23) % 24;
            if (curser_pos == 1) sTime.Minutes = (sTime.Minutes + 59) % 60;
            if (curser_pos == 2) sTime.Seconds = (sTime.Seconds + 59) % 60;
            if (curser_pos == 3) sDate.Date = (sDate.Date == 1) ? 31 : sDate.Date - 1;
            if (curser_pos == 4) sDate.Month = (sDate.Month == 1) ? 12 : sDate.Month - 1;
            if (curser_pos == 5) sDate.Year = (sDate.Year == 0) ? 99 : sDate.Year - 1;
        }
        // Exit if BOTH LEFT and RIGHT buttons are pressed
        if ((HAL_GPIO_ReadPin(BTN_PORT, BTN_LEFT) == GPIO_PIN_SET) &&
            (HAL_GPIO_ReadPin(BTN_PORT, BTN_RIGHT) == GPIO_PIN_SET))
        {
            HAL_Delay(50); // Debounce
            if ((HAL_GPIO_ReadPin(BTN_PORT, BTN_LEFT) == GPIO_PIN_SET) &&
                (HAL_GPIO_ReadPin(BTN_PORT, BTN_RIGHT) == GPIO_PIN_SET))
            {
                break; // Exit loop
            }
        }

        // Check if ENTER is pressed and RIGHT is NOT pressed
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
    lcd_send_cmd(0x0C);  // Turn off cursor when exiting edit mode
    lcd_clear();
    HAL_Delay(200); // Ensure stable exit
    HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
    HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
}

void setDateTimeDisplay(void)
{
	char buffer[16];
	memset(buffer,0,sizeof(buffer));
	sprintf(buffer,"%02d/%02d/%02d = Date",sDate.Date,sDate.Month,sDate.Year);
	lcd_send_cmd(0xC0);
	lcd_send_string(buffer);
	memset(buffer,0,sizeof(buffer));
	sprintf(buffer,"%02d:%02d:%02d = Time",sTime.Hours,sTime.Minutes,sTime.Seconds);
	lcd_send_cmd(0x80);
	lcd_send_string(buffer);
}

uint8_t ReadButton(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_9);
    if (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET)  // Button pressed
    {
        HAL_Delay(75);  // Debounce delay
        if (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET)  // Confirm still pressed
        {
            while (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_RESET);  // Wait for release
            return 1;  // Button press confirmed
        }
    }
    return 0;  // No press detected
}
