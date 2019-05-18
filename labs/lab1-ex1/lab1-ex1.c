#include <avr/io.h>
#include "system.h"

int main (void)
{
    system_init ();

    /* Initialise port to drive LED 1.  */
    DDRC |= (1<<2);
    DDRD &= ~(1<<7);
    /* TODO.  */

    while (1)
    {
        /* Turn LED 1 on.  */
        /* TODO.  */
        if (PIND & (1<<7)) {
            PORTC |= (1<<2);
        } else {
            PORTC &= ~(1<<2);
        }
    }
}
