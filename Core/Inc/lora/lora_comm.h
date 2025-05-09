/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : lora_comm.h
  * @brief          : Header for LoRa communication file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 Gonazalez & Morris.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * 
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LORA_COMM_H__
#define __LORA_COMM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

/* Structure declarations ----------------------------------------------------*/
typedef enum {
  LORA_OK = 0,
  LORA_ERROR,
  LORA_BUSY,
  LORA_TIMEOUT
} LoRa_Status;

/* Function prototypes -------------------------------------------------------*/
LoRa_Status LoRa_init(void);

LoRa_Status LoRa_startAsRX(void);
LoRa_Status LoRa_startAsTX(void);
LoRa_Status LoRa_stop(void);

LoRa_Status LoRa_send(uint8_t *data, uint16_t length);
LoRa_Status LoRa_receive(uint8_t *buffer, uint16_t *length);

#endif /* LORA_COMM_H */ 
