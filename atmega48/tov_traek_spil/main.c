#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	int a = 0; // player a input
	int b = 0; // player b input
	int pa = a; // previous a
	int pb = b; // previous b
	DDRD = 0xFF; //data direction register for portD, alle 8 pins sat til output
	PORTD = 0x00; //alle bits = 0
	DDRC = 0x00; //data direction register for portC, alle 8 pins sat til input

	PORTD = 0b00001000; // start position in center
	while (1) //forever  
	{
		_delay_ms(1);
		a = bit_is_set(PINC, PC0);
		b = bit_is_set(PINC, PC1);
		if (a && !pa) {
			PORTD = PORTD << 1;
		}
		if (b && !pb) {
			PORTD = PORTD >> 1;
		}
		if (PORTD == 0b01000000 || PORTD == 0b00000001) {
			_delay_ms(1000);
			while (!(bit_is_set(PINC, PC0)&&bit_is_set(PINC, PC1))) {
				_delay_ms(10);
			}
			PORTD = 0b00001000;
			a = 0;
			b = 0;
		}
		pa = a;
		pb = b;
	}

	
}