#ifndef PROGMEM_READER_H
#define PROGMEM_READER_H
#include <Arduino.h>

#define FRAME(index, section) (                                           \
	((uint64_t)pgm_read_byte(((uint8_t *)(section + index)) + 7) << 56) | \
	((uint64_t)pgm_read_byte(((uint8_t *)(section + index)) + 6) << 48) | \
	((uint64_t)pgm_read_byte(((uint8_t *)(section + index)) + 5) << 40) | \
	((uint64_t)pgm_read_byte(((uint8_t *)(section + index)) + 4) << 32) | \
	((uint64_t)pgm_read_byte(((uint8_t *)(section + index)) + 3) << 24) | \
	((uint64_t)pgm_read_byte(((uint8_t *)(section + index)) + 2) << 16) | \
	((uint64_t)pgm_read_byte(((uint8_t *)(section + index)) + 1) << 8) |  \
	((uint64_t)pgm_read_byte(((uint8_t *)(section + index)) + 0)))

#endif