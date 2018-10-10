#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "ir_uart.h"

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
    if (input == -1)
        return [-1,-1];
    else if (input < 35) {
        // Do stuff
        int count = 0;
        while(input > 6 && input > -1) {
            input = input - 7;
            count += 1;
        }
        return [input, count];
    } else if (input < 70) {
        char direction = 'N';
        int data[3] = decode_received(input - 35) + direction;
        return data;
    } else if (input < 105) {
        char direction = 'E';
        int data[3] = decode_received(input - (35*2)) + direction;
    }  else if (input < 105) {
        char direction = 'S';
        int data[3] = decode_received(input - (35*3)) + direction;
    }  else if (input < 105) {
        char direction = 'W';
        int data[3] = decode_received(input - (35*4)) + direction;
    }
}

char encode_pos_laser(int[] position, char direction) {
    // Encode the position so we can transmit
    int x = position[0];
    int y = position[1];
    int z = 0;
    if (direction == 'N')
        z = 0;
    else if (direction == 'E')
        z = 1;
    else if (direction == 'S')
        z = 2;
    else if (direction == 'W')
        z = 3;
    else
        z = -1

    if (z != -1)
        return (x+(7*y)) + (35*(z+1))
    else
        return -1;
}

/* Do we need to differentiate between a shot transmit /receive and a sole
 * transmission transmit / revceive?
 *
 * I think we can just transmit al lthe data all the time, take the first 6
 * bits as position and the last 2 bits as direction then the function calling
 * it can determine if its a shot or just a wall hit and only use the data
 * it needs to for whatever it detemrines happened. Just a thought. */
