#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "ir_uart.h"

int main(void) {
    system_init ();
    navswitch_init ();
    ir_uart_init();
}

void transmit(int[] position) {
    // Transmit position
    char transmit_char = encode_position(position)
    ir_uart_putc(transmit_char);
}

void receive() {
    // Receive position
    char received = ir_uart_getc();
    int[] position = decode_received(received);
}

void transmit_laser(int[] position, char direction) {
    // Transmit shot
}

void receive_shot(int[] positon, char direction) {
    // Receive laser
}

char encode_position(int[] position) {
    // Encode the position so we can transmit
    int x = position[0];
    int y = position[1];
    return (x+(7*y))
}

int[] decode_received(char input) {
    // Decode the character received
    if (input < 35) {
        // Do stuff
        int count = 0;
        while(input > 6 && input > -1) {
            input = input - 7;
            count += 1;
        }
        return [input, count];
    }
}

char encode_pos_laser(int[] position, char direction) {
    // Encode the position so we can transmit
}
