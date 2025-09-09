/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : lsu_payload.c
  * @brief          : Source file for LSU payload management, handles shared payload
  *                   data between main and backup transmission FSMs
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
#include "lsu_payload.h"

#include <stddef.h>

/* Private variables ----------------------------------------------------------*/
static LSU_Payload lsu_payload;
static bool payload_valid = false;

/* Public functions ------------------------------------------------------- */
void lsuPayload_set(const LSU_Payload* payload) {
  if (payload != NULL) {
    lsu_payload = *payload;
    payload_valid = true;
  }
}

const LSU_Payload* lsuPayload_get(void) {
  if (payload_valid) {
    return &lsu_payload;
  }
  return NULL;
}

bool lsuPayload_isValid(void) {
  return payload_valid;
}
