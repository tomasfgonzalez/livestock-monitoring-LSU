/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : tim2.h
  * @brief          : Header for TIM2 file
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
#ifndef __TIM2_H
#define __TIM2_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

/* Structure declarations ----------------------------------------------------*/
extern TIM_HandleTypeDef htim2;

/* Function prototypes -------------------------------------------------------*/
void TIM2_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __TIM2_H */
