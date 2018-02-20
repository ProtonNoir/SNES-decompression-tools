#ifndef _COMPRESS_H
#define _COMPRESS_H

const uint32_t WINDOW_SIZE = 4096; //0x1000
const uint32_t DATA_SIZE = 65536; //0x10000
const uint8_t TEST_BIT = 1; //0x01
const uint8_t WRITE_BIT = 128; //0x01
const uint16_t MAX_MATCH_LENGTH = 18; //0x12
const uint16_t MIN_MATCH_LENGTH = 3; //0x3

uint32_t compress(uint8_t* in_buf, uint8_t* out_buf, size_t in_size);
uint32_t decompress(uint32_t in_pos, uint8_t* in_buf, uint32_t out_pos, uint8_t* out_buf, uint16_t size);

#endif
