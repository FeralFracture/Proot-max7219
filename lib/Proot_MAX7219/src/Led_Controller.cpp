#include "Led_Controller.h"

Led_Controller::Led_Controller(int dataPin, int csPin, int clkPin, int numDevices, int intensity, bool initializeToDefault)
{
	if (numDevices <= 0 || numDevices > 16)
		numDevices = 16;		//clamp the value to 16 if out of bounds
	deviceCount = numDevices;
	fallbackIntensity = intensity;

	//Initialize chosen pins for data connections
	SPI_MOSI = dataPin;
	SPI_CLK = clkPin;
	SPI_CS = csPin;

	pinMode(SPI_MOSI, OUTPUT);
	pinMode(SPI_CLK, OUTPUT);
	pinMode(SPI_CS, OUTPUT);

	digitalWrite(SPI_CS, HIGH);

	frameBuffer = new uint8_t * [deviceCount];
	newFrameBuffer = new uint8_t * [deviceCount];

	for (int module = 0; module < deviceCount; module++) {
		frameBuffer[module] = new uint8_t[8]();
		newFrameBuffer[module] = new uint8_t[8]();
		for (int row = 0; row < 8; row++) {
			frameBuffer[module][row] = 0x00;
			newFrameBuffer[module][row] = 0x00;
		}
	}

	for (int i = 0; i < deviceCount; i++) {
		spiTransfer(i, OP_DISPLAYTEST, 0);
		setScanLimit(i, 7);					//scanlimit is set to max on startup
		spiTransfer(i, OP_DECODEMODE, 0);	//decode is done in source
		clearDisplay(i);
		shutdown(i, true);					//we go into shutdown-mode on startup
	}
	if (initializeToDefault)
		reset();
}

void Led_Controller::setModule(int addr, face_section sect, int segment, bool flip) {
	uint64_t frame;
	switch (sect) {
	case MOUTH:
		frame = FRAME(segment, default_mouth);
		break;
	case NOSE:
		frame = FRAME(segment, default_nose);
		break;
	case EYE:
		frame = FRAME(segment, default_eye);
	}
	setModule(addr, frame, flip);
}

void Led_Controller::setModule(int addr, uint64_t frame, bool flip) {
	for (int i = 0; i < 8; i++) {
		uint8_t data = (frame >> (i * 8)) & 0xFF;
		if (frameBuffer[addr][7-i] != data) {
			if (flip)
				data = pgm_read_byte(&REVERSE_LOOKUP_TABLE[data]);
			newFrameBuffer[addr][7-i] = data;
		}
	}
}

void Led_Controller::latchDisplays() {
	for (int row = 0; row < 8; row++) {
		for (int addr = 0; addr < deviceCount; addr++) {
			// Compare current frame buffer with the next frame buffer and update accordingly per module
			if (frameBuffer[addr][row] == newFrameBuffer[addr][row])
				continue;
			// If there is a difference, update the row
			frameBuffer[addr][row] = newFrameBuffer[addr][row];
			spidata[addr * 2 + 1] = row + 1;
			spidata[addr * 2] = frameBuffer[addr][row];
		}
		spiOut();
	}
}
void Led_Controller::setIntensity(int addr, int intensity) {
	if (intensity < 0 || intensity >= 16) {
		return;
	}
	if (addr == -1)
	{
		for (int i = 0; i <= deviceCount; i++) {
			spiTransfer(i, OP_INTENSITY, intensity);
		}
		return;
	}
	if (addr < 0 || addr >= deviceCount)
		return;
	spiTransfer(addr, OP_INTENSITY, intensity);
}

void Led_Controller::spiTransfer(int addr, volatile byte opcode, volatile byte data) {
	spidata[addr * 2 + 1] = opcode;
	spidata[addr * 2] = data;
	spiOut();
}

void Led_Controller::spiOut() {
	digitalWrite(SPI_CS, LOW);			//enable the line 
	for (int i = MAX_BYTES; i > 0; i--)	//Now shift out the data 
		shiftOut(SPI_MOSI, SPI_CLK, MSBFIRST, spidata[i - 1]);
	digitalWrite(SPI_CS, HIGH);			//latch the data onto the display
}

int Led_Controller::getDeviceCount() {
	return deviceCount;
}

void Led_Controller::setScanLimit(int addr, int limit) {
	if (addr < 0 || addr >= deviceCount)
		return;
	if (limit >= 0 && limit < 8)
		spiTransfer(addr, OP_SCANLIMIT, limit);
}

void Led_Controller::clearDisplay(int addr) {
	int offset;
	if (addr == -1) {
		for (int module = 0; module <= deviceCount; module++) {
			offset = module * 8;
			for (int row = 0; row < 8; row++) {
				frameBuffer[module][row] = 0;
				newFrameBuffer[module][row] = frameBuffer[module][row];
				spiTransfer(module, row + 1, 0);
			}
		}
		return;
	}
	if (addr < 0 || addr >= deviceCount)
		return;
	offset = addr * 8;
	for (int row = 0; row < 8; row++) {
		frameBuffer[addr][row] = 0;
		newFrameBuffer[addr][row] = frameBuffer[addr][row];
		spiTransfer(addr, row + 1, 0);
	}
}

void Led_Controller::shutdown(int addr, bool b) {
	if (addr == -1)
	{
		for (int module = 0; module <= deviceCount; module++) {
			spiTransfer(module, OP_SHUTDOWN, b ? 0 : 1);
		}
		return;
	}
	if (addr < 0 || addr >= deviceCount)
		return;

	spiTransfer(addr, OP_SHUTDOWN, b ? 0 : 1);
}

void Led_Controller::reset() {
	for (int module = 0; module < deviceCount; module++) {
		spiTransfer(module, OP_DISPLAYTEST, 0);
		setScanLimit(module, 7);
		spiTransfer(module, OP_DECODEMODE, 0);
		clearDisplay(module);
		shutdown(module, true);
		for (int row = 0; row < 8; row++) {
			frameBuffer[module][row] = 0x00;
			newFrameBuffer[module][row] = 0x00;
		}
		bool flip = (module >= (deviceCount / 2));

		face_section sect;
		switch (module) {
		case 0: case 1: case 12: case 13:
			sect = EYE;
			break;
		case 6: case 7:
			sect = NOSE;
			break;
		default:
			sect = MOUTH;
		}

		int segment;
		switch (module) {
		case 1: case 3: case 10: case 12:
			segment = 1;
			break;
		case 4: case 9:
			segment = 2;
			break;
		case 5: case 8:
			segment = 3;
			break;
		default:
			segment = 0;
		}
		shutdown(module, false);
		setIntensity(module, fallbackIntensity);
		setModule(module, sect, segment, flip);
	}
	latchDisplays();
}