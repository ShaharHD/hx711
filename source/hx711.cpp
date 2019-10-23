#include <stdio.h>
#include <inttypes.h>
#ifdef __arm__
#include <wiringPi.h>
#endif
#include "hx711.h"

HX711::HX711(uint8_t clockPin, uint8_t dataPin) :
  mGainBits(1),
  mScale(1.0f),
  mOffset(0),
  mClockPin(clockPin),
  mDataPin(dataPin)
{
#ifdef __arm__
  wiringPiSetupPhys();

  pinMode(mClockPin, OUTPUT);
  pinMode(mDataPin, INPUT);
#endif
}

bool HX711::isReady()
{
#ifdef __arm__
  return digitalRead(mDataPin) == LOW;
#else
  return true;
#endif
}

void HX711::setGain(uint8_t gain)
{
  switch (gain)
  {
  // channel A, gain 128
  case 128:
    this->mGainBits = 1;
    break;
  // channel A, gain 64
  case 64:
    this->mGainBits = 3;
    break;
  // channel B, gain 32
  case 32:
    this->mGainBits = 2;
    break;
  }

#ifdef __arm__
  digitalWrite(mClockPin, LOW);
  read();
#endif
}

int32_t HX711::read()
{
#ifdef __arm__
  // wait for the chip to become ready
  while (!this->isReady());

  int32_t data = 0;
  // pulse the clock pin 24 times to read the data
  for (uint8_t i = 24; i--;)
  {
    digitalWrite(mClockPin, HIGH);
    delayMicroseconds(1);
    digitalWrite(mClockPin, LOW);

    data |= (digitalRead(mDataPin) << i);
  }

  // set the channel and the gain factor for the next reading
  for (int i = 0; i < mGainBits; i++)
  {
    digitalWrite(mClockPin, HIGH);
    delayMicroseconds(1);
    digitalWrite(mClockPin, LOW);
  }

  if (data & 0x800000)
  {
    data |= (long)~0xffffff;
  }

  return data;
#else
  return 0;
#endif
}

int32_t HX711::readAverage(uint8_t times)
{
#ifdef __arm__
  int64_t sum = 0;
  for (uint8_t i = 0; i < times; i++)
  {
    sum += read();
  }
  return sum / times;
#else
  return 0;
#endif
}

int32_t HX711::getRawValue(uint8_t times)
{
  return readAverage(times) - mOffset;
}

float HX711::getUnits(uint8_t times)
{
  return getRawValue(times) / mScale;
}

void HX711::tare(uint8_t times)
{
  uint64_t sum = readAverage(times);
  setOffset(sum);
}

void HX711::setScale(float scale)
{
  mScale = scale;
}

void HX711::setOffset(int32_t offset)
{
  mOffset = offset;
}

void HX711::powerDown()
{
#ifdef __arm__
  digitalWrite(mClockPin, LOW);
  digitalWrite(mClockPin, HIGH);
#endif
}

void HX711::powerUp()
{
#ifdef __arm__
  digitalWrite(mClockPin, LOW);
#endif
}

int32_t HX711::getOffset()
{
  return this->mOffset;
}

float HX711::getScale()
{
  return this->mScale;
}
