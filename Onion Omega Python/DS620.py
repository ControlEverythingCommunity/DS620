# Distributed with a free-will license.
# Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
# DS620
# This code is designed to work with the DS620_I2CS I2C Mini Module available from ControlEverything.com.
# https://www.controleverything.com/products

from OmegaExpansion import onionI2C
import time

# Get I2C bus
i2c = onionI2C.OnionI2C()

# DS620 address, 0x48(72)
#		0x51(81)	Select Start command
data = [0x51]
i2c.write(0x48, data)
# DS620 address, 0x48(72)
# Select configuration register MSB, 0xAC(172)
#		0x0E(14)	Continous conversion, Temperature conversion ON
#					Resolution = 13 bit
i2c.writeByte(0x48, 0xAC, 0x0E)
# DS620 address, 0x48(72)
# Select configuration register LSB, 0xAD(173)
#		0x00(00)	Thermostate disabled
i2c.writeByte(0x48, 0xAD, 0x00)

time.sleep(0.5)

# DS620 addres, 0x48(72)
# Read data back from 0xAA(170), 2 bytes, MSB first
data = i2c.readBytes(0x48, 0xAA, 2)

# Convert the data to 13-bits
cTemp = (data[0] * 256 + (data[1] & 0xF8)) / 8
if cTemp > 4095 :
	cTemp -= 8192
cTemp = cTemp * 0.0625
fTemp = cTemp * 1.8 + 32

# Output data to screen
print "Temperature in Celsius : %.2f C" %cTemp
print "Temperature in Fahrenheit : %.2f F" %fTemp
