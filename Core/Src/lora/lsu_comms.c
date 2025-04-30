/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : lsu_comms.c
  * @brief          : Source file for LSU communications, handles the API to
  *                   communicate with the Central Unit
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "LSU_comms.h"
#include "rylr998.h"

#include <math.h>
#include <string.h>
#include <stdio.h>

uint8_t address = 0x00;

/* Private variables ---------------------------------------------------------*/
static char txBuffer[TX_BUFFER_SIZE];

/* Private functions ---------------------------------------------------------*/
int count_digits(int32_t num) {
  if (num == 0) return 1; // Number 0 has only one digit

  int count = 0;
  if (num < 0) count++; // We consider the '-' sign as a digit
  
  num = abs(num);
  while (num > 0) {
    num /= 10;
    count++;
  }
  return count;
}

/* Public functions ----------------------------------------------------------*/

void LSU_setAddress(uint8_t addr){
  address = addr;
}

void LSU_setChannelMain(void) {
  rylr998_setChannel(1, address);
}

void LSU_setChannelAux(void){
  rylr998_setChannel(0, address);
}

// We need delays to ensure the data is sent before performing the next operation
#define SEND_DATA_DELAY 500
void LSU_sendParameters(uint16_t destination, int32_t Lat, int32_t Lon, uint16_t T1, uint16_t T2, uint8_t bpm) {
  memset(txBuffer, 0, sizeof(TX_BUFFER_SIZE));
  uint8_t data_length = count_digits(Lat)
                      + count_digits(Lon)
                      + count_digits(T1)
                      + count_digits(T2)
                      + count_digits(bpm)
                      + 4; // including the four dashes
  sprintf(txBuffer, AT"SEND=%u,%u,%ld-%ld-%u-%u-%u"END, destination, data_length, Lat, Lon, T1, T2, bpm);

  rylr998_sendCommand(txBuffer);
  HAL_Delay(SEND_DATA_DELAY);
  rylr998_getCommand(RYLR_OK);
}

#define SEND_SYNC_DELAY 100
void LSU_sendSyncRequest(uint16_t destination){
  memset(txBuffer, 0, sizeof(TX_BUFFER_SIZE));
  uint8_t data_length = 4;
  sprintf(txBuffer, AT"SEND=%u,%u,SYNC"END, destination, data_length);

  rylr998_sendCommand(txBuffer);
  HAL_Delay(SEND_SYNC_DELAY);
  rylr998_getCommand(RYLR_OK);
}

RYLR_RX_data_t* LSU_getConfig(void){
  return rylr998_getCommand(RYLR_RCV);
}
