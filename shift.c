#include "shift.h"
#include <avr/io.h>
#include <util/delay.h>
#define PL 1
#define Q7 2
#define CP 0
#define DELAY 10
#define SET_IN(BIT) DDRB &= ~(1<<BIT)
#define SET_OUT(BIT) DDRB |= (1<<BIT)
#define SETB(BIT) PORTB |= (1<<BIT)
#define CLEARB(BIT) PORTB &= ~(1<<BIT)

void setup_shift()
{
  SET_IN(Q7);
  SET_OUT(PL);
  SET_OUT(CP);
  SETB(PL);
  CLEARB(CP);
  //set pull up resistor maybe necessary?
  //SETB(Q7);
}

void parallel_in()
{
  CLEARB(PL);
  _delay_ms(DELAY);
  SETB(PL);
}

uint16_t serial_out()
{
  uint16_t i,bit,word;
  word = 0;
  for (i = 0; i < 16; i++)
  {
    CLEARB(CP);
    _delay_ms(DELAY);
    SETB(CP);
    _delay_ms(DELAY);
    bit = PINB & (1<<Q7);
    word |= (bit << i);
  }
  return word;
}
