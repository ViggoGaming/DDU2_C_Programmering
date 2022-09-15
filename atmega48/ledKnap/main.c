#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	int dir = 1;
	int on = 1;
	int inp = 0;
	int pinp = 0;
	DDRD = 0xFF; //data direction register for portD, alle 8 pins sat til output
	PORTD = 0x00; //alle bits = 0
	DDRC = 0x00; //data direction register for portC, alle 8 pins sat til input

	PORTD = 0x01;
	while (1) //forever  
	{
		_delay_ms(40);
		inp = bit_is_set(PINC, PC0);
		if (inp && !pinp) {
			on = !on;
		}
		if (on) {
			if (dir) {
				PORTD = PORTD << 1; //bitshift; skubber hele rækken til venstre
			}
			else {
				PORTD = PORTD >> 1; //bitshift; skubber hele rækken til venstre
			}
			if (PORTD == 0x01 || PORTD == 0x80) {
				dir = !dir;
			}
		}
		pinp = inp;
	}
	return 0;
}