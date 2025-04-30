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
#include "rylr998.h"

/* Public functions ----------------------------------------------------------*/
void LSU_setAddress(uint8_t addr);
void LSU_setChannelMain(void);
void LSU_setChannelAux(void);

void LSU_sendParameters(uint16_t destination, int32_t Lat, int32_t Lon, uint16_t T1, uint16_t T2, uint8_t bpm);
void LSU_sendSyncRequest(uint16_t destination);
RYLR_RX_data_t* LSU_getConfig(void);

#ifdef __cplusplus
}
#endif

#endif /* __LSU_COMMS_H__ */
