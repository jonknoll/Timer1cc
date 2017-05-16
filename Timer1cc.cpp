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


This is the implementation of the Timer1 capture/compare library. See header
file for details.
*/

#include <Arduino.h>
#include "Timer1cc.h"

Timer1Cap Timer1;              // make an instance for people to use


ISR(TIMER1_CAPT_vect)
{
  Timer1.captureIsr(ICR1);
}

ISR(TIMER1_COMPA_vect)
{
   Timer1.rolloverIsr();
}


void Timer1Cap::initialize(timerSourceEnum prescaler, uint16_t rolloverCount)
{
   TCCR1A = 0;
   TCCR1B = 0;
   TCNT1 = 0;
   OCR1A = rolloverCount;
   captureIsr = 0;
   rolloverIsr = 0;
   tccr1bReg = _BV(WGM12); // CTC mode
   tccr1bReg |= prescaler;
}

// 0=capture on falling edge (default), 1=capture on rising edge
// 0=no noise canceller, 1=enable noise canceller (adds 4 sys clk delay)
void Timer1Cap::attachCaptureInterrupt(captureIsr_t captureCalback, bool edge, bool enableNoiseCanceller)
{
   captureIsr = captureCalback;
   tccr1bReg = (tccr1bReg & ~_BV(ICNC1)) | (edge << ICNC1);
   tccr1bReg = (tccr1bReg & ~_BV(ICNC1)) | (enableNoiseCanceller << ICNC1);
   
   TIFR1 |= _BV(ICF1);  // clear flag before enabling interrupt
   TIMSK1 |= _BV(ICIE1);
}

void Timer1Cap::detachCaptureInterrupt(void)
{
   TIMSK1 &= ~_BV(ICIE1);
}

void Timer1Cap::attachRolloverInterrupt(rolloverIsr_t rolloverCallback)
{
   rolloverIsr = rolloverCallback;
   TIFR1 |= _BV(OCF1A);     // clear flag before enabling interrupt
   TIMSK1 |= _BV(OCIE1A);
}

void Timer1Cap::detachRolloverInterrupt(void)
{
   TIMSK1 &= ~_BV(OCIE1A);
}

void Timer1Cap::start(void)
{
   TCCR1B = tccr1bReg;
}

void Timer1Cap::stop(void)
{
   TCCR1B = 0;
}

uint16_t Timer1Cap::getTimer(void)
{
   return(TCNT1);
}
