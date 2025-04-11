/*
 * sdcard.h
 *
 *  Created on: Mar 9, 2025
 *      Author: ramakrishnan
 */

#ifndef INC_SDCARD_H_
#define INC_SDCARD_H_

#include "main.h"
#include "fatfs.h"
#include "fatfs_sd.h"
#include "string.h"
#include "i2c-lcd.h"
#include "stdio.h"

void log_to_sdcard(char *data);
void create_new_file();
uint8_t check_sdcard_status(void);
void ReadsdCardStorage(void);

extern FATFS fs; //file system
extern FIL fil;  //file
extern FRESULT fresult; //result
extern UINT br,bw;   //file read and write count
extern int line_count;

/*CAPACITY RELATED VARIABLES*/
extern FATFS *pfs;
extern DWORD fre_clust;
extern uint32_t total,free_space;



#endif /* INC_SDCARD_H_ */
