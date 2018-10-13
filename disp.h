#ifndef DISP_H
#define DISP_H

#include <stdbool.h>
#include <stdint.h>

/** debug function*/
char getDebugChar(void);

/** temporary laser display details */
typedef struct laser_s {
    int counter;
    int x;
    int y;
    char direction;// 'N', 'W', 'S', 'E'
} Laser;

/** temporary player display details */
typedef struct player_s {
    int counter;//time to display
    int x;
    int y;
} Player;

/** sets screen off */
void disp_init(void);

/** display self for a time */
void disp_add_self(int position[2]);

/** display the enemy for a time */
void disp_add_enemy(int position[2]);

/** creates self laser */
void disp_add_self_laser(int laser[3]);

/** creates enemy laser */
void disp_add_enemy_laser(int laser[3]);

/** Displays a laser for a frame*/
void disp_laser(Laser laser);

/** given a starting position, find laser coordinates */
void set_laser_coords(int* startx, int* starty, int* endx, int* endy, char direction);

/** swaps two integers */
void swap_int(int* x, int* y);

/** check if we hit a thing */
bool laser_hit_self(int laser[3], int position[2]);

/** will display text and flash light */
void disp_game_win(void);

/** Display the bitmap **/
void disp_bitmap(const uint8_t level[]);

/** will display text */
void disp_game_lose(void);

/** displays a character using tinygl */
void disp_character (char character);

/** clears a character using tinygl */
void disp_clear_character(void);

/** deals with displaying current instances */
void disp_update(const uint8_t level[]);

#endif
