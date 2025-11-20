#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H
#include <Arduino.h>
#include <progmem_reader.h>

// the opcodes for the MAX7221 and MAX7219
#define OP_NOOP 0
#define OP_DIGIT0 1
#define OP_DIGIT1 2
#define OP_DIGIT2 3
#define OP_DIGIT3 4
#define OP_DIGIT4 5
#define OP_DIGIT5 6
#define OP_DIGIT6 7
#define OP_DIGIT7 8
#define OP_DECODEMODE 9
#define OP_INTENSITY 10
#define OP_SCANLIMIT 11
#define OP_SHUTDOWN 12
#define OP_DISPLAYTEST 15

#define MAX_BYTES deviceCount * 2

const uint64_t default_mouth[] PROGMEM = {
	0x0080fce6723e0000,
	0xe07f0f0100000000,
	0x0f3c60c080000000,
	0x30180c0703000000};
const uint64_t default_eye[] PROGMEM = {
	// 0xFFFFFFFFFFFFFFFF,
	// 0xFFFFFFFFFFFFFFFF
	0x80f0f8fcfefffe00,
	0x030f7fffff0f0100
};
const uint64_t default_nose[] PROGMEM = {
	0x00000080c0c0c0f0};

enum face_section
{
	EYE,
	MOUTH,
	NOSE
};

class Led_Controller
{
private:
	uint8_t **frameBuffer;
	uint8_t **newFrameBuffer;
	byte spidata[32];
	byte status[128];
	uint8_t SPI_MOSI,
			SPI_CLK,
			SPI_CS,
			deviceCount,
			fallbackIntensity;
	void spiTransfer(uint8_t addr, byte opcode, byte data);
	void spiOut();

public:
	bool autoLatch = false;

	Led_Controller(uint8_t dataPin, uint8_t csPin, uint8_t clkPin, uint8_t numDevices = 1, uint8_t intensity = 1, bool initializeToDefault = false);
	uint8_t Led_Controller::reverse8(uint8_t x);
	void setModule(uint8_t addr, face_section sect, uint8_t segment = 0, bool flip = false);
	void setModule(uint8_t addr, uint64_t frame, bool flip = false);
	void latchDisplays();
	void reset();
	int getDeviceCount();
	void setScanLimit(uint8_t addr, uint8_t limit);
	void setIntensity(uint8_t addr, uint8_t intensity);
	void shutdown(uint8_t addr, bool status);
	void clearDisplay(uint8_t addr);
};
#endif
