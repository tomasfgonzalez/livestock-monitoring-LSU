/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : tim2.h
  * @brief          : Header for TIM2 file
  ******************************************************************************
  */
#ifndef __TIM2_H
#define __TIM2_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32l0xx_hal.h"

extern TIM_HandleTypeDef htim2;

// Function to configure and initialize TIM2
void TIM2_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __TIM2_H */
