// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// DS620
// This code is designed to work with the DS620_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/products

import com.pi4j.io.i2c.I2CBus;
import com.pi4j.io.i2c.I2CDevice;
import com.pi4j.io.i2c.I2CFactory;
import java.io.IOException;

public class DS620
{
	public static void main(String args[]) throws Exception
	{
		// Create I2C bus
		I2CBus bus = I2CFactory.getInstance(I2CBus.BUS_1);
		// Get I2C device, DS620 I2C address is 0x48(72)
		I2CDevice device = bus.getDevice(0x48);

		// Send Start Command
		device.write((byte)0x51);
		// Select configuration register MSB
		// Conversion resolution = 13 bits, temperature conversion, continuous mode
		device.write(0xAC, (byte)0x0E);
		// Select configuration register LSB
		// Thermostat disabled
		device.write(0xAD, (byte)0x00);
		Thread.sleep(250);

		// Get 2 bytes of data from the address 0xAA(170)
		// temp msb, temp lsb
		byte[] data = new byte[2];
		device.read(0xAA, data, 0, 2);

		// Convert data the data to 13-bits
		int temp = ((data[0] & 0xFF) * 256 + (data[1] & 0xF8)) / 8;
		if(temp > 4095)
		{
			temp -= 8192;
		}
		double cTemp = temp * 0.0625;
		double fTemp = cTemp * 1.8 + 32;
		
		// Output data to screen
		System.out.printf("Temperature in Celsius : %.2f C %n", cTemp);
		System.out.printf("Temperature in Fahrenheit : %.2f F %n", fTemp);
	}
}
