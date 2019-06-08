#include <Bela.h>
#include <Scope.h>
#include <cmath>
#include "LSM9DS1.h"

// Define this to print data to terminal
#define DEBUG_LSM9DS1

// Change this to change how often the LSM is read (in Hz)
int readInterval = 50;

// instantiate the scope
Scope scope;

LSM9DS1 lsm;	
AuxiliaryTask lsmTask;	// Auxiliary task to read I2C

int readCount = 0;			// How long until we read again...
int readIntervalSamples = 0; // How many samples between reads

void readLSM(void*);

bool setup(BelaContext *context, void *userData)
{
	lsm.settings.device.mAddress = 0x1E;
    lsm.settings.device.agAddress = 0x6B;
	if(!lsm.begin())
	{
	  rt_printf("Error initialising LSM\n");
	  return false;
	}
	
	lsmTask = Bela_createAuxiliaryTask(readLSM, 50, "bela-lsm");
	readIntervalSamples = context->audioSampleRate / readInterval;

	return true;
}

void render(BelaContext *context, void *userData)
{
	for (unsigned int n=0; n<context->audioFrames; n++)
	{
		if (++readCount >= readIntervalSamples)
		{
			readCount = 0;
			Bela_scheduleAuxiliaryTask(lsmTask);
		}
	}

}

void cleanup(BelaContext *context, void *userData)
{ }

// Auxiliary task to read the lsm board
void readLSM(void*)
{
  // Update the sensor values whenever new data is available
  if ( lsm.gyroAvailable() )
  {
    // To read from the gyroscope,  first call the
    // readGyro() function. When it exits, it'll update the
    // gx, gy, and gz variables with the most current data.
    lsm.readGyro();
  }
  if ( lsm.accelAvailable() )
  {
    // To read from the accelerometer, first call the
    // readAccel() function. When it exits, it'll update the
    // ax, ay, and az variables with the most current data.
    lsm.readAccel();
  }
  if ( lsm.magAvailable() )
  {
    // To read from the magnetometer, first call the
    // readMag() function. When it exits, it'll update the
    // mx, my, and mz variables with the most current data.
    lsm.readMag();
  }	
	
#ifdef DEBUG_LSM9DS1
    rt_printf("%d ,%d, %d\n", lsm.ax, lsm.ay, lsm.az);
#endif
	
}
