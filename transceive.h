#ifndef TRANSCEIVE_H
#define TRANSCEIVE_H

/** Transmit the position **/
void transmit_pos(int position[]);

/** Transmits that player 1 has been selected **/
void transmit_player1(void)

/** Transmits the map the player 1 selected. **/
void transmit_map(char currentMap);

/** get the data received from the ir device **/
char rec_get_data(void);

/** transmit the data for the laser **/
void transmit_laser(int position[], char direction);

/** Check if the input is an enemy position only **/
int rec_is_enemy(unsigned char input);

/** Check the ir device is ready to receive **/
int rec_got_data(void);

/** decode the received data for the enemy position **/
void rec_get_enemy(int enemy[], unsigned char input);

/** decode the received data for the enemy position and laser direction **/
void rec_get_laser(int laser[], unsigned char input);

/** Encode the player and laser direction **/
char encode_pos_laser(int position[], char direction);

/** Encode the player position **/
char encode_position(int position[]);

#endif
