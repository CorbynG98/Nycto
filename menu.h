#ifndef MENU_H
#define MENU_H

/** Main loop for the main menu, waits for inputs
 *  and deals with them **/
void main_menu(char* level, bool* isPlayer1, int position[], char* direction, uint8_t bitmap[]);

/** Builds the currently selected level **/
void build_level(char map, uint8_t bitmap);

/** Places the player in the start position **/
void set_player_pos(int position[], char* direction, bool isPlayer1, char level);

#endif
