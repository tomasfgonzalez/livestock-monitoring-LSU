/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : gps_parser.h
  * @brief          : Header for GPS parser file
  ******************************************************************************
  */

 /* Define to prevent recursive inclusion -------------------------------------*/
#ifndef GPS_PARSER_H
#define GPS_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "stm32l0xx_hal.h"
#include "gps_structures.h"

/* Global variables ----------------------------------------------------------*/
extern UBXMessage ubxMessage;

/* Internal functions --------------------------------------------------------- */
ubxMsgType processGPS(uint8_t c);

/* GPS parser functions ------------------------------------------------------- */
void processUBXData(uint8_t* data, uint16_t);
uint8_t get_UBX_GpsFixStatus(void);
int32_t get_UBX_GpsLongitude(void);
int32_t get_UBX_GpsLatitude(void);

#ifdef __cplusplus
}
#endif

#endif /* GPS_PARSER_H */
