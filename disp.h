/** @file   disp.h
    @author Benjamin Scott
    @date   16 October 2018
    @brief  Display manager
*/

#ifndef DISP_H
#define DISP_H

#include <stdbool.h>
#include <stdint.h>

//customization constants
#define PACER_RATE 350
#define DISP_TIME 60 //time to display a player for
#define DISP_LASER_TIME 40 // time to display a laser for
#define DISP_WIN_FLASH_TIME 80 // time to flash blue for
#define DISP_NPC_WAIT_MULT 10 // multiple of DISP_LASER_TIME to wait for between shots
#define MESSAGE_RATE 13

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

/** initializes tinygl and blue LED */
void disp_init(void);

/** displays a character using tinygl */
void disp_character (char character);

/** clears a character using tinygl */
void disp_clear_character(void);

/** sets given player to a position and displays them temporarily */
void disp_add_player(Player* player, int position[2]);

/** display self for a time */
void disp_add_self(int position[2]);

/** display the enemy for a time */
void disp_add_enemy(int position[2]);

/** sets given laserPtr to laser*/
void disp_add_laser(Laser* laserPtr, int laser[3]);

/** creates self laser */
void disp_add_self_laser(int laser[3]);

/** creates enemy laser */
void disp_add_enemy_laser(int laser[3]);

/** creates npc laser */
void disp_add_npc_laser(int laser[3]);

/** refresh timer on npc laser */
void disp_refresh_npc_laser(void);

/** Displays a laser for a frame*/
void disp_laser(Laser laser, const uint8_t level[], bool isNPC);

/** will display text and flash light */
void disp_game_win(void);

/** displays a bitmap, one pixel at a time, because tinygl doesn't
 *  allow us to add columns to their private matrix :( */
void disp_bitmap(const uint8_t level[]);

/** will display text */
void disp_game_lose(void);

/** returns true if the npc laser has been initialised*/
bool have_npc_laser(void);

/** return true if position intersects with the npc laser */
bool hit_npc_laser(int position[2], const uint8_t level[]);

/** deals with displaying current instances */
void disp_update(const uint8_t level[]);

#endif
