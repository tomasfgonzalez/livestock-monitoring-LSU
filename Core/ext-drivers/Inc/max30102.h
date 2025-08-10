/**
  ******************************************************************************
  * @author         : Unknown
  * @file           : max30102.h
  * @brief          : Header for MAX30102 heart rate sensor
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
#ifndef __MAX30102_H__
#define __MAX30102_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

#include <stdint.h>
#include <stdbool.h>

/* Defines -------------------------------------------------------------------*/
#define MAX30102_I2C_ADDR 0x57
#define MAX30102_I2C_TIMEOUT 1000

#define MAX30102_BYTES_PER_SAMPLE 6
#define MAX30102_SAMPLE_LEN_MAX 32

#define MAX30102_INTERRUPT_STATUS_1 0x00
#define MAX30102_INTERRUPT_STATUS_2 0x01
#define MAX30102_INTERRUPT_ENABLE_1 0x02
#define MAX30102_INTERRUPT_ENABLE_2 0x03
#define MAX30102_INTERRUPT_A_FULL 7
#define MAX30102_INTERRUPT_PPG_RDY 6
#define MAX30102_INTERRUPT_ALC_OVF 5
#define MAX30102_INTERRUPT_DIE_TEMP_RDY 1

#define MAX30102_FIFO_WR_PTR 0x04
#define MAX30102_OVF_COUNTER 0x05
#define MAX30102_FIFO_RD_PTR 0x06

#define MAX30102_FIFO_DATA 0x07

#define MAX30102_FIFO_CONFIG 0x08
#define MAX30102_FIFO_CONFIG_SMP_AVE 5
#define MAX30102_FIFO_CONFIG_ROLL_OVER_EN 4
#define MAX30102_FIFO_CONFIG_FIFO_A_FULL 0

#define MAX30102_MODE_CONFIG 0x09
#define MAX30102_MODE_SHDN 7
#define MAX30102_MODE_RESET 6
#define MAX30102_MODE_MODE 0

#define MAX30102_SPO2_CONFIG 0x0a
#define MAX30102_SPO2_ADC_RGE 5
#define MAX30102_SPO2_SR 2
#define MAX30102_SPO2_LED_PW 0

#define MAX30102_LED1_PA1 0x0c
#define MAX30102_LED2_PA2 0x0d

#define MAX30102_MULTI_LED_CTRL_1 0x11
#define MAX30102_MULTI_LED_CTRL_SLOT2 4
#define MAX30102_MULTI_LED_CTRL_SLOT1 0
#define MAX30102_MULTI_LED_CTRL_2 0x12
#define MAX30102_MULTI_LED_CTRL_SLOT4 4
#define MAX30102_MULTI_LED_CTRL_SLOT3 0

#define MAX30102_DIE_TINT 0x1f
#define MAX30102_DIE_TFRAC 0x20
#define MAX30102_DIE_TFRAC_INCREMENT 0.0625f
#define MAX30102_DIE_TEMP_CONFIG 0x21
#define MAX30102_DIE_TEMP_EN 1

/* Structure declarations ----------------------------------------------------*/
typedef enum max30102_mode_t {
  max30102_heart_rate = 0x02,
  max30102_spo2 = 0x03,
  max30102_multi_led = 0x07
} max30102_mode_t;

typedef enum max30102_smp_ave_t {
  max30102_smp_ave_1,
  max30102_smp_ave_2,
  max30102_smp_ave_4,
  max30102_smp_ave_8,
  max30102_smp_ave_16,
  max30102_smp_ave_32,
} max30102_smp_ave_t;

typedef enum max30102_sr_t {
  max30102_sr_50,
  max30102_sr_100,
  max30102_sr_200,
  max30102_sr_400,
  max30102_sr_800,
  max30102_sr_1000,
  max30102_sr_1600,
  max30102_sr_3200
} max30102_sr_t;

typedef enum max30102_led_pw_t {
  max30102_pw_15_bit,
  max30102_pw_16_bit,
  max30102_pw_17_bit,
  max30102_pw_18_bit
} max30102_led_pw_t;

typedef enum max30102_adc_t {
  max30102_adc_2048,
  max30102_adc_4096,
  max30102_adc_8192,
  max30102_adc_16384
} max30102_adc_t;

typedef enum max30102_multi_led_ctrl_t {
  max30102_led_off,
  max30102_led_red,
  max30102_led_ir
} max30102_multi_led_ctrl_t;

typedef struct max30102_t {
  I2C_HandleTypeDef *_ui2c;
  uint32_t _ir_samples[32];
  uint32_t _red_samples[32];
} max30102_t;

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief Initialize the MAX30102 sensor.
 */
void max30102_Init(void);

/**
 * @brief Start the MAX30102 sensor, putting it in normal operation mode.
 */
void max30102_Start(void);

/**
 * @brief Stop the MAX30102 sensor, putting it in shutdown mode.
 */
void max30102_Stop(void);

/**
 * @brief Check if the MAX30102 sensor is ready.
 * @return True if the sensor is ready, false otherwise.
 */
bool max30102_IsReady(void);

/**
 * @brief Get the BPM from the MAX30102 sensor.
 * @return The BPM value.
 */
uint16_t max30102_GetBPM(void);

#ifdef __cplusplus
}
#endif

#endif /* MAX30102_H */
