#include <avr/io.h>
#include <util/delay.h>

#define MIN(a, b) (a) > (b) ? b : a
#define MAX(a, b) (a) < (b) ? b : a

int main(void) {
  int p = 4;
  int b1 = PC0;
  int b2 = PC1;
  DDRC = 0x00;  // Input of two buttons
  DDRD = 0xFF;  // data direction register for portD, alle 8 pins sat til output
  PORTD = 0x00; // Random mess
  while (1)     // forever
  {
    _delay_ms(100);
    if (bit_is_set(PINC, b1)) {
      p = MIN(p + 1, 7);
    }
    if (bit_is_set(PINC, b2)) {
      p = MAX(p - 1, 0);
    }
    // Flip
    if (PORTD >> (p)&1) {
      // Sluk
      PORTD &= (~(0x01 << p)); // Maske: 11101111
    } else {
      // Tænd
      PORTD |= (0x01 << p); // Or: 00010000
    }
  }
}
