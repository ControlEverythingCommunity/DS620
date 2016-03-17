// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// DS620
// This code is designed to work with the DS620_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/products

#include<application.h>
#include<spark_wiring_i2c.h>

// DS620 I2C address is 0x48(72)
#define Addr 0x48

float cTemp = 0, fTemp = 0;
void setup()
{
    // Set variable
    Particle.variable("i2cdevice", "DS620");
    Particle.variable("cTemp", cTemp);
    
    // Initialise I2C communication 
    Wire.begin();
    // Initialise Serial Communication
    Serial.begin(9600);
    
    // Start I2C Transmission
    Wire.beginTransmission(Addr);
    // Select start command
    Wire.write(0x51);
    // Stop I2C Transmission
    Wire.endTransmission();
    
    // Start I2C Transmission
    Wire.beginTransmission(Addr);
    // Select configuration msb register
    Wire.write(0xAC);
    // Conversion resolution = 13 bits ,temperature conversion on ,continuous mode on
    Wire.write(0x0E);
    // Stop I2C Transmission
    Wire.endTransmission();
    
    // Start I2C Transmission
    Wire.beginTransmission(Addr);
    // Select configuration lsb register
    Wire.write(0xAD);
    // Thermostat disabled 
    Wire.write(0x00);
    // Stop I2C Transmission
    Wire.endTransmission();
    delay(300);
}

void loop()
{
    unsigned int data[2];
    // Start I2C Transmission
    Wire.beginTransmission(Addr);
    // Select data register
    Wire.write(0xAA);
    // Stop I2C Transmission
    Wire.endTransmission();
    
    // Request 2 bytes of data
    Wire.requestFrom(Addr, 2);
  
    // Read 2 bytes of data
    // temp msb, temp lsb
    if(Wire.available() == 2)
    {
        data[0] = Wire.read();
        data[1] = Wire.read();
    }
    
    // Convert the data 
    int temp = (data[0] * 256 + (data[1] & 0xF8)) / 8;
    if(temp > 4095)
    {
        temp -= 8192; 
    }
    cTemp = (temp) * 0.0625;  
    fTemp = (cTemp * 1.8 ) + 32;
    
    // Output data to dashboard
    Particle.publish("Temperature in Celsius : ", String(cTemp));
    Particle.publish("Temperature in Fahrenheit : ", String(fTemp));
    delay(1000);
}
