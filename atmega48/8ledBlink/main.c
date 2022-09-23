#include <avr/io.h>
#include <util/delay.h>

int main(void) {
  DDRD = 0xFF;  // data direction register for portD, alle 8 pins sat til output
  PORTD = 0x00; // 00000000
  while (1)     // forever
  {
    _delay_ms(100);
    if (PORTD) {
      PORTD = 0x00; // 0
    } else {
      PORTD = 0xFF; // 255
    }
  }
}
