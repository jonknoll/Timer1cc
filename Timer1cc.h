/***************************************************************************//**
MIT License

Copyright (c) 2017 Jon Knoll

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

@DESCRIPTION
Timer 1 capture/compare library. This library is specifically for using the
capture & compare feature of Timer1.
   
Current support:
- ATmega328: Timer1 - mode 4 (CTC)
- Adjustable prescaler
- Adjustable rollover value with interrupt
- Timer capture interrupt

Additional modes and compare output setting could be added to the library.

Usage: This library has a built-in instance of the class. It is not intended to
be instantiated. Use the built-in instance: Timer1.

This initializes Timer1 to rollover and interrupt once per second:
Timer1.initialize(TIMER_DIV256_62500HZ, 62500);
Timer1.attachRolloverInterrupt(myCallback);
Timer1.start();

@VERSION 1.1.0
*/

// Usage:
#ifndef TIMER1CC_H
#define TIMER1CC_H

#include <Arduino.h>

typedef enum
{
   TIMER_NO_FREQ = 0,
   TIMER_DIV1_16MHZ = 1,
   TIMER_DIV8_2MHZ = 2,
   TIMER_DIV64_250KHZ = 3,
   TIMER_DIV256_62500HZ = 4,
   TIMER_DIV1024_15625 = 5,
   TIMER_EXT_FALLING = 6,
   TIMER_EXT_RISING = 7
} timerSourceEnum;


typedef void (*captureIsr_t)(uint16_t timerVal);
typedef void (*rolloverIsr_t)(void);


class Timer1Cap
{
public:
   void initialize(timerSourceEnum prescaler, uint16_t rolloverCount=0xFFFF);
   
   void attachCaptureInterrupt(captureIsr_t captureCalback, bool edge, bool enableNoiseCanceller);
   void detachCaptureInterrupt(void);

   void attachRolloverInterrupt(rolloverIsr_t rolloverCallback);
   void detachRolloverInterrupt(void);
   
   void start(void);
   void stop(void);
   uint16_t getTimer(void);

   // Interrupt handlers - Not intended to be called externally
   captureIsr_t captureIsr;
   rolloverIsr_t rolloverIsr;
   
private:
   uint8_t tccr1bReg;
};

extern Timer1Cap Timer1;

#endif // TIMER1CC_H
