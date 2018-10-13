#ifndef MENU_H
#define MENU_H

/** Main loop for the main menu, waits for inputs
 *  and deals with them **/
void main_loop(int num_levels, char* level, bool* isPlayer1);

/** Builds the currently selected level **/
void build_level(char map);

#endif
