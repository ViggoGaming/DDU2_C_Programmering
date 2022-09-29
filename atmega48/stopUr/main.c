#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

// Hvor ofte displayet skal opdatere i ms. 
#define timestep 5
// Milisekunder mellem tallet inkrementeres
#define countinterval 100

// Binære koder for cifre          0           1           2          3            4           5           6          7           8            9
volatile uint8_t seg_code[] = {0b11000000, 0b11111001, 0b10100100, 0b10110000, 0b10011001, 0b10010010, 0b10000010, 0b11111000, 0b10000000, 0b10010000};

// Indeholder de 4 cifre skærmen viser
uint8_t digits[4];

// Deklarerer funktionen som opdaterer 'digits'
void set_num(uint32_t num);

int main()
{
    // Sætter alle B-porte til output, som styrer display segmenterne A-G
    DDRB = 0xFF;

    // Sætter PC0 og PC1 til at få input fra knapperne. Sætter resten til at give output til ciffer anoderne
    DDRC = 0b00111100; // Input: 0-1, Buttons. Output: 2-5, Digit anodes

    // Index for hvilket ciffer vi viser 
    uint8_t dind = 0;

    // Timer der tæller hvor mange millisekunder timeren har været tændt
    uint32_t millis = 0;

    // Tidligere værdi af pause-knappen. Nødvendig for at knappen fungerer
    bool pp = false;

    // Boolean til at holde styr på om stopuret er pauset eller ej 
    bool paused = false;

    while (1)
    {
        _delay_ms(timestep);
        
        // Input 
        // Hvis pauseknappen er trykket, og den ikke var trykket før
        // Dette er den første knap, der sider til PC0
        if ((PINC & (1 << 0)) && !pp)
        {
            paused = !paused;
        }
        
        // Hvis 'Reset' knappen er trykket
        // Dette er den anden knap, der sider til PC1
        if ((PINC & (1 << 1)))
        {
            // Sæt displayet til 0
            millis = 0;
            set_num(millis);
        }

        // Hvis uret er tændt (ikke pauset)
        if (!paused)
        {
            // Inkrementer timeren millis med vores timestep
            millis += timestep;
            
            // Hvis vi er nået til et punkt hvor tallet skal opdaters
            if (millis % countinterval < timestep) {
                // Opdater tallet der vises
                set_num((millis / countinterval));
            }
        }

        // Vis
        dind = (dind + 1) % 4;
        PORTC = 1 << (2 + dind);        // Tænd anoden til det korresponderende ciffer
        PORTB = seg_code[digits[dind]]; // Vis cifret
        pp = PINC&(1 << 0);             // Opdater previous pause, der fortæller om trykknappen var nede "lige før"
    }
}

// Udregner hvilke tal der skal vises på display
// Heltallet bliver konverteret til enkelte tal

//  F.eks. 1337 bliver til [7, 3, 3, 1] Det bliver vendt om igen når det vises.
void set_num(uint32_t num)
{
    uint8_t i;
    for (i = 0; i < 4; i++)
    {
        // Set digits[i] til det sidste ciffer i tallet
        digits[i] = num % 10;
        // Division med 10 fjerner det sidste ciffer fra tallet
        num /= 10;
    }
}