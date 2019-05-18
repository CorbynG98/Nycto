/** @file   transceive.c
    @author Corbyn Greenwood
    @date   12 Oct 2018
    @brief  Deals with transmission and receiving from board to board
 *          for player position and shot detection.
*/

#include "system.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "transceive.h"

#define SETPLAYER1 171
#define LOSS 172
#define MAX 255

#define WIDTH TINYGL_WIDTH
#define HEIGHT TINYGL_HEIGHT
#define AREA (TINYGL_WIDTH * TINYGL_HEIGHT)
#define WIDTH_INDEX TINYGL_WIDTH-1
#define HEIGHT_INDEX TINYGL_HEIGHT-1

/** encodes and transmits the position **/
void transmit_pos(int position[])
{
    // Encode and transmit position
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

/** returns if received data is SETPLAYER1, receiving board becomes
 *  player 2 if they get true here **/
bool rec_player(unsigned char data) {
    return data == SETPLAYER1;
}

/** returns true if data received is LOSS, meaning receiver won the game **/
bool rec_win(unsigned char data) {
    return data == LOSS;
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
    if (input < AREA) {
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
    while(input > WIDTH_INDEX && input < MAX) {
        input = input - TINYGL_WIDTH;
        count += 1;
    }
    // Set the x and y coordinates
    enemy[0] = input;
    enemy[1] = count;
}

/** Set the laser information **/
void set_laser(int laser[], int x, int y, char dir)
{
    // Set the data to the laser array
    laser[0] = x;
    laser[1] = y;
    laser[2] = dir;
}

/** Given an array for the position and laser direction, fills in the information
 *  after decoding the information from input **/
void rec_get_laser(int laser[], unsigned char input)
{
    // from the input, decode the data so we get the position and the
    // direction of the laser.
    int enemy[2] = {0, 0};
    if (input < (AREA*2)) {
        rec_get_enemy(enemy, input - (AREA*1));
        set_laser(laser, enemy[0], enemy[1], 'N');
        //laser[0] = enemy[0];
        //laser[1] = enemy[1];
        //laser[2] = 'N';
    } else if (input < (AREA*3)) {
        rec_get_enemy(enemy, input - (AREA*2));
        set_laser(laser, enemy[0], enemy[1], 'E');
        //laser[0] = enemy[0];
        //laser[1] = enemy[1];
        //laser[2] = 'E';
    }  else if (input < (AREA*4)) {
        rec_get_enemy(enemy, input - (AREA*3));
        set_laser(laser, enemy[0], enemy[1], 'S');
        //laser[0] = enemy[0];
        //laser[1] = enemy[1];
        //laser[2] = 'S';
    }  else if (input < (AREA*5)) {
        rec_get_enemy(enemy, input - (AREA*4));
        set_laser(laser, enemy[0], enemy[1], 'W');
        //laser[0] = enemy[0];
        //laser[1] = enemy[1];
        //laser[2] = 'W';
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
        z = MAX;

    if (z != MAX)
        return (x+(WIDTH*y)) + (AREA*(z+1));
    return MAX;
}

/** encodes the character position into a single character **/
unsigned char encode_position(int position[])
{
    // Encode the position so we can transmit
    int x = position[0];
    int y = position[1];
    return (x+(WIDTH*y));
}
