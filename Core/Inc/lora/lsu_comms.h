/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : lsu_comms.h
  * @brief          : Header file for LSU communications, handles the API to
  *                   communicate with the Central Unit
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LSU_COMMS_H__
#define __LSU_COMMS_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

#include "rylr998.h"

/* Public type definitions --------------------------------------------------- */
typedef struct {
  int32_t latitude;
  int32_t longitude;
  uint16_t temperature_livestock;
  uint16_t temperature_environment;
  uint8_t heartrate;
} LSU_Payload;

/* Public functions ----------------------------------------------------------*/
void LSU_setAddress(uint8_t addr);
void LSU_setChannelMain(void);
void LSU_setChannelAux(void);

void LSU_sendParameters(uint16_t destination, LSU_Payload* payload);
void LSU_sendSyncRequest(uint16_t destination);
bool LSU_checkChannelBusy(void);
RYLR_RX_data_t* LSU_getData(void);

#ifdef __cplusplus
}
#endif

#endif /* __LSU_COMMS_H__ */
