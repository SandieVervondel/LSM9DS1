/******************************************************************************
I2cCommunication.h
I2C Communication Implementation Library Header File
Vervondel
Original Creation Date: Januari 12, 2019

Distributed as-is; no warranty is given.
******************************************************************************/
#ifndef __I2CCommunication_H__
#define __I2CCommunication_H__

#include "Bela.h"
#include <I2c.h>


class I2cCommunication : public I2c
{

public:
	I2cCommunication();
		
	///////////////////
	// I2C Functions //
	///////////////////

	// init() -- Initialize the I2C hardware.
	// This function will setup all I2C pins and related hardware.
    bool init(uint8_t bus, uint8_t i2caddr);
	
	// writeByte() -- Write a byte out of I2C to a register in the device
	// Input:
	//	- register = The register to be written to.
	//	- data = Byte to be written to the register.
	void writeByte(uint8_t pRegister, uint8_t pData);
	
	// readByte() -- Read a single byte from a register over I2C.
	// Input:
	//	- register = The register to be read from.
	// Output:
	//	- The byte read from the requested address.
	uint8_t readByte(uint8_t pRegister);
	
	// readBytes() -- Read a series of bytes, starting at a register via SPI
	// Input:
	//	- register = The register to begin reading.
	// 	- * dest = Pointer to an array where we'll store the readings.
	//	- count = Number of registers to be read.
	// Output: No value is returned by the function, but the registers read are
	// 		all stored in the *dest array given.
	uint8_t readBytes(uint8_t pRegister, uint8_t * pDest, uint8_t pCount);
	
	int readI2C() { return 0; } // Unused
	
private:
    int _i2c_address;
};

#endif // I2CCommunication_H //
