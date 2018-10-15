/*
 * File:   menu.h
 * Author: Corbyn Greenwood
 * Date:   12 Oct 2018
 * Descr:  Main menu header file so the other modules have access
 *         to these functions
*/

#ifndef MENU_H
#define MENU_H

/** Main loop for the main menu, waits for inputs
 *  and deals with them **/
void main_menu(char* level, bool* isPlayer1, int position[], char* direction, uint8_t bitmap[]);

/** Builds the currently selected level **/
void build_level(char map, uint8_t bitmap, int isChosen);

/** Places the player in the start position **/
void set_player_pos(int position[], char* direction, bool isPlayer1, char level);

/** Puts x and y coordinates into the position array **/
void set_pos(int position[], int x, int y);

/** Puts values into the bitmap**/
void set_bitmap(uint8_t bitmap[], uint8_t val1, uint8_t val2, uint8_t val3, uint8_t val4, uint8_t val5);

#endif
