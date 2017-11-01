#include "BMScomms.h"

uint8_t bmscommsMasks[]  = {0xC0, 0xE0, 0xE0};
char    bmscommsValues[] = {0xC0, 0xA0, 0x40};
uint8_t bmscommsErrorPacketCount = 0;

// Encode a packet
uint8_t * bmscomms_encode_packet(uint16_t milliVolts, uint8_t status, uint8_t balancing){
  bmscommsDataArr[0] = ((milliVolts - BMSCOMMS_MILLIVOLT_OFFSET) >> 5) | 0xC0;
  bmscommsDataArr[1] = (milliVolts - BMSCOMMS_MILLIVOLT_OFFSET) & 0x1F | (0x5 << 5);
  bmscommsDataArr[2] = (0x2 << 5) | ((status & 0x7) << 2) | (balancing & 0x3);
  return bmscommsDataArr;
}

#ifdef __WITH_AVRLIBC__

void bmscomms_init_suart( void ){
    softuart_init(BMSCOMMS_BAUD);
}

void bmscomms_process_packet(uint16_t *milliVolts, uint8_t *status, uint8_t *balancing) {
  // Get buffer packet start location
  uint8_t buff_out = softuart_decr_index(softuart_buf_get_packetstartpos());
  
  // Get bytes from buffer
  for (uint8_t i = 0; i < BMSCOMMS_PACKET_LENGTH; i++) {
    buff_out = softuart_incr_index(buff_out);
    bmscommsDataArr[i] = softuart_buf_get(buff_out);
  }
  
  // Check if we have exhausted the ring buffer
  if (buff_out == softuart_buf_get_inpos() ){
    softuart_buf_set_newdata(0);
  }
  
  // Set inpos to buff_out
  softuart_buf_set_outpos(buff_out);
  
  *milliVolts = (((bmscommsDataArr[0] & 0x3F) << 5) | (bmscommsDataArr[1] & 0x1F)) + BMSCOMMS_MILLIVOLT_OFFSET;
  *status = (bmscommsDataArr[2] & 0x1C) >> 2;
  *balancing = (bmscommsDataArr[2] & 0x3);
}


uint8_t bmscomms_find_and_process_packet(uint16_t *milliVolts, uint8_t *status, uint8_t *balancing) {
  if (softuart_find_packet(bmscommsValues, bmscommsMasks, BMSCOMMS_PACKET_LENGTH) == 0) {
      // Packet found, begin processing
      bmscomms_process_packet(milliVolts, status, balancing);
      bmscommsErrorPacketCount = 0;
      return BMSCOMMS_SUCCESS;
  } else {
    if (++bmscommsErrorPacketCount >= BMSCOMMS_ERROR_PACKETS && *status == BMSCOMMS_STATUS_OK) {
      *status = BMSCOMMS_STATUS_FAULT;
    }
    return BMSCOMMS_FAILURE;
  }
}

#endif
