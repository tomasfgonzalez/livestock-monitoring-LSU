#include "max30102_for_stm32_hal.h"
#include <process_HR.h>
#ifdef __cplusplus
extern "C"
{
#endif

max30102_t max30102;

void max30102_irq_check(void){
if (max30102_has_interrupt(&max30102)){
	  		max30102_interrupt_handler(&max30102);
	  }else if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8) == GPIO_PIN_RESET && !max30102_has_interrupt(&max30102)){
		  max30102_init_conf();
	  }
}



void max30102_init_conf(void){

	  max30102_init(&max30102,&hi2c1);
	  max30102_reset(&max30102);
	  max30102_clear_fifo(&max30102);
	  // FIFO configurations
	  max30102_set_fifo_config(&max30102, max30102_smp_ave_1, 0, 0);
	  // LED configurations
	  max30102_set_led_pulse_width(&max30102, max30102_pw_16_bit);
	   max30102_set_adc_resolution(&max30102, max30102_adc_4096);
	   max30102_set_sampling_rate(&max30102, max30102_sr_400);
	   max30102_set_led_current_1(&max30102, 8);
	   //max30102_set_led_current_2(&max30102, 8);

	  // Set HR mode
	  max30102_set_mode(&max30102, max30102_heart_rate);
	  // Enable FIFO_A_FULL interrupt
	  max30102_set_a_full(&max30102, 1);

}


uint8_t payload[4];

/**
 * @brief MAX30102 initiation function.
 *
 * @param obj Pointer to max30102_t object instance.
 * @param hi2c Pointer to I2C object handle
 */
void max30102_init(max30102_t *obj, I2C_HandleTypeDef *hi2c)
{
    obj->_ui2c = hi2c;
    obj->_interrupt_flag = 0;
    memset(obj->_ir_samples, 0, MAX30102_SAMPLE_LEN_MAX * sizeof(uint32_t));
    memset(obj->_red_samples, 0, MAX30102_SAMPLE_LEN_MAX * sizeof(uint32_t));
}

/**
 * @brief Write buffer of buflen bytes to a register of the MAX30102.
 *
 * @param obj Pointer to max30102_t object instance.
 * @param reg Register address to write to.
 * @param buf Pointer containing the bytes to write.
 * @param buflen Number of bytes to write.
 */
void max30102_write(max30102_t *obj, uint8_t reg, uint8_t *buf, uint16_t buflen)
{
    payload[0] = reg;
    if (buf != NULL && buflen != 0)
        memcpy(payload + 1, buf, buflen);
    HAL_I2C_Master_Transmit(obj->_ui2c, MAX30102_I2C_ADDR << 1, payload, buflen + 1, MAX30102_I2C_TIMEOUT);
}

/**
 * @brief Read buflen bytes from a register of the MAX30102 and store to buffer.
 *
 * @param obj Pointer to max30102_t object instance.
 * @param reg Register address to read from.
 * @param buf Pointer to the array to write to.
 * @param buflen Number of bytes to read.
 */
void max30102_read(max30102_t *obj, uint8_t reg, uint8_t *buf, uint16_t buflen)
{
    uint8_t reg_addr = reg;
    HAL_I2C_Master_Transmit(obj->_ui2c, MAX30102_I2C_ADDR << 1, &reg_addr, 1, MAX30102_I2C_TIMEOUT);
    HAL_I2C_Master_Receive(obj->_ui2c, MAX30102_I2C_ADDR << 1, buf, buflen, MAX30102_I2C_TIMEOUT);
}

/**
 * @brief Reset the sensor.
 *
 * @param obj Pointer to max30102_t object instance.
 */
void max30102_reset(max30102_t *obj)
{
    uint8_t val = 0x40;
    max30102_write(obj, MAX30102_MODE_CONFIG, &val, 1);
}

/**
 * @brief Enable A_FULL interrupt.
 *
 * @param obj Pointer to max30102_t object instance.
 * @param enable Enable (1) or disable (0).
 */
void max30102_set_a_full(max30102_t *obj, uint8_t enable)
{
    uint8_t reg = 0;
    max30102_read(obj, MAX30102_INTERRUPT_ENABLE_1, &reg, 1);
    reg &= ~(0x01 << MAX30102_INTERRUPT_A_FULL);
    reg |= ((enable & 0x01) << MAX30102_INTERRUPT_A_FULL);
    max30102_write(obj, MAX30102_INTERRUPT_ENABLE_1, &reg, 1);
}

/**
 * @brief Enable PPG_RDY interrupt.
 *
 * @param obj Pointer to max30102_t object instance.
 * @param enable Enable (1) or disable (0).
 */
void max30102_set_ppg_rdy(max30102_t *obj, uint8_t enable)
{
    uint8_t reg = 0;
    max30102_read(obj, MAX30102_INTERRUPT_ENABLE_1, &reg, 1);
    reg &= ~(0x01 << MAX30102_INTERRUPT_PPG_RDY);
    reg |= ((enable & 0x01) << MAX30102_INTERRUPT_PPG_RDY);
    max30102_write(obj, MAX30102_INTERRUPT_ENABLE_1, &reg, 1);
}

/**
 * @brief Enable ALC_OVF interrupt.
 *
 * @param obj Pointer to max30102_t object instance.
 * @param enable Enable (1) or disable (0).
 */
void max30102_set_alc_ovf(max30102_t *obj, uint8_t enable)
{
    uint8_t reg = 0;
    max30102_read(obj, MAX30102_INTERRUPT_ENABLE_1, &reg, 1);
    reg &= ~(0x01 << MAX30102_INTERRUPT_ALC_OVF);
    reg |= ((enable & 0x01) << MAX30102_INTERRUPT_ALC_OVF);
    max30102_write(obj, MAX30102_INTERRUPT_ENABLE_1, &reg, 1);
}


/**
 * @brief Set interrupt flag on interrupt. To be called in the corresponding external interrupt handler.
 *
 * @param obj Pointer to max30102_t object instance.
 */
void max30102_on_interrupt(max30102_t *obj)
{
    obj->_interrupt_flag = 1;
}

/**
 * @brief Check whether the interrupt flag is active.
 *
 * @param obj Pointer to max30102_t object instance.
 * @return uint8_t Active (1) or inactive (0).
 */
uint8_t max30102_has_interrupt(max30102_t *obj)
{
    return obj->_interrupt_flag;
}

/**
 * @brief Read interrupt status registers (0x00 and 0x01) and perform corresponding tasks.
 *
 * @param obj Pointer to max30102_t object instance.
 */
void max30102_interrupt_handler(max30102_t *obj)
{
    uint8_t reg[2] = {0x00};
    // Interrupt flag in registers 0x00 and 0x01 are cleared on read
    max30102_read(obj, MAX30102_INTERRUPT_STATUS_1, reg, 2);

    if ((reg[0] >> MAX30102_INTERRUPT_A_FULL) & 0x01) {
           // FIFO almost full
           max30102_read_fifo(obj);
           process_buffer();
       }


    // Reset interrupt flag
    obj->_interrupt_flag = 0;
}

/**
 * @brief Shutdown the sensor.
 *
 * @param obj Pointer to max30102_t object instance.
 * @param shdn Shutdown bit.
 */
void max30102_shutdown(max30102_t *obj, uint8_t shdn)
{
    uint8_t config;
    max30102_read(obj, MAX30102_MODE_CONFIG, &config, 1);
    config = (config & 0x7f) | (shdn << MAX30102_MODE_SHDN);
    max30102_write(obj, MAX30102_MODE_CONFIG, &config, 1);
}

/**
 * @brief Set shutdown mode, can read and write registers, but IRQ are off
 */

void max30102_set_shutdown(void){
	max30102_shutdown(&max30102,1);
}


/**
 * @brief Set measurement mode.
 *
 * @param obj Pointer to max30102_t object instance.
 * @param mode Measurement mode enum (max30102_mode_t).
 */
void max30102_set_mode(max30102_t *obj, max30102_mode_t mode)
{
    uint8_t config;
    max30102_read(obj, MAX30102_MODE_CONFIG, &config, 1);
    config = (config & 0xf8) | mode;
    max30102_write(obj, MAX30102_MODE_CONFIG, &config, 1);
    max30102_clear_fifo(obj);
}

/**
 * @brief Set sampling rate.
 *
 * @param obj Pointer to max30102_t object instance.
 * @param sr Sampling rate enum (max30102_spo2_st_t).
 */
void max30102_set_sampling_rate(max30102_t *obj, max30102_sr_t sr)
{
    uint8_t config;
    max30102_read(obj, MAX30102_SPO2_CONFIG, &config, 1);
    config = (config & 0x63) | (sr << MAX30102_SPO2_SR);
    max30102_write(obj, MAX30102_SPO2_CONFIG, &config, 1);
}

/**
 * @brief Set led pulse width.
 *
 * @param obj Pointer to max30102_t object instance.
 * @param pw Pulse width enum (max30102_led_pw_t).
 */
void max30102_set_led_pulse_width(max30102_t *obj, max30102_led_pw_t pw)
{
    uint8_t config;
    max30102_read(obj, MAX30102_SPO2_CONFIG, &config, 1);
    config = (config & 0x7c) | (pw << MAX30102_SPO2_LED_PW);
    max30102_write(obj, MAX30102_SPO2_CONFIG, &config, 1);
}

/**
 * @brief Set ADC resolution.
 *
 * @param obj Pointer to max30102_t object instance.
 * @param adc ADC resolution enum (max30102_adc_t).
 */
void max30102_set_adc_resolution(max30102_t *obj, max30102_adc_t adc)
{
    uint8_t config;
    max30102_read(obj, MAX30102_SPO2_CONFIG, &config, 1);
    config = (config & 0x1f) | (adc << MAX30102_SPO2_ADC_RGE);
    max30102_write(obj, MAX30102_SPO2_CONFIG, &config, 1);
}

/**
 * @brief Set LED current.
 *
 * @param obj Pointer to max30102_t object instance.
 * @param ma LED current float (0 < ma < 51.0).
 */
void max30102_set_led_current_1(max30102_t *obj, float ma)
{
    uint8_t pa = ma / 0.2;
    max30102_write(obj, MAX30102_LED_IR_PA1, &pa, 1);
}

/**
 * @brief Set LED current.
 *
 * @param obj Pointer to max30102_t object instance.
 * @param ma LED current float (0 < ma < 51.0).
 */
void max30102_set_led_current_2(max30102_t *obj, float ma)
{
    uint8_t pa = ma / 0.2;
    max30102_write(obj, MAX30102_LED_RED_PA2, &pa, 1);
}


/**
 * @brief
 *
 * @param obj Pointer to max30102_t object instance.
 * @param smp_ave
 * @param roll_over_en Roll over enabled(1) or disabled(0).
 * @param fifo_a_full Number of empty samples when A_FULL interrupt issued (0 < fifo_a_full < 15).
 */
void max30102_set_fifo_config(max30102_t *obj, max30102_smp_ave_t smp_ave, uint8_t roll_over_en, uint8_t fifo_a_full)
{
    uint8_t config = 0x00;
    config |= smp_ave << MAX30102_FIFO_CONFIG_SMP_AVE;
    config |= ((roll_over_en & 0x01) << MAX30102_FIFO_CONFIG_ROLL_OVER_EN);
    config |= ((fifo_a_full & 0x0f) << MAX30102_FIFO_CONFIG_FIFO_A_FULL);
    max30102_write(obj, MAX30102_FIFO_CONFIG, &config, 1);
}

/**
 * @brief Clear all FIFO pointers in the sensor.
 *
 * @param obj Pointer to max30102_t object instance.
 */

uint32_t start_time = 0;


void max30102_clear_fifo(max30102_t *obj)
{
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
void max30102_read_fifo(max30102_t *obj)
{
    // First transaction: Get the FIFO_WR_PTR
    uint8_t wr_ptr = 0, rd_ptr = 0;
    max30102_read(obj, MAX30102_FIFO_WR_PTR, &wr_ptr, 1);
    max30102_read(obj, MAX30102_FIFO_RD_PTR, &rd_ptr, 1);

    int8_t num_samples;



    num_samples = (int8_t)wr_ptr - (int8_t)rd_ptr;
    if (num_samples < 1)
    {
        num_samples += 32;
    }
    uint16_t red_sample_16=0;
    // Second transaction: Read NUM_SAMPLES_TO_READ samples from the FIFO
    if (num_samples > 0) {
        for (int8_t i = 0; i < num_samples; i++) {
            uint8_t sample[6];
            max30102_read(obj, MAX30102_FIFO_DATA, sample, 6);
            red_sample_16 = (int16_t)(((uint32_t)(sample[3]) << 16 | (uint32_t)(sample[4]) << 8 | sample[5]) >> 2) & 0xFFFF;

            max30102_put_Buffer(red_sample_16);
        }
    }

}



#ifdef __cplusplus
}
#endif
