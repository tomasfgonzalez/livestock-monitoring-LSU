/*
 * rylr998.h
 *
 *  Created on: Mar 14, 2025
 *      Author: Tomas Francisco Gonzalez
 */

#ifndef INC_RYLR998_H_
#define INC_RYLR998_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------- */
#include "stm32l0xx_hal.h"
#include "usart.h"

/* Defines --------------------------------------------------------------- */
#define END "\r\n"
#define AT "AT+"

#define TX_BUFFER_SIZE 128 //118


typedef enum
{
	RYLR_OK = 0x00U,
	//RYLR_ADDRESS,
	RYLR_RCV,
	RYLR_RCV_ERR,
	RYLR_RCV_ACK,
	//RYLR_RDY,
	//RYLR_IPR,
	//RYLR_UID,
	//RYLR_VER,
	//RYLR_FACTORY,
	//RYLR_RESET,
	//RYLR_READY,
	RYLR_ERR,
	RYLR_NOT_FOUND

} RYLR_RX_command_t;

typedef struct{
	uint8_t networkId;      		//valid range: 3-15, 18(default)
	uint16_t address; 				//0~65535 (default 0)
	uint8_t SF;						//*SF7to SF9 at 125kHz, SF7 to SF10 at 250kHz, and SF7 to SF11 at 500kHz
	uint8_t BW;						// 7: 125 KHz (default) 8: 250 KHz 9: 500 KHz
	uint8_t CR;						//1: Coding Rate 4/5 2 :Coding Rate 4/6  3 :Coding Rate 4/7  4 :Coding Rate 4/8
	uint8_t ProgramedPreamble;      //When NETWORKID=18, The value can be configured to 4~24. Other NETWORKID can only be configured to 12
	uint8_t mode;					//0：Transceiver mode (default). 1：Sleep mode.  2 : Smart receiving power saving mode
	uint32_t rxTime;				//<RX time>=30ms~60000ms, (default 1000)
	uint32_t LowSpeedTime;			//<Low speed time>=30ms~60000ms, (default 1000)
	uint32_t baudRate;				//valid range 300-115200(default)
	uint32_t frequency;				//915000000: 915000000Hz ?				Set the frequency as 868500000Hz. AT+BAND=868500000
	uint8_t memory; 				// saves in flash memory=1
	char password[8];				// 8 chars
	uint8_t CRFOP; 					//22: 22dBm(default) 21: 21dBm 20: 20dBm ... 01: 1dBm 00: 0dBm
}RYLR_config_t;



/*
 * +RCV=<Address>,<Length>,<Data>,<RSSI>,<SNR>,
	<Address> Transmitter Address ID
	<Length> Data Length
	<Data> ASCII Data
	<RSSI> Received Signal Strength
	<SNR> Signal-to-noise ratio
 */

typedef struct{
	uint16_t id;
	uint8_t byte_count;
	char data[64];    //LoRa suports up to 240 data char, this must less or equal RxBuff var in UART file
	uint8_t rssi;      //Always negative
	uint8_t snr;
} RYLR_RX_data_t;

typedef struct {
    const char *prefix;
    RYLR_RX_command_t command;
} RYLR_CommandEntry;


//Tx CFG
void rylr998_setChannel(uint8_t ch,uint8_t address);
void rylr998_config(const RYLR_config_t *config_handler);


RYLR_RX_data_t* rylr998_getCommand(RYLR_RX_command_t cmd);
RYLR_RX_data_t* rylr998_readCurrentPacket(void);
void rylr998_sendCommand(const char *cmd);

//IRQ
void rylr998_SetInterruptFlag(uint8_t val);
uint8_t rylr998_GetInterruptFlag(void);

#ifdef __cplusplus
}
#endif

#endif /* INC_RYLR998_H_ */