/*
 * rylr998.c
 *
 *  Created on: Mar 14, 2025
 *      Author: Tomas Francisco Gonzalez
 */
#include "rylr998.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define TX_BUFFER_SIZE 128 //118
static char uartTxBuffer[TX_BUFFER_SIZE];


//----------------------------------
// 		 IRQ FLAG -> LOOK UART FILE
//----------------------------------


volatile uint8_t rylr998_interrupt_flag;



void rylr998_SetInterruptFlag(uint8_t val){
	rylr998_interrupt_flag =val;
}


uint8_t rylr998_GetInterruptFlag(void){
	return rylr998_interrupt_flag;
}




//------------------------------
// 		 LSU CHANNELS
//------------------------------

void rylr998_setChannel(uint8_t ch,uint8_t address){
	RYLR_config_t config_handler;
	if(ch){                             //MAIN CHANNEL
	config_handler.networkId =18;
	config_handler.address =address;
	config_handler.SF=9;
	config_handler.BW=7;
	config_handler.CR=1;
	config_handler.ProgramedPreamble=12;
	config_handler.mode=0;
	config_handler.rxTime=0;
	config_handler.LowSpeedTime=0;
	//config_handler.baudRate=115200;
	config_handler.frequency=915000000;
	config_handler.memory=1;
	//strcpy(config_handler.password, "FFFFFFFF"); //we dont want the \0 terminator so we overflow, estan comentados para ver los msj
	config_handler.CRFOP=22;
	}else{     							//AUX CHANNEL
	config_handler.networkId =18;
	config_handler.address =address;
	config_handler.SF=9;
	config_handler.BW=7;
	config_handler.CR=1;
	config_handler.ProgramedPreamble=12;
	config_handler.mode=0;
	config_handler.rxTime=0;
	config_handler.LowSpeedTime=0;
	//config_handler.baudRate=115200;
	config_handler.frequency=915000000;
	config_handler.memory=1;
	//strcpy(config_handler.password, "FFFFFFFF"); //we dont want the \0 terminator so we overflow, esta comentado para no tener que config password en ambos dispositivos
	config_handler.CRFOP=22;
	}
	rylr998_config(&config_handler);
}



//------------------------------
// 		Config Commands
//------------------------------
static void rylr998_setAddress(const uint8_t address) {
	memset(uartTxBuffer, 0, sizeof(TX_BUFFER_SIZE));
    snprintf(uartTxBuffer, sizeof(uartTxBuffer), AT "ADDRESS=%d" END, address);
    rylr998_sendCommand(uartTxBuffer);
}

static void rylr998_networkId(const uint8_t networkId){
	memset(uartTxBuffer, 0, sizeof(TX_BUFFER_SIZE));
	snprintf(uartTxBuffer, TX_BUFFER_SIZE, AT "NETWORKID=%u" END, networkId);
	rylr998_sendCommand(uartTxBuffer);
}

static void rylr998_setParameter(const uint8_t SF,const uint8_t BW,const uint8_t CR,const uint8_t ProgramedPreamble){
	memset(uartTxBuffer, 0, sizeof(TX_BUFFER_SIZE));
	snprintf(uartTxBuffer, TX_BUFFER_SIZE, AT "PARAMETER=%u,%u,%u,%u" END, SF, BW, CR, ProgramedPreamble);
	rylr998_sendCommand(uartTxBuffer);
}
/*
void rylr998_reset(void){
	memset(uartTxBuffer, 0, sizeof(TX_BUFFER_SIZE));
	snprintf(uartTxBuffer, TX_BUFFER_SIZE, AT "RESET" END);
	rylr998_sendCommand(uartTxBuffer);
}*/

static void rylr998_mode(const uint8_t mode,const uint32_t rxTime,const uint32_t LowSpeedTime){
	memset(uartTxBuffer, 0, sizeof(TX_BUFFER_SIZE));
	if (rxTime==0||LowSpeedTime==0){snprintf(uartTxBuffer, TX_BUFFER_SIZE, AT"MODE=%u" END, mode);
	}else{snprintf(uartTxBuffer, TX_BUFFER_SIZE, AT"MODE=2,%lu,%lu" END,rxTime,LowSpeedTime);}
	rylr998_sendCommand(uartTxBuffer);
}

/*
void rylr998_setBaudRate(uint32_t baudRate){
	memset(uartTxBuffer, 0, sizeof(TX_BUFFER_SIZE));
	snprintf(uartTxBuffer, TX_BUFFER_SIZE,  AT "IPR=%lu" END, baudRate);
	rylr998_sendCommand(uartTxBuffer);
}*/

static void rylr998_setBand(const uint32_t frequency,const uint8_t memory){
	memset(uartTxBuffer, 0, sizeof(TX_BUFFER_SIZE));
	if(memory){snprintf(uartTxBuffer, TX_BUFFER_SIZE, AT"BAND=%lu,M"END,frequency);
	}else{snprintf(uartTxBuffer, TX_BUFFER_SIZE, AT"BAND=%lu,M"END,frequency);}
	rylr998_sendCommand(uartTxBuffer);
}

static void rylr998_setCPIN(const char *password){
	memset(uartTxBuffer, 0, sizeof(TX_BUFFER_SIZE));
	snprintf(uartTxBuffer, TX_BUFFER_SIZE,  AT "CPIN=%s"END, password);
	rylr998_sendCommand(uartTxBuffer);
}

static void rylr998_setCRFOP(const uint8_t CRFOP){
	memset(uartTxBuffer, 0, sizeof(TX_BUFFER_SIZE));
	snprintf(uartTxBuffer, TX_BUFFER_SIZE,  AT"CRFOP=%u"END, CRFOP);
	rylr998_sendCommand(uartTxBuffer);
}

/*void rylr998_FACTORY(void){
	memset(uartTxBuffer, 0, sizeof(TX_BUFFER_SIZE));
	snprintf(uartTxBuffer, TX_BUFFER_SIZE,  AT"FACTORY"END);
	rylr998_sendCommand(uartTxBuffer);
}*/

//------------------------------
// 		 CONFIG ALL SETTINGS
//------------------------------

void rylr998_config(const RYLR_config_t *config_handler){
		//rylr998_FACTORY();
		//rylr998_getCommand(RYLR_FACTORY,rx_buff,RX_BUFF);
		//NETWORKID
		rylr998_networkId(config_handler->networkId);
		rylr998_getCommand(RYLR_OK,rx_buff,RX_BUFF);
		//ADDRESS
		rylr998_setAddress(config_handler->address);
		rylr998_getCommand(RYLR_OK,rx_buff,RX_BUFF);
		//PARAMETERS
		rylr998_setParameter(config_handler->SF, config_handler->BW, config_handler->CR, config_handler->ProgramedPreamble);
		rylr998_getCommand(RYLR_OK,rx_buff,RX_BUFF);
		//MODE
		rylr998_mode(config_handler->mode,config_handler->rxTime,config_handler->LowSpeedTime);
		rylr998_getCommand(RYLR_OK,rx_buff,RX_BUFF);
		//BaudRate
		//rylr998_setBaudRate(config_handler->baudRate);
		//rylr998_getCommand(RYLR_IPR,rx_buff,RX_BUFFER_SIZE); //ADD RYLR_IPR
		//FREQ Band
		rylr998_setBand(config_handler->frequency,config_handler->memory);
		rylr998_getCommand(RYLR_OK,rx_buff,RX_BUFF);
		//PASSWORD
		//rylr998_setCPIN(config_handler->password);
		//rylr998_getCommand(RYLR_OK,rx_buff,RX_BUFF);
		//RF Output
		rylr998_setCRFOP(config_handler->CRFOP);
		rylr998_getCommand(RYLR_OK,rx_buff,RX_BUFF);
}


//------------------------------
// 				LSU
//------------------------------

int count_digits(int32_t num) {
    if (num == 0) return 1; // El número 0 tiene un solo dígito
    int count = 0;
    if (num<0){
    	count=1;
    }
    num = abs(num); // Si es negativo, tomamos su valor absoluto
    while (num > 0) {
        num /= 10;
        count++;
    }
    return count;
}

void LSU_sendParameters(uint16_t destination,int32_t Lat,int32_t Lon,uint16_t T1,uint16_t T2,uint8_t bpm){
	 memset(uartTxBuffer, 0, sizeof(TX_BUFFER_SIZE));
	 uint8_t data_length=count_digits(Lat)+count_digits(Lon)+count_digits(T1)+count_digits(T2)+count_digits(bpm)+4;// +4 por los 4 guiones
	 sprintf(uartTxBuffer, AT"SEND=%u,%u,%ld-%ld-%u-%u-%u"END, destination, data_length, Lat, Lon, T1, T2, bpm);
	 rylr998_sendCommand(uartTxBuffer);
	 HAL_Delay(500);    //el dato es largo, si mandamos un unico msj a la vez esto  no va, pero si mandamos uno atras del otro, salta el ERR-17 que significa q no se termino la Tx
	 rylr998_getCommand(RYLR_OK,rx_buff,RX_BUFF);
}

void LSU_sendSyncRequest(uint16_t destination){
	 memset(uartTxBuffer, 0, sizeof(TX_BUFFER_SIZE));
	 sprintf(uartTxBuffer, AT"SEND=%u,4,SYNC"END, destination);
	 rylr998_sendCommand(uartTxBuffer);
	 HAL_Delay(100);    //el dato es corto
	 rylr998_getCommand(RYLR_OK,rx_buff,RX_BUFF);
}





//-----------------------------------------------
// 			SEND AND GET DATA FROM LPUART1
//------------------------------------------------
void rylr998_sendCommand(const char *cmd) {
    HAL_UART_Transmit(&hlpuart1, (uint8_t *)cmd, strlen(cmd), 20);
}

void rylr998_getCommand(RYLR_RX_command_t cmd,uint8_t *rx_buff,uint8_t RX_BUFFER_SIZE){
	HAL_Delay(30);  //Sin un retardo, la bandera no llega a ponerse en 1, Esta parte del codigo resulta delicada
	while(!rylr998_interrupt_flag){
	}
	if(rylr998_GetInterruptFlag()){
				if(rylr998_prase_reciver(rx_buff,RX_BUFFER_SIZE)!=cmd){
			}else{
				//wrong command
			}
		}
}



//------------------------------
// 		 RX PROCESS
//------------------------------

const RYLR_CommandEntry commandTable[] = {
    {"+OK", RYLR_OK},
    {"+RCV", RYLR_RCV},
    {"+ERR", RYLR_ERR},
	{"ACK", RYLR_RCV_ACK},
    //{"+FACTORY", RYLR_FACTORY},
    {NULL, RYLR_NOT_FOUND} // Sentinel value
};

RYLR_RX_data_t rx_packet;

RYLR_RX_command_t rylr998_ResponseFind(const char *rxBuffer) {
    for (int i = 0; commandTable[i].prefix != NULL; i++) {
        if (strncmp(rxBuffer, commandTable[i].prefix, strlen(commandTable[i].prefix)) == 0) {
            return commandTable[i].command;
        }
    }
    return RYLR_NOT_FOUND;
}


RYLR_RX_command_t rylr998_prase_reciver(uint8_t *pBuff, uint8_t RX_BUFFER_SIZE) {
    static char aux_buff[64];  // Should match with RX_BUFFER_SIZE
    static uint8_t start_indx = 0;
    uint8_t i = 0;

    // Find the '+' start character
    while (i < RX_BUFFER_SIZE && pBuff[(start_indx + i) % RX_BUFFER_SIZE] != '+') {
        i++;
    }
    start_indx = (start_indx + i) % RX_BUFFER_SIZE;

    // Copy the command into aux_buff
    for (i = 0; i < RX_BUFFER_SIZE; i++) {
        aux_buff[i] = pBuff[(start_indx + i) % RX_BUFFER_SIZE];
        if (aux_buff[i] == '\n') break;
    }
    aux_buff[i + 1] = '\0'; // Ensure null termination

    rylr998_SetInterruptFlag(0);
    start_indx = (start_indx + i + 1) % RX_BUFFER_SIZE;

    volatile  RYLR_RX_command_t cmd = rylr998_ResponseFind(aux_buff);   //se define como volatil para que no sea optimizada
    if(cmd==RYLR_OK){

    } else if (cmd == RYLR_RCV) {
    	char *ptr = aux_buff;
    	memset(rx_packet.data, 0, sizeof(rx_packet.data));  // Initialize data as empty string

    	// Skip past "+RCV="
    	while (*ptr && *ptr != '=') ptr++;
    	if (*ptr) ptr++; // Skip '='

    	// Parse ID (first number after '=')
    	rx_packet.id = 0;
    	while (*ptr >= '0' && *ptr <= '9') {
    	    rx_packet.id = rx_packet.id * 10 + (*ptr - '0');
    	    ptr++;
    	}
    	if (*ptr != ',') return cmd = RYLR_RCV_ERR; // Invalid format
    	ptr++; // Skip ','

    	// Parse byte count
    	rx_packet.byte_count = 0;
    	while (*ptr >= '0' && *ptr <= '9') {
    	    rx_packet.byte_count = rx_packet.byte_count * 10 + (*ptr - '0');
    	    ptr++;
    	}
    	if (*ptr != ',') return cmd = RYLR_RCV_ERR; // Invalid format
    	ptr++; // Skip ','

    	// Parse data (copy up to byte_count or buffer size)
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
    	// Parse RSSI
    	rx_packet.rssi = 0;
    	while (*ptr >= '0' && *ptr <= '9') {
    		rx_packet.rssi = rx_packet.rssi * 10 + (*ptr - '0');
    	    ptr++;
    	}

    	if (*ptr != ',') return cmd = RYLR_RCV_ERR; // Invalid format
    	ptr++; // Skip ','

    	// Parse SNR
    	rx_packet.snr = 0;
    	while (*ptr >= '0' && *ptr <= '9') {
    	    rx_packet.snr = rx_packet.snr * 10 + (*ptr - '0');
    	    ptr++;
    	}

    	//------------------------------
    	// 		 PROCESS RECIVED DATA:
    	//------------------------------

    	if(rylr998_ResponseFind(rx_packet.data)==RYLR_RCV_ACK){
    		cmd = RYLR_RCV_ACK;
    	}

    	//TODO proccess recived SYNC DATA


    } else if (cmd == RYLR_ERR) {
        while (1) { Error_Handler(); } // Handle error
    }
    return cmd;
}


