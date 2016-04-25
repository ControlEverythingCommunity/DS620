// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// DS620
// This code is designed to work with the DS620_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/products

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

void main() 
{
	// Create I2C bus
	int file;
	char *bus = "/dev/i2c-1";
	if((file = open(bus, O_RDWR)) < 0) 
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}
	// Get I2C device, DS620 I2C address is 0x48(72)
	ioctl(file, I2C_SLAVE, 0x48);

	// Send Start Command(0x51)
	char config[2] = {0};
	config[0] = 0x51;
	write(file, config, 1);
	// Select configuration register MSB(0xAC)
	// Conversion resolution = 13 bits, temperature conversion, continuous mode(0x0E)
	config[0] = 0xAC;
	config[1] = 0x0E;
	write(file, config, 2);
	// Select configuration register LSB(0xAD)
	// Thermostat disabled(0x00)
	config[0] = 0xAD;
	config[1] = 0x00;
	write(file, config, 2);
	sleep(1);

	// Get 2 bytes of data from the register(0xAA)
	// temp msb, temp lsb
	char reg[1] = {0xAA};
	write(file, reg, 1);
	char data[2] = {0};
	if(read(file, data, 2) != 2)
	{
		printf("Erorr : Input/output Erorr \n");
	}
	else
	{
		// Convert data the data to 13-bits
		int temp = (data[0] * 256 + (data[1] & 0xF8)) / 8;
		if(temp > 4095)
		{
			temp -= 8192;
		}
		double cTemp = temp * 0.0625;
		double fTemp = cTemp * 1.8 + 32;

		// Output data to screen
		printf("Temperature in Celsius : %.2f C \n", cTemp);
		printf("Temperature in Fahrenheit : %.2f F \n", fTemp);
	}
}
