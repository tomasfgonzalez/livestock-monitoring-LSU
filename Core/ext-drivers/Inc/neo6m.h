/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : gps_parser.h
  * @brief          : Header for GPS parser file
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
#ifndef __NEO6M_H__
#define __NEO6M_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Public functions ------------------------------------------------------------ */

/**
 * Process data
 * */
void neo6m_ProcessData(uint8_t* data, uint16_t);

/**
 * Get latitude from NavPosLLH message
 * */
int32_t neo6m_GetLatitude(void);

/**
 * Get longitude from NavPosLLH message
 * */
int32_t neo6m_GetLongitude(void);

/**
 * Get time of the week UTC from NavPosLLH message
 * */
uint32_t neo6m_GetTimeOfTheWeekUTC_ms(void);

/**
 * Get fix status from NavStatus message
 * */
uint8_t neo6m_GetFixStatus(void);

#ifdef __cplusplus
}
#endif

#endif /* __NEO6M_H__ */
