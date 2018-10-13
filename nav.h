#ifndef NAV_H
#define NAV_H

/** Initialise and check what button was pressed. */
bool nav_getminput(char* prev_dir);

/** Attempt to move the player in the direction they chose */
bool nav_move(int position[], char* prev_dir);

/** Get the horizontal input of the nav switch, for the menu **/
char nav_getmhorizontal(void);

/** Return true if the player tried to shoot. */
bool nav_shoot(void);

/** Check if the player is trying to move into a wall. */
bool nav_hitwall(int position[], char move_dir);

/** Convert the uint8_t bitmap value to a binary string **/
void ul32toBinary(uint8_t a, char binary[]);

/** Checks if the player hits an inner wall. **/
bool nav_hitwallinner(int position[], char move_dir, const uint8_t bitmap[]);

#endif
