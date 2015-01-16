#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <stdint.h>
#include "shift.h"
#include "usbdrv/usbdrv.h"


USB_PUBLIC uint8_t usbFunctionSetup(uint8_t data[8])
{
  usbRequest_t *rq = (void *)data;
  switch(rq->bRequest)
  {
    //read 16 bit value
    case 0:
    {
      parallel_in();
      uint16_t x = serial_out();
      return 0;
    }
  }
  //should not get here
  return 0;
}

#define abs(x) ((x) > 0 ? (x) : (-x))

// Called by V-USB after device reset
void hadUsbReset() {
    int frameLength, targetLength = (unsigned)(1499 * (double)F_CPU / 10.5e6 + 0.5);
    int bestDeviation = 9999;
    uchar trialCal, bestCal, step, region;

    // do a binary search in regions 0-127 and 128-255 to get optimum OSCCAL
    for(region = 0; region <= 1; region++) {
        frameLength = 0;
        trialCal = (region == 0) ? 0 : 128;
        
        for(step = 64; step > 0; step >>= 1) { 
            if(frameLength < targetLength) // true for initial iteration
                trialCal += step; // frequency too low
            else
                trialCal -= step; // frequency too high
                
            OSCCAL = trialCal;
            frameLength = usbMeasureFrameLength();
            
            if(abs(frameLength-targetLength) < bestDeviation) {
                bestCal = trialCal; // new optimum found
                bestDeviation = abs(frameLength -targetLength);
            }
        }
    }

    OSCCAL = bestCal;
}

int main()
{
  uint8_t i;
  setup_shift();
  wdt_enable(6);
  usbInit();
  usbDeviceDisconnect();
  for(i = 0; i < 250; i++)
  {
    wdt_reset();
    _delay_ms(2);
  }
  usbDeviceConnect();
  sei();
  while(1)
  {
    wdt_reset();
    usbPoll();
  }
  return 0;
}
