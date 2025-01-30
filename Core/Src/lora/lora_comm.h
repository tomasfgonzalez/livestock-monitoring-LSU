#ifndef LORA_COMM_H
#define LORA_COMM_H

typedef enum {
    LORA_OK = 0,
    LORA_ERROR,
    LORA_BUSY,
    LORA_TIMEOUT
} LoRa_Status;

LoRa_Status LoRa_init(void);

LoRa_Status LoRa_startAsRX(void);
LoRa_Status LoRa_startAsTX(void);
LoRa_Status LoRa_stop(void);

LoRa_Status LoRa_send(uint8_t *data, uint16_t length);
LoRa_Status LoRa_receive(uint8_t *buffer, uint16_t *length);

#endif /* LORA_COMM_H */ 
