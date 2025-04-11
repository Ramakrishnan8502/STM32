///*
// * app.c
// *
// *  Created on: Mar 15, 2025
// *      Author: ramakrishnan
// */
#include "app.h"
extern int line_count;
extern uint8_t messageReceived;
extern UART_HandleTypeDef huart1;
char rxBuffer[RX_BUFFER_SIZE];

char time[30];
char date[30];

uint16_t rx_index = 0;
uint8_t rx_data;
char uart_buffer[1024];
uint8_t messageReceived =0;
int line_count = 0;

uint8_t sdCardsts = 0;


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_15);

  if (huart->Instance == USART1)
  {

    if (rx_index < RX_BUFFER_SIZE - 1)
    {

      rxBuffer[rx_index++] = rx_data;


      if (rx_data == '\n')
      {

        messageReceived = 1;
      }


      HAL_UART_Receive_IT(&huart1, &rx_data, 1);
    }
    else
    {
	 rx_index = 0;
      memset(rxBuffer, 0, RX_BUFFER_SIZE);
      HAL_UART_Receive_IT(&huart1, &rx_data, 1);
    }
  }
}

uint8_t processReceivedData(void)
{
	if(messageReceived == 0) return 0;
	log_to_sdcard(rxBuffer);

}

void MainInit(void)
{
   lcd_init();

   HAL_PWR_EnableBkUpAccess(); // Enable backup domain access

   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 1);

	if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) != 0x1234)
	{
		RTC_setTime();
		RTC_setDate();
		RTC_Backup_Write();
	} else
	{
		RTC_Backup_Read();
	}
}

void MainApp(void)
	{
		  getDateTime();
		  DisplayDateTime();
		  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_15);
//		  sdCardsts = check_sdcard_status();
		  sdCardsts = 1;
		  processReceivedData();
		  HAL_Delay(200);
	}


#define DEBOUNCE_DELAY 50  // 50ms debounce delay

void HandleButtonPress()
{
    static uint32_t lastPressTime = 0;
    uint32_t currentTime = HAL_GetTick();

    // Ensure enough time has passed since the last press
    if ((currentTime - lastPressTime) < DEBOUNCE_DELAY)
        return;  // Ignore button press if within debounce time

    // Check for button combinations
    if (ReadButton(GPIOA, BTN_3_Pin) && ReadButton(GPIOA, BTN_5_Pin))
    {
        EditDate();
    }
    else if (ReadButton(GPIOA, BTN_1_Pin) && ReadButton(GPIOA, BTN_3_Pin))
    {
        ReadsdCardStorage();
    }
    else
    {
        MainApp();
    }

    lastPressTime = currentTime;  // Update last press time
}


