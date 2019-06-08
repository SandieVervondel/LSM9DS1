/******************************************************************************
I2cCommunication.cpp
I2C Communication Implementation Source File
Vervondel
Original Creation Date: Januari 12, 2019

Distributed as-is; no warranty is given.
******************************************************************************/

#include <I2cCommunication.h>
#include <I2c.h>


I2cCommunication::I2cCommunication()
{
}


bool I2cCommunication::init(uint8_t pBus, uint8_t pI2cAddr)
{
  _i2c_address = pI2cAddr;
  
  if(initI2C_RW(pBus, pI2cAddr, 0) > 0)
	  return false;

  return true;
}


void I2cCommunication::writeByte(uint8_t pRegister, uint8_t pData)
{
  uint8_t buf[2] = { pRegister, pData };

  if(write(i2C_file, buf, 2) != 2)
  {
  	rt_printf("Failed to write register %d\n", pRegister);
	return;
  }
}

uint8_t I2cCommunication::readByte(uint8_t pRegister)
{
  i2c_char_t inbuf, outbuf;
  struct i2c_rdwr_ioctl_data packets;
  struct i2c_msg messages[2];

  /*
   * In order to read a register, we first do a "dummy write" by writing
   * 0 bytes to the register we want to read from.  This is similar to
   * the packet in set_i2c_register, except it's 1 byte rather than 2.
   */
  outbuf = pRegister;
  messages[0].addr  = _i2c_address;
  messages[0].flags = 0;
  messages[0].len   = sizeof(outbuf);
  messages[0].buf   = &outbuf;

  /* The data will get returned in this structure */
  messages[1].addr  = _i2c_address;
  messages[1].flags = I2C_M_RD; // | I2C_M_NOSTART;
  messages[1].len   = sizeof(inbuf);
  messages[1].buf   = &inbuf;

  /* Send the request to the kernel and get the result back */
  packets.msgs      = messages;
  packets.nmsgs     = 2;
  if(ioctl(i2C_file, I2C_RDWR, &packets) < 0)
  {
    rt_printf("readByte Unable to send data\n");
    return 0;
  }

  return inbuf;
}

uint8_t I2cCommunication::readBytes(uint8_t pRegister, uint8_t * pDest, uint8_t pCount)
{
  i2c_char_t outbuf;
  struct i2c_rdwr_ioctl_data packets;
  struct i2c_msg messages[2];

  /*
   * In order to read a register, we first do a "dummy write" by writing
   * 0 bytes to the register we want to read from.  This is similar to
   * the packet in set_i2c_register, except it's 1 byte rather than 2.
   */
  outbuf = pRegister;
  messages[0].addr  = _i2c_address;
  messages[0].flags = 0;
  messages[0].len   = sizeof(outbuf);
  messages[0].buf   = &outbuf;

  /* The data will get returned in this structure */
  messages[1].addr  = _i2c_address;
  messages[1].flags = I2C_M_RD; // | I2C_M_NOSTART;
  messages[1].len   = pCount;
  messages[1].buf   = pDest;

  /* Send the request to the kernel and get the result back */
  packets.msgs      = messages;
  packets.nmsgs     = 2;
  if(ioctl(i2C_file, I2C_RDWR, &packets) < 0)
  {
    rt_printf("readBytes, Unable to read data");
    return 0;
  }
  
  return pCount;
}
