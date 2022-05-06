#include "AS5048B.h"

AS5048B::AS5048B(uint8_t addr = AS5048_ADDRESS) {
  // Constructor sets address of the encoder. Default address is 0x40
  chipAddress = addr;
  AS5048B::I2CSetup();
}

void AS5048B::I2CSetup() {
  // I2C setup
  file = wiringPiI2CSetup(chipAddress);

  if (file == -1) {
    printf("Failed to init I2C communication.\n");
    usleep(1000);
    exit(0);
  }

  printf("I2C communication successfully setup.\n");
  usleep(1000);
}

double AS5048B::angle(int unit, bool newVal) {
  // Function that outputs the encoder angle in the desired unit
  value_16bit = wiringPiI2CReadReg16(
      file, AS5048B_ANGLMSB_REG);  // Read the MSB and the LSB register

  // most significant bit
  value_MSB =
      value_16bit &
      0xFF;  // MSB is already in position, just cut of the unwanted part

  value_LSB = value_16bit >> 8;  // shift LSB to the right position

  value_14bit = ((uint16_t)value_MSB)
                << 6;  // cast the 8 bit value to 16bit size and shift the MSB

  // value 6 position to the left

  value_14bit +=
      (value_LSB &
       0x3f);  // mask/remove the top two bits of LSB and add the other to the

  // previous result to get the final result
  raw_ang = (double)value_14bit;  // cast the unsigned 16 bit value to double

  // floating point value
  return AS5048B::convertAngle(unit, raw_ang);
}

double AS5048B::convertAngle(int unit, double angle) {
  // convert raw sensor reading into angle unit
  double angleConv;

  switch (unit) {
    case U_RAW:
      // Sensor raw measurement
      angleConv = angle;
      break;

    case U_TRN:
      // full turn ratio
      angleConv = (angle / AS5048B_RESOLUTION);
      break;

    case U_DEG:
      // degree
      angleConv = (angle / AS5048B_RESOLUTION) * 360.0;
      break;

    case U_RAD:
      // Radian
      angleConv = (angle / AS5048B_RESOLUTION) * 2 * 3.14;
      break;

    case U_MOA:
      // minute of arc
      angleConv = (angle / AS5048B_RESOLUTION) * 60.0 * 360.0;
      break;

    case U_SOA:
      // second of arc
      angleConv = (angle / AS5048B_RESOLUTION) * 60.0 * 60.0 * 360.0;
      break;

    case U_GRAD:
      // grade
      angleConv = (angle / AS5048B_RESOLUTION) * 400.0;
      break;

    case U_MILNATO:
      // NATO MIL
      angleConv = (angle / AS5048B_RESOLUTION) * 6400.0;
      break;

    case U_MILSE:
      // Swedish MIL
      angleConv = (angle / AS5048B_RESOLUTION) * 6300.0;
      break;

    case U_MILRU:
      // Russian MIL
      angleConv = (angle / AS5048B_RESOLUTION) * 6000.0;
      break;

    default:
      // no conversion => raw angle
      angleConv = angle;
      break;
  }

  return angleConv;
}

void AS5048B::doProgZero(void) {
  // this will burn the zero position OTP register as described in the datasheet 
  // enable programming mode
  AS5048B::progRegister(0x01);

  usleep(10000);

  // set the burn bit: enables automatic programming procedure
  AS5048B::progRegister(0x08);
  usleep(10000);

  // read angle information (equals to 0)
  AS5048B::angle(U_RAW, true);
  usleep(10000);

  // enable verification
  AS5048B::progRegister(0x40);
  usleep(10000);

  // read angle information (equals to 0)
  AS5048B::angle(U_RAW, true);
  usleep(10000);
}

void AS5048B::progRegister(uint8_t regVal) {
  wiringPiI2CWriteReg8(file, AS5048B_PROG_REG, regVal);
}

void AS5048B::setZeroReg(void) {
  AS5048B::zeroRegW((uint16_t)0x00);
  uint16_t newZero = AS5048B::angle(U_RAW, true);
  AS5048B::zeroRegW(newZero);
}

void AS5048B::zeroRegW(uint16_t regVal) {
  wiringPiI2CWriteReg8(file, AS5048B_ZEROMSB_REG, (uint8_t)(regVal >> 6));
  wiringPiI2CWriteReg8(file, AS5048B_ZEROLSB_REG, (uint8_t)(regVal & 0x3F));
}

void AS5048B::burnZero(void) {
  AS5048B::setZeroReg();
  AS5048B::doProgZero();
}
