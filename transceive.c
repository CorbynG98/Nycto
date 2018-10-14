/*
 * File:   transceive.c
 * Author: Corbyn Greenwood
 * Date:   12 Oct 2018
 * Descr:  Deals with transmission and receiving from board to board
 *         for player position and shot detection.
*/

#include "system.h"
#include "ir_uart.h"
#include "transceive.h"

#define SETPLAYER1 171
#define LOSS 172

/** encodes and transmits the position **/
void transmit_pos(int position[])
{
    // Transmit position
    unsigned char transmit_char = encode_position(position);
    ir_uart_putc(transmit_char);
}

/** transmits that one of the boards has become player 1 **/
void transmit_player1(void) {
    ir_uart_putc(SETPLAYER1);
}

/** transmits the selected map so both boards have the same level **/
void transmit_map(char currentMap) {
    ir_uart_putc(currentMap);
}

/** transmits to the other board that they lost the game. **/
void transmit_loss(void) {
    ir_uart_putc(LOSS);
}

/** returns if received data is 171, receving board becomes
 *  player 2 if they get true here **/
bool rec_player(unsigned char data) {
    return data == 171;
}

/** returns true if data received is 172, meaning they won the game **/
bool rec_win(unsigned char data) {
    return data == 172;
}

/** receives any data from the ir device  **/
unsigned char rec_get_data(void)
{
    // Receive position
    unsigned char received = ir_uart_getc();
    return received;
}

/** Encodes the position and laser and then sends the single character **/
void transmit_laser(int position[], char direction) {
    // Transmit shot
    // Still need todo this will only be like 3 lines :D
    unsigned char transmit_las_pos = encode_pos_laser(position, direction);
    ir_uart_putc(transmit_las_pos);
}

/** checks if the received data is just an enemy position or if it is also a
 *  laser direction **/
int rec_is_enemy(unsigned char input)
{
    // Check if the input is only an enemy position without laser
    if (input < 35) {
        return 1;
    }
    return 0;
}

/** returns true if the ir device is ready to receive data **/
int rec_got_data(void) {
    // Check that the ir device is ready to receive data.
    return ir_uart_read_ready_p();
}

/** given an array of positions, decodes the received data and sets
 *  the x and y, [0] and [1] of the position from the inpu **/
void rec_get_enemy(int enemy[], unsigned char input)
{
    // decode the position of the character.
    int count = 0;
    while(input > 4 && input < 255) {
        input = input - 5;
        count += 1;
    }
    enemy[0] = input;
    enemy[1] = count;
}

/** Given an array for the position and laser direction, fills in the information
 *  after decoding the information from input **/
void rec_get_laser(int laser[], unsigned char input)
{
    // from the input, decode the data so we get the position and the
    // direction of the laser.
    int enemy[2] = {0, 0};
    if (input < 70) {
        rec_get_enemy(enemy, input - (35*1));
        laser[0] = enemy[0];
        laser[1] = enemy[1];
        laser[2] = 'N';
    } else if (input < 105) {
        rec_get_enemy(enemy, input - (35*2));
        laser[0] = enemy[0];
        laser[1] = enemy[1];
        laser[2] = 'E';
    }  else if (input < 140) {
        rec_get_enemy(enemy, input - (35*3));
        laser[0] = enemy[0];
        laser[1] = enemy[1];
        laser[2] = 'S';
    }  else if (input < 175) {
        rec_get_enemy(enemy, input - (35*4));
        laser[0] = enemy[0];
        laser[1] = enemy[1];
        laser[2] = 'W';
    }

}

/** encodes the laser and character position into a single character **/
unsigned char encode_pos_laser(int position[], char direction)
{
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
        z = 255;

    if (z != 255)
        return (x+(5*y)) + (35*(z+1));
    return 255;
}

/** encodes the character position into a single character **/
unsigned char encode_position(int position[])
{
    // Encode the position so we can transmit
    int x = position[0];
    int y = position[1];
    return (x+(5*y));
}
