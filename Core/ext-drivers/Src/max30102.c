/**
  ******************************************************************************
  * @authors        : Unknown
  * @file           : max30102.c
  * @brief          : Driver for MAX30102 heart rate sensor
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

/* Includes ------------------------------------------------------------------*/
#include "max30102.h"
#include "max30102_Buffer.h"
#include "max30102_Process.h"
#include "i2c.h"

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

/* Private variables ---------------------------------------------------------*/
max30102_t max30102;

static bool isInitialized = false;

/* Private functions ---------------------------------------------------------*/

/**
 * @brief Write buffer of buflen bytes to a register of the MAX30102.
 *
 * @param obj Pointer to max30102_t object instance.
 * @param reg Register address to write to.
 * @param buf Pointer containing the bytes to write.
 * @param buflen Number of bytes to write.
 */
void max30102_write(max30102_t* obj, uint8_t reg, uint8_t* buf, uint16_t buflen) {
  uint8_t* payload = (uint8_t*)malloc((buflen + 1) * sizeof(uint8_t));
  *payload = reg;
  if (buf != NULL && buflen != 0) memcpy(payload + 1, buf, buflen);

  HAL_I2C_Master_Transmit(obj->_ui2c, MAX30102_I2C_ADDR << 1, payload, buflen + 1, MAX30102_I2C_TIMEOUT);
  free(payload);
}

/**
 * @brief Read buflen bytes from a register of the MAX30102 and store to buffer.
 *
 * @param obj Pointer to max30102_t object instance.
 * @param reg Register address to read from.
 * @param buf Pointer to the array to write to.
 * @param buflen Number of bytes to read.
 */
void max30102_read(max30102_t* obj, uint8_t reg, uint8_t* buf, uint16_t buflen) {
  uint8_t reg_addr = reg;
  HAL_I2C_Master_Transmit(obj->_ui2c, MAX30102_I2C_ADDR << 1, &reg_addr, 1, MAX30102_I2C_TIMEOUT);
  HAL_I2C_Master_Receive(obj->_ui2c, MAX30102_I2C_ADDR << 1, buf, buflen, MAX30102_I2C_TIMEOUT);
}

/**
 * @brief MAX30102 initiation function.
 *
 * @param obj Pointer to max30102_t object instance.
 * @param hi2c Pointer to I2C object handle
 */
void max30102_init(max30102_t* obj, I2C_HandleTypeDef* hi2c) {
  obj->_ui2c = hi2c;
  memset(obj->_ir_samples, 0, MAX30102_SAMPLE_LEN_MAX * sizeof(uint32_t));
  memset(obj->_red_samples, 0, MAX30102_SAMPLE_LEN_MAX * sizeof(uint32_t));

  uint8_t reset_config = 0x40;
  max30102_write(obj, MAX30102_MODE_CONFIG, &reset_config, 1);
}

/**
 * @brief MAX30102 reset function.
 *
 * @param obj Pointer to max30102_t object instance.
 */
void max30102_reset(max30102_t *obj) {
  uint8_t val = 0x40;
  max30102_write(obj, MAX30102_MODE_CONFIG, &val, 1);
}

/**
 * @brief Enable A_FULL interrupt.
 *
 * @param obj Pointer to max30102_t object instance.
 * @param enable Enable (1) or disable (0).
 */
void max30102_set_a_full(max30102_t* obj, uint8_t enable) {
  uint8_t reg = 0;
  max30102_read(obj, MAX30102_INTERRUPT_ENABLE_1, &reg, 1);
  reg &= ~(0x01 << MAX30102_INTERRUPT_A_FULL);
  reg |= ((enable & 0x01) << MAX30102_INTERRUPT_A_FULL);
  max30102_write(obj, MAX30102_INTERRUPT_ENABLE_1, &reg, 1);
}

/**
 * @brief Shutdown the sensor.
 *
 * @param obj Pointer to max30102_t object instance.
 * @param shdn Shutdown bit.
 */
void max30102_set_shutdown(max30102_t* obj, uint8_t shdn) {
  uint8_t config;
  max30102_read(obj, MAX30102_MODE_CONFIG, &config, 1);
  config = (config & 0x7f) | (shdn << MAX30102_MODE_SHDN);
  max30102_write(obj, MAX30102_MODE_CONFIG, &config, 1);
}

/**
 * @brief Clear all FIFO pointers in the sensor.
 *
 * @param obj Pointer to max30102_t object instance.
 */
void max30102_clear_fifo(max30102_t* obj) {
  uint8_t val = 0x00;
  max30102_write(obj, MAX30102_FIFO_WR_PTR, &val, 3);
  max30102_write(obj, MAX30102_FIFO_RD_PTR, &val, 3);
  max30102_write(obj, MAX30102_OVF_COUNTER, &val, 3);
}

/**
 * @brief Read FIFO content and store to buffer in max30102_t object instance.
 *
 * @param obj Pointer to max30102_t object instance.
 */
void max30102_read_fifo(max30102_t* obj) {
  // First transaction: Get the FIFO_WR_PTR
  uint8_t wr_ptr = 0, rd_ptr = 0;
  max30102_read(obj, MAX30102_FIFO_WR_PTR, &wr_ptr, 1);
  max30102_read(obj, MAX30102_FIFO_RD_PTR, &rd_ptr, 1);

  int8_t num_samples;

  num_samples = (int8_t)wr_ptr - (int8_t)rd_ptr;
  if (num_samples < 1) {
    num_samples += 32;
  }

  uint16_t red_sample_16 = 0;
  // Second transaction: Read NUM_SAMPLES_TO_READ samples from the FIFO
  if (num_samples > 0) {
    for (int8_t i = 0; i < num_samples; i++) {
      uint8_t sample[6];
      max30102_read(obj, MAX30102_FIFO_DATA, sample, 6);
      red_sample_16 = (int16_t)(((uint32_t)(sample[3]) << 16 | (uint32_t)(sample[4]) << 8 | sample[5]) >> 2) & 0xFFFF;

      max30102_Buffer_Put(red_sample_16);
    }
  }
}

/* Public functions ------------------------------------------------------------*/
void max30102_Init(void) {
  max30102_Buffer_Init();

  max30102_init(&max30102, &hi2c1);
  max30102_reset(&max30102);
  max30102_clear_fifo(&max30102);

  uint8_t fifo_config = (max30102_smp_ave_1 << MAX30102_FIFO_CONFIG_SMP_AVE);
  max30102_write(&max30102, MAX30102_FIFO_CONFIG, &fifo_config, 1);

  uint8_t spo2_config
    = (max30102_adc_4096 << MAX30102_SPO2_ADC_RGE)  // ADC Resolution
    | (max30102_sr_400 << MAX30102_SPO2_SR)         // Sampling Rate
    | (max30102_pw_16_bit << MAX30102_SPO2_LED_PW); // LED Pulse Width
  max30102_write(&max30102, MAX30102_SPO2_CONFIG, &spo2_config, 1);

  uint8_t led_pa = 40;
  max30102_write(&max30102, MAX30102_LED_IR_PA1, &led_pa, 1);

  uint8_t mode_config = max30102_heart_rate;
  max30102_write(&max30102, MAX30102_MODE_CONFIG, &mode_config, 1);

  // Clear FIFO and enable interrupt
  max30102_clear_fifo(&max30102);
  max30102_set_a_full(&max30102, 1);

  isInitialized = true;
  __HAL_GPIO_EXTI_GENERATE_SWIT(GPIO_PIN_8);
}

void max30102_Start(void) {
  max30102_set_shutdown(&max30102, 0);
}

void max30102_Stop(void) {
  max30102_set_shutdown(&max30102, 1);
}

bool max30102_IsReady(void) {
  return max30102_Buffer_IsReady();
}

uint16_t max30102_GetBPM(void) {
  uint16_t* buffer = max30102_Buffer_GetBuffer();
  uint16_t elapsed_time = max30102_Buffer_GetElapsedTime();
  return max30102_Process_Run(buffer, elapsed_time);
}

/* Interrupt handling ------------------------------------------------------------*/

/**
 * @brief Read interrupt status registers (0x00 and 0x01) and perform corresponding tasks.
 */
void max30102_InterruptHandler(void) {
  uint8_t reg[2] = {0x00};

  // Interrupt flag in registers 0x00 and 0x01 are cleared on read
  max30102_read(&max30102, MAX30102_INTERRUPT_STATUS_1, reg, 2);

  if ((reg[0] >> MAX30102_INTERRUPT_A_FULL) & 0x01) {
    max30102_read_fifo(&max30102);
  }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  if (GPIO_Pin == GPIO_PIN_8 && isInitialized) {
    max30102_InterruptHandler();
  }
}
