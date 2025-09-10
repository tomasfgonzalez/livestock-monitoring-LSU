/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : lsuPayload.h
  * @brief          : Header for LSU payload management, handles shared payload
  *                   data between main and backup transmission FSMs
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
#ifndef __LSU_PAYLOAD_H__
#define __LSU_PAYLOAD_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ---------------------------------------------------------------*/
#include "lsu_comms.h"

/* Public functions ------------------------------------------------------- */
void lsuPayload_set(const LSU_Payload* payload);
const LSU_Payload* lsuPayload_get(void);
bool lsuPayload_isValid(void);

#ifdef __cplusplus
}
#endif

#endif /* LSU_PAYLOAD_H__ */
