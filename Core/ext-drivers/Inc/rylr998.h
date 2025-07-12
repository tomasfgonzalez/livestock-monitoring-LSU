/**
  ******************************************************************************
  * @author         : Tomas Gonzalez
  * @file           : rylr998.h
  * @brief          : Header for RYLR998 driver
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RYLR998_H__
#define __RYLR998_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

/* Defines -------------------------------------------------------------------*/
#define AT "AT+"
#define END "\r\n"

#define TX_BUFFER_SIZE 128
#define RX_BUFFER_SIZE 64

/* Type definitions ----------------------------------------------------------*/
typedef enum {
	RYLR_OK = 0x00U,
	//RYLR_ADDRESS,
	RYLR_RCV,
	RYLR_RCV_ERR,
	RYLR_RCV_ACK,
	//RYLR_RDY,
	//RYLR_IPR,
	//RYLR_UID,
	//RYLR_VER,
	RYLR_FACTORY,
	//RYLR_RESET,
	//RYLR_READY,
	RYLR_ERR,
	RYLR_NOT_FOUND
} RYLR_RX_command_t;

typedef struct {
	uint8_t networkId;        // valid range: 3-15, 18(default)
	uint16_t address; 		    // 0~65535 (default 0)
	uint8_t SF;						    // SF7to SF9 at 125kHz, SF7 to SF10 at 250kHz, and SF7 to SF11 at 500kHz
	uint8_t BW;						    // 7: 125 KHz (default) | 8: 250 KHz | 9: 500 KHz
	uint8_t CR;						    // 1: Coding Rate 4/5 | 2: Coding Rate 4/6 | 3: Coding Rate 4/7 | 4: Coding Rate 4/8
	uint8_t ProgramedPreamble;// When NETWORKID=18, The value can be configured to 4~24. Other NETWORKID can only be configured to 12
	uint8_t mode;					    // 0: Transceiver mode (default) | 1: Sleep mode | 2 : Smart receiving power saving mode
	uint32_t rxTime;				  // RX time = 30ms~60000ms, (default 1000)
	uint32_t LowSpeedTime;		// Low speed time = 30ms~60000ms, (default 1000)
	uint32_t baudRate;				// valid range 300-115200(default)
	uint32_t frequency;				// 915000000: 915000000Hz
	uint8_t memory; 				  // saves in flash memory = 1
	char password[8];				  // 8 chars
	uint8_t CRFOP; 					  // 22: 22dBm(default) 21: 21dBm 20: 20dBm ... 01: 1dBm 00: 0dBm
} RYLR_config_t;

typedef struct {
	uint16_t id;
	uint8_t byte_count;
	char data[64];            // LoRa suports up to 240 data char, this must less or equal RxBuff var in UART file
	uint8_t rssi;             // Always negative
	uint8_t snr;
} RYLR_RX_data_t;

typedef struct {
    const char *prefix;
    RYLR_RX_command_t command;
} RYLR_CommandEntry;

/* Public functions ----------------------------------------------------------*/

/**
 * @brief Set the channel and address
 * 
 * @param ch 
 * @param address 
 */
void rylr998_setChannel(uint8_t ch, uint8_t address);

/**
 * @brief Configure the LoRa module
 * 
 * @param config_handler 
 */
void rylr998_config(const RYLR_config_t *config_handler);

/**
 * @brief Get the command from the LoRa module
 * When received a message through command RCV we get the following:
 * +RCV=<Address>,<Length>,<Data>,<RSSI>,<SNR>,
 * <Address> Transmitter Address ID
 * <Length> Data Length
 * <Data> ASCII Data
 * <RSSI> Received Signal Strength
 * <SNR> Signal-to-noise ratio
 * @param cmd: command to get
 * @return Pointer to the deserialized message
 */
RYLR_RX_data_t* rylr998_getCommand(RYLR_RX_command_t cmd);

/**
 * @brief Reads the latest stored packet received
 * 
 * @return Pointer to the deserialized message
 */
RYLR_RX_data_t* rylr998_readCurrentPacket(void);

/**
 * @brief Send a command to the LoRa module
 * 
 * @param cmd 
 */
void rylr998_sendCommand(const char *cmd);

/**
 * @brief Set the interrupt flag
 * 
 * @param val 
 */
void rylr998_SetInterruptFlag(uint8_t val);

/**
 * @brief Get the interrupt flag
 */
uint8_t rylr998_GetInterruptFlag(void);

/**
 * @brief Check if there is an error
 * 
 * @return true iff there is an error
 */
bool rylr998_hasError(void);

#ifdef __cplusplus
}
#endif

#endif /* __RYLR998_H__ */
