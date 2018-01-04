/******************************************************************************
 * 
 * 
 * 
 * 
 *****************************************************************************/
#pragma once
#include <stdint.h>
#ifdef __WITH_AVRLIBC__
  #include "../softuart/softuart.h"
#endif

#define BMSCOMMS_PACKET_LENGTH        3
#define BMSCOMMS_BAUD                 4800

// No comms error count
#ifndef BMSCOMMS_ERROR_PACKETS
#define BMSCOMMS_ERROR_PACKETS        5
#endif

// exit statuses
#define BMSCOMMS_SUCCESS              0
#define BMSCOMMS_FAILURE              1

// Statuses
#define BMSCOMMS_STATUS_OK            0
#define BMSCOMMS_STATUS_FAULT         1
#define BMSCOMMS_STATUS_OVER_VOLTAGE  2
#define BMSCOMMS_STATUS_OVER_TEMP     3
#define BMSCOMMS_STATUS_UNDER_VOLTAGE 4

// Balancing
#define BMSCOMMS_BALANCE_NONE         0
#define BMSCOMMS_BALANCE_SOME         1
#define BMSCOMMS_BALANCE_ALL          2

#define BMSCOMMS_MILLIVOLT_OFFSET     1652
#define BMSCOMMS_MILLIVOLTS_MAX       3700

static uint8_t bmscommsDataArr[BMSCOMMS_PACKET_LENGTH];

uint8_t * bmscomms_encode_packet(uint16_t milliVolts, uint8_t status, uint8_t balancing);


#ifdef __WITH_AVRLIBC__

void bmscomms_init_suart( void );

void bmscomms_process_packet(uint16_t *milliVolts, uint8_t *status, uint8_t *balancing);

uint8_t bmscomms_find_and_process_packet(uint16_t *milliVolts, uint8_t *status, uint8_t *balancing);

#endif
