#include <avr/io.h>
#include <util/delay.h>

int seg_code[] = {0b11000000, 0b11111001, 0b10100100, 0b10110000, 0b10011001,
                  0b10010010, 0b10000010, 0b11111000, 0b10000000, 0b10010000};

int main(void) {
  // int num = 0;
  // int counter = 0;
  // int steps = 20;
  // int dig = 0;
  DDRD = 0xFF;        // Out, LED
  DDRB = 0xFF;        // Out, Segments
  DDRC = 0b00111100;  // In: PC0, PC1. Out: PC2, PC3, PC4, PC5.
  PORTC = 0b00111100; // Tænder PC2, PC3, PC4, PC5, som styrer hvilket display
                      // segment der er tændt
  while (1) {
    PORTD = 0x01; // 00000001
    int i;
    for (i = 0; i < 10; i++) {
      _delay_ms(1000);
      PORTB = seg_code[i];
    }
    _delay_ms(2000);
    PORTB = 0;
  }
}
