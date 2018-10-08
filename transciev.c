#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "ir_uart.h"

int main(void) {
    system_init ();
    navswitch_init ();
    ir_uart_init();
}

void transmit(position) {
    // Transmit position
}

void receive(position) {
    // Receive position
}

void transmit_laser(position, direction) {
    // Transmit shot
}

void receive_shot(positon, direction) {
    // Receive laser
}

char encode_position(int[] position) {
    // Encode the position so we can transmit
}

int[] decode_position(char input) {
    // Decode the character received
}
