/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : tests.c
  * @brief          : Source file for tests, holds functions to test the device
  ******************************************************************************
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <data.h>
#include "gpio.h"




struct data_package pkt;



void adc_data(void) {
	uint16_t Voltage_val[2];
    HAL_Delay(200);
    ADC_GetValues(Voltage_val);

    pkt.V_T1 = Voltage_val[0];
    pkt.V_T2 = Voltage_val[1];
}


void gps_data(void) {
	HAL_Delay(200);
	// Obtener latitud y longitud
	pkt.Lat = get_UBX_GpsLatitude();
	pkt.Lon = get_UBX_GpsLongitude();
	pkt.fixStatus = get_UBX_GpsFixStatus();

	// GPS FIX STATUS: 0 NO FIX, 1 DEAD RECKONING ONLY, 2 2D FIX, 3 3D FIX,
	// 4 GPS + DEAD RECKONING COMBINED, 5 TIME ONLY FIX

}



void hr_data(void){
	max30102_Init();
	max30102_Start();

	while (!max30102_IsReady());

	pkt.bpm = max30102_GetBPM();

	max30102_Stop();
}


void run_sleep_mode(uint32_t ticks) {
  HAL_SuspendTick();
  RTC_setWakeUpTimer(ticks);

  /* Enter STOP 2 mode */
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

  RTC_clearWakeUpTimer();
  SystemClock_Config();
  HAL_ResumeTick();

  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
  HAL_Delay(1000);
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
}




	int32_t get_Lat(){
		return pkt.Lat;
	}

	int32_t get_Lon(){
		return pkt.Lon;
	}


	uint8_t get_fixStatus(){
		return pkt.fixStatus;
	}

	uint16_t get_V_T1(){
		return pkt.V_T1;
	}

	uint16_t get_V_T2(){
			return pkt.V_T2;
	}

	uint16_t get_bpm(){
			return pkt.bpm;
	}





