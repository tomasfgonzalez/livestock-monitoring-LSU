/**
  ******************************************************************************
  * @author         : Tomas Gonzalez
  * @file           : rylr998.c
  * @brief          : Driver for RYLR998 LoRa module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "rylr998.h"

#include "lpuart.h"
#include "stm32l0xx_hal.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/* Private variables ----------------------------------------------------- */
static char txBuffer[TX_BUFFER_SIZE];

static bool rx_packet_available = false;
static RYLR_RX_data_t rx_packet;
RYLR_RX_command_t last_cmd;

static bool hasError = false;

/* Private constants ----------------------------------------------------- */
const RYLR_CommandEntry commandTable[] = {
  {"+OK", RYLR_OK},
  {"+RCV", RYLR_RCV},
  {"+ERR", RYLR_ERR},
  {"+READY",RYLR_READY},
	{"ACK", RYLR_RCV_ACK},
  {"+FACTORY", RYLR_FACTORY},
  {NULL, RYLR_NOT_FOUND} // Sentinel value
};

/* Private functions ----------------------------------------------------- */

//----------------------------------
// 			RX PROCESS
//----------------------------------
RYLR_RX_command_t rylr998_ResponseFind(const char *rxBuffer) {
  for (int i = 0; commandTable[i].prefix != NULL; i++) {
    if (strncmp(rxBuffer, commandTable[i].prefix, strlen(commandTable[i].prefix)) == 0) {
      return commandTable[i].command;
    }
  }
  return RYLR_NOT_FOUND;
}

static uint16_t start_indx = 0;


RYLR_RX_command_t rylr998_parse_received(uint8_t *pBuff, uint8_t pBuff_size) {
  static char aux_buff[RX_BUFFER_SIZE];  // Should match with RX_BUFFER_SIZE
  uint8_t i = 0;
  // Find the '+' start character
  while (i < pBuff_size && pBuff[(start_indx + i) % pBuff_size] != '+') i++;

  start_indx = (start_indx + i) % pBuff_size;

  // Copy the command into aux_buff
  for (i = 0; i < pBuff_size; i++) {
    aux_buff[i] = pBuff[(start_indx + i) % pBuff_size];
    if (aux_buff[i] == '\n') break;
  }
  aux_buff[i + 1] = '\0'; // Ensure null termination

  // Reset the interrupt flag
  rylr998_SetInterruptFlag(false);

  // Update the start index
  start_indx = (start_indx + i + 1) % pBuff_size;

  // Find the command in the command table
  volatile RYLR_RX_command_t cmd = rylr998_ResponseFind(aux_buff);   //se define como volatil para que no sea optimizada
  if (cmd == RYLR_OK) {
    // Do nothing
  } else if (cmd == RYLR_RCV) {
    // Initialize data as empty string
    char *ptr = aux_buff;
    memset(rx_packet.data, 0, sizeof(rx_packet.data));

    // Skip past "+RCV="
    while (*ptr && *ptr != '=') ptr++;
    if (*ptr) ptr++; // Skip '='

    /* --- Parse ID (first number after '=') --- */
    rx_packet.id = 0;
    while (*ptr >= '0' && *ptr <= '9') {
        rx_packet.id = rx_packet.id * 10 + (*ptr - '0');
        ptr++;
    }
    if (*ptr != ',') return cmd = RYLR_RCV_ERR; // Invalid format
    ptr++; // Skip ','

    /* --- Parse byte count --- */
    rx_packet.byte_count = 0;
    while (*ptr >= '0' && *ptr <= '9') {
        rx_packet.byte_count = rx_packet.byte_count * 10 + (*ptr - '0');
        ptr++;
    }
    if (*ptr != ',') return cmd = RYLR_RCV_ERR; // Invalid format
    ptr++; // Skip ','

    /* --- Parse data (copy up to byte_count or buffer size) --- */
    size_t i = 0;
    size_t max_copy = (rx_packet.byte_count < sizeof(rx_packet.data)) ?
                      rx_packet.byte_count : sizeof(rx_packet.data) - 1;
    while (i < max_copy && *ptr && *ptr != ',') {
        rx_packet.data[i++] = *ptr++;
    }
    rx_packet.data[i] = '\0';

    // Skip remaining data if needed
    while (*ptr && *ptr != ',') ptr++;
    if (*ptr != ',') return cmd = RYLR_RCV_ERR; // Invalid format
    ptr++; // Skip ','
    ptr++; // Skip '-'

    /* --- Parse RSSI --- */
    rx_packet.rssi = 0;
    while (*ptr >= '0' && *ptr <= '9') {
      rx_packet.rssi = rx_packet.rssi * 10 + (*ptr - '0');
        ptr++;
    }

    if (*ptr != ',') return cmd = RYLR_RCV_ERR; // Invalid format
    ptr++; // Skip ','

    /* --- Parse SNR --- */
    rx_packet.snr = 0;
    while (*ptr >= '0' && *ptr <= '9') {
      rx_packet.snr = rx_packet.snr * 10 + (*ptr - '0');
      ptr++;
    }

    /* --- Process received data --- */
    if (rylr998_ResponseFind(rx_packet.data) == RYLR_RCV_ACK) {
      cmd = RYLR_RCV_ACK;
    }
    rx_packet_available = true;
  } else if (cmd == RYLR_ERR) {
    hasError = true;
  }
  return cmd;
}

//----------------------------------
// 			CONFIG COMMANDS
//----------------------------------

static void rylr998_setAddress(const uint8_t address) {
	memset(txBuffer, 0, sizeof(TX_BUFFER_SIZE));
  snprintf(txBuffer, sizeof(txBuffer), AT "ADDRESS=%d" END, address);
  rylr998_sendCommand(txBuffer);
}

static void rylr998_networkId(const uint8_t networkId){
	memset(txBuffer, 0, sizeof(TX_BUFFER_SIZE));
	snprintf(txBuffer, TX_BUFFER_SIZE, AT "NETWORKID=%u" END, networkId);
	rylr998_sendCommand(txBuffer);

}

static void rylr998_setParameter(const uint8_t SF,const uint8_t BW,const uint8_t CR,const uint8_t ProgramedPreamble){
	memset(txBuffer, 0, sizeof(TX_BUFFER_SIZE));
	snprintf(txBuffer, TX_BUFFER_SIZE, AT "PARAMETER=%u,%u,%u,%u" END, SF, BW, CR, ProgramedPreamble);
	rylr998_sendCommand(txBuffer);
}

static void rylr998_mode(const uint8_t mode,const uint32_t rxTime,const uint32_t LowSpeedTime){
	memset(txBuffer, 0, sizeof(TX_BUFFER_SIZE));
	if (rxTime==0||LowSpeedTime==0){snprintf(txBuffer, TX_BUFFER_SIZE, AT"MODE=%u" END, mode);
	}else{snprintf(txBuffer, TX_BUFFER_SIZE, AT"MODE=2,%lu,%lu" END,rxTime,LowSpeedTime);}
	rylr998_sendCommand(txBuffer);
}

// void rylr998_setBaudRate(uint32_t baudRate){
// 	memset(txBuffer, 0, sizeof(TX_BUFFER_SIZE));
// 	snprintf(txBuffer, TX_BUFFER_SIZE,  AT "IPR=%lu" END, baudRate);
// 	rylr998_sendCommand(txBuffer);
// }

static void rylr998_setBand(const uint32_t frequency,const uint8_t memory){
	memset(txBuffer, 0, sizeof(TX_BUFFER_SIZE));
	if(memory){snprintf(txBuffer, TX_BUFFER_SIZE, AT"BAND=%lu,M"END,frequency);
	}else{snprintf(txBuffer, TX_BUFFER_SIZE, AT"BAND=%lu,M"END,frequency);}
	rylr998_sendCommand(txBuffer);
}

static void rylr998_setCRFOP(const uint8_t CRFOP){
	memset(txBuffer, 0, sizeof(TX_BUFFER_SIZE));
	snprintf(txBuffer, TX_BUFFER_SIZE,  AT"CRFOP=%u"END, CRFOP);
	rylr998_sendCommand(txBuffer);
}

// void rylr998_FACTORY(void){
// 	memset(txBuffer, 0, sizeof(TX_BUFFER_SIZE));
// 	snprintf(txBuffer, TX_BUFFER_SIZE,  AT"FACTORY"END);
// 	rylr998_sendCommand(txBuffer);
// }

void rylr998_config(const RYLR_config_t *config_handler){
	//rylr998_FACTORY();
	//rylr998_getCommand(RYLR_FACTORY);

	rylr998_networkId(config_handler->networkId);

	if(last_cmd !=RYLR_OK){
	}

	rylr998_setAddress(config_handler->address);
	if(last_cmd !=RYLR_OK){
		}

	rylr998_setParameter(config_handler->SF, config_handler->BW, config_handler->CR, config_handler->ProgramedPreamble);
	if(last_cmd !=RYLR_OK){
	}

	rylr998_mode(config_handler->mode,config_handler->rxTime,config_handler->LowSpeedTime);
	if(last_cmd !=RYLR_OK){
	}

	//rylr998_setBaudRate(config_handler->baudRate);
	//rylr998_getCommand(RYLR_IPR); //ADD RYLR_IPR

	rylr998_setBand(config_handler->frequency,config_handler->memory);
	if(last_cmd !=RYLR_OK){
	}

	//rylr998_setCPIN(config_handler->password);
	//rylr998_getCommand(RYLR_OK);

	rylr998_setCRFOP(config_handler->CRFOP);
	if(last_cmd !=RYLR_OK){
	}
}

/* Public functions ----------------------------------------------------- */

//----------------------------------
// 		 IRQ FLAG
//----------------------------------

volatile uint8_t rylr998_interrupt_flag;
volatile uint16_t rylr998_last_rx;

void rylr998_SetInterruptFlag(uint8_t val){
	rylr998_interrupt_flag = val;
}

uint8_t rylr998_GetInterruptFlag(void){
	return rylr998_interrupt_flag;
}

//----------------------------------
// 		 CONFIGURE
//----------------------------------

void rylr998_setChannel(uint8_t channel, uint8_t address) {
	RYLR_config_t config_handler;

  // TODO: Add password
	if (channel == 1) {
		/* MAIN CHANNEL ------------------*/
		config_handler.networkId = 18;
		config_handler.address = address;
		config_handler.SF = 9;
		config_handler.BW = 7;
		config_handler.CR = 1;
		config_handler.ProgramedPreamble = 12;
		config_handler.mode = 0;
		config_handler.rxTime = 0;
		config_handler.LowSpeedTime = 0;
		config_handler.frequency = 915000000;
		config_handler.memory = 1;
		//strcpy(config_handler.password, "FFFFFFFF");
		config_handler.CRFOP = 22;
	} else {
		/* AUX CHANNEL ------------------*/
		config_handler.networkId = 18;
		config_handler.address = address;
		config_handler.SF = 9;
		config_handler.BW = 7;
		config_handler.CR = 1;
		config_handler.ProgramedPreamble = 12;
		config_handler.mode = 0;
		config_handler.rxTime = 0;
		config_handler.LowSpeedTime = 0;
		config_handler.frequency = 925000000;
		config_handler.memory = 1;
		//strcpy(config_handler.password, "FFFFFFFF");
		config_handler.CRFOP = 22;
	}
	rylr998_config(&config_handler);
}
//-----------------------------------------------
// 			SEND AND GET DATA FROM LPUART
//------------------------------------------------
void rylr998_sendCommand(const char *cmd) {
  HAL_UART_Transmit(&hlpuart1, (uint8_t *)cmd, strlen(cmd), 20);
  HAL_Delay(60);
}

/*RYLR_RX_data_t* rylr998_getCommand(RYLR_RX_command_t cmd){
	uint8_t* rx_buff = LPUART_getRxBuff();

	// Without delay, the flag does not get set to 1
	// This part of the code is delicate
	HAL_Delay(30);
	while (!rylr998_interrupt_flag);


	if (rylr998_parse_received(rx_buff, LPUART_RX_SIZE) != cmd) {
		// Wrong command
		return NULL;
	}

	return &rx_packet;
}*/


void rylr998_processcmd(void){
	last_cmd = rylr998_parse_received(LPUART_getRxBuff(), LPUART_RX_SIZE);
}

RYLR_RX_command_t getlast_cmd(void){
	return last_cmd;
}

void reset_rx_pointer(void){
	start_indx=0;
}

RYLR_RX_data_t* rylr998_readCurrentPacket(void) {
	if (rx_packet_available) {
		rx_packet_available=false;
		return &rx_packet;
	}
	return NULL;
}

bool rylr998_hasError(void) {
	return hasError;
}
