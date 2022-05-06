#ifndef AS5048B_H
#define AS5048B_H

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <wiringPiI2C.h>

#include <cstdlib>

using namespace std;

// Default addresses for AS5048B

#define AS5048_ADDRESS 0x40  // 0b10000 + ( A1 & A2 to GND)
#define AS5048B_PROG_REG 0x03
#define AS5048B_ADDR_REG 0x15
#define AS5048B_ZEROMSB_REG 0x16  // bits 0..7
#define AS5048B_ZEROLSB_REG 0x17  // bits 0..5
#define AS5048B_GAIN_REG 0xFA
#define AS5048B_DIAG_REG 0xFB
#define AS5048B_MAGNMSB_REG 0xFC    // bits 0..7
#define AS5048B_MAGNLSB_REG 0xFD    // bits 0..5
#define AS5048B_ANGLMSB_REG 0xFE    // bits 0..7
#define AS5048B_ANGLLSB_REG 0xFF    // bits 0..5
#define AS5048B_RESOLUTION 16384.0  // 14 bits

// unit consts - just to make the units more readable

#define U_RAW 1
#define U_TRN 2
#define U_DEG 3
#define U_RAD 4
#define U_GRAD 5
#define U_MOA 6
#define U_SOA 7
#define U_MILNATO 8
#define U_MILSE 9
#define U_MILRU 10

class AS5048B {
 private:
  int file;
  int adapter_nr;
  char filename[20];
  int addr;
  char buf[10];

  // variables
  bool _debugFlag;
  uint8_t chipAddress;
  uint8_t _addressRegVal;
  uint16_t _zeroRegVal;
  double _lastAngleRaw;
  double _movingAvgExpAngle;
  double _movingAvgExpSin;
  double _movingAvgExpCos;
  double _movingAvgExpAlpha;
  int _movingAvgCountLoop;
  uint8_t value_MSB;
  uint8_t value_LSB;
  uint16_t value_16bit;
  uint16_t value_14bit;
  double raw_ang;

 public:
  AS5048B(uint8_t);

  // methods
  void I2CSetup(void);
  double convertAngle(
      int,
      double);  // RAW, TRN, DEG, RAD, GRAD, MOA, SOA, MILNATO, MILSE, MILRU

  double getExpAvgRawAngle(void);
  void progRegister(
      uint8_t regVal);    // nothing so far - manipulate the OTP register
  void doProg(void);      // progress programming slave address OTP
  void doProgZero(void);  // progress programming zero position OTP
  void setZeroReg(void);  // set Zero to current angle position
  void zeroRegW(uint16_t regVal);  // write Zero register value
  uint16_t zeroRegR(void);         // read Zero register value
  void burnZero(void);
  uint16_t angleRegR(void);  // read raw value of the angle register
  double angle(int unit = U_RAW, bool newVal = true);
  uint8_t diagR(void);  // read diagnostic register
};

#endif
