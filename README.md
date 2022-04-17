# as5048b_raspi
AMS AS5048B library to interface the sensor with Raspberry Pi

This is a class library file to interface the non-contact rotary poisition sensor AMS AS5048B using I2C with Raspberry Pi Zero W, Zero 2 W, 3 B+, and 4 B. Testing has been conducted using Raspeberry Pi Zero W.

This class library uses the wiringPiI2C library for iinterfacing the sensor using the wiringPiI2CReadReg8() and the wiringPiI2CWriteReg8() functions.
