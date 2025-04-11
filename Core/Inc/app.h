///*
// * app.h
// *
// *  Created on: Mar 15, 2025
// *      Author: ramakrishnan
// */
//
//#ifndef INC_APP_H_
//#define INC_APP_H_
//
#include "fatfs_sd.h"
#include "string.h"
#include "i2c-lcd.h"
#include "stdio.h"
#include "sdcard.h"
#include "rtc.h"
#include "keys.h"
#include "main.h"
#include "fatfs.h"

#define RX_BUFFER_SIZE 200


 uint8_t processReceivedData(void);
 void MainInit(void);
 void MainApp(void);
 void HandleButtonPress();
//#endif /* INC_APP_H_ */
