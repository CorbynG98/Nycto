#include "disp.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"
#include "pio.h"
#include <stdbool.h>

//customization constants
#define DISP_RATE 250 //should be the same as PACER_RATE
#define DISP_TIME 60 //time to display a player for
#define DISP_LASER_TIME 40 // time to display a laser for
#define DISP_WIN_FLASH_TIME 80 // time to flash blue for
#define DISP_NPC_WAIT_MULT 10 // multiple of DISP_LASER_TIME to wait for between shots
#define MESSAGE_RATE 10

//AVR constants
#define OUTPUT_HIGH 1
#define LED_PIO PIO_DEFINE (PORT_C, 2)

//static objects
static Laser selfLaser = {0, 0, 0, 'N'};
static Laser enemyLaser = {0, 0, 0, 'N'};
static Laser npcLaser = {DISP_LASER_TIME+1, 0, 0, 'N'};
static Player self = {0, 0, 0};
static Player enemy = {0, 0, 0};

//static game variables
static bool dispGameWon = false;
static bool dispGameLost = false;
static int winFlashCounter = DISP_WIN_FLASH_TIME;//length of blue light flash at end
static char* GAMEWON = "WIN";
static char* GAMELOST = "LOSE";

static char dispDebugChar = 'D';

char getDebugChar(void) {
    return dispDebugChar;
}

/** sets screen off */
void disp_init(void)
{
    tinygl_init (DISP_RATE);
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    //tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    pio_config_set (LED_PIO, PIO_OUTPUT_LOW);
}

/** sets given player to a position and displays them temporarily */
void disp_add_player(Player* player, int position[2])
{
    player->counter = DISP_TIME;
    player->x = position[0];
    player->y = position[1];
}

/** display self for a time */
void disp_add_self(int position[2])
{
    disp_add_player(&self, position);
}

/** display the enemy for a time */
void disp_add_enemy(int position[2])
{
    disp_add_player(&enemy, position);
}

/** sets given laserPtr to laser*/
void disp_add_laser(Laser* laserPtr, int laser[3])
{
    laserPtr->counter = DISP_LASER_TIME;
    laserPtr->x = laser[0];
    laserPtr->y = laser[1];
    laserPtr->direction = laser[2];
}

/** creates self laser */
void disp_add_self_laser(int laser[3])
{
    disp_add_laser(&selfLaser, laser);
    //also add self player
    int pos[2] = {laser[0], laser[1]};
    disp_add_self(pos);
}

/** creates enemy laser */
void disp_add_enemy_laser(int laser[3])
{
    disp_add_laser(&enemyLaser, laser);
    //also add enemy player
    int pos[2] = {laser[0], laser[1]};
    disp_add_enemy(pos);
}

/** creates npc laser */
void disp_add_npc_laser(int laser[3])
{
    disp_add_laser(&npcLaser, laser);
}

/** refresh timer on npc laser */
void disp_refresh_npc_laser(void)
{
    npcLaser.counter = DISP_LASER_TIME;
}

/** Displays a laser for a frame*/
void disp_laser(Laser laser, const uint8_t level[], bool isNPC)
{
    int endx = laser.x;
    int endy = laser.y;
    int startx = laser.x;
    int starty = laser.y;
    set_laser_coords(&startx, &starty, &endx, &endy, laser.direction, level, isNPC);
    tinygl_draw_line (tinygl_point (startx, starty),
                      tinygl_point (endx, endy),
                      OUTPUT_HIGH);
}

/** given a starting position, find laser coordinates */
void set_laser_coords(int* startx, int* starty, int* endx, int* endy, char direction, const uint8_t level[], bool isNPC)
{
    //set default position if hit no walls, moving one position past the player
    int increment = 1;
    if (isNPC)
        increment = 0;
    switch (direction) {
        case 'N':
            (*starty)-=increment;
            *endy = 0;
            break;
        case 'E':
            (*startx)+=increment;
            *endx = TINYGL_WIDTH;
            break;
        case 'S':
            (*starty)+=increment;
            *endy = TINYGL_HEIGHT;
            break;
        case 'W':
            (*startx)-=increment;
            *endx = 0;
            break;
    }

    //check if laser hits a wall
    bool hitWall = false;
    if (*startx < *endx || *starty < *endy) {
        //if pointing down or right
        int i=*startx;
        while (i <= *endx && !hitWall) {
            int j=*starty;
            while (j <= *endy && !hitWall) {
                if (level[i] & (1 << j)) {
                    //if level has a wall at i,j
                    *endx = i;
                    *endy = j;
                    hitWall = true;
                }
                j++;
            }
            i++;
        }
    } else {
        int i=*startx;
        while (i >= *endx && !hitWall) {
            int j=*starty;
            while (j >= *endy && !hitWall) {
                if (level[i] & (1 << j)) {
                    //if level has a wall at i,j
                    *endx = i;
                    *endy = j;
                    hitWall = true;
                }
                j--;
            }
            i--;
        }
    }



}

/** swaps two integers */
void swap_int(int* x, int* y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

/** check if we the laser intersects with position, taking into account
 * the level */
bool laser_hit_self(int laser[3], int position[2], const uint8_t level[], bool isNPC)
{
    int endx = laser[0];
    int endy = laser[1];
    int startx = laser[0];
    int starty = laser[1];

    set_laser_coords(&startx ,&starty, &endx, &endy, laser[2], level, isNPC);

    //make sure start is before end
    if (startx > endx) {
        swap_int(&startx, &endx);
    }
    if (starty > endy) {
        swap_int(&starty, &endy);
    }

    //if hit by enemy laser
    for (int i=startx;i <= endx; i++) {
        for (int j=starty;j <= endy; j++) {
            if (position[0] == i && position[1] == j) {
                return true;
            }
        }
    }
    return false;
}

/** OBSOLETE: Returns whether an LED should be turned on to establish a
 * fading effect as counter ramps down from max. Causes lag and is
 * basically useless */
 /*
bool fader(int counter, int max) {
    if (counter < max/2) {
        return counter % 2;
    } else {
        return true;
    }
}
* */

/** will display text and flash light */
void disp_game_win(void) {
    dispGameWon = true;
}

/** will display text */
void disp_game_lose(void) {
    dispGameLost = true;
}

/** displays a character using tinygl */
void disp_character (char character)
{
    char buffer[2];
    buffer[0] = character;
    buffer[1] = '\0';
    tinygl_text (buffer);
}

/** clears a character using tinygl */
void disp_clear_character(void)
{
    disp_character(' ');
}

/** displays a bitmap, one pixel at a time, because tinygl doesn't
 *  allow us to add columns to their private matrix :( */
void disp_bitmap(const uint8_t level[])
{
    for (int i=0;i<TINYGL_WIDTH;i++) {
        for (int j=0;j<TINYGL_HEIGHT;j++) {
            if (level[i] & (1 << j)) {
                tinygl_pixel_set(tinygl_point (i, j), OUTPUT_HIGH);
            }
        }
    }
}

/** returns true if the npc laser has been initialised*/
bool have_npc_laser(void)
{
    return npcLaser.counter < DISP_LASER_TIME+1;
}

/** return true if position intersects with the npc laser */
bool hit_npc_laser(int position[2], const uint8_t level[])
{
    if (npcLaser.counter > 0) {
        int laser[3] = {npcLaser.x, npcLaser.y, npcLaser.direction};
        return laser_hit_self(laser, position, level, true);
    } else {
        return false;
    }
}

/** deals with displaying current instances */
void disp_update(const uint8_t level[])
{
    if (dispGameWon) {
        //blue light flash
        if (winFlashCounter == -DISP_WIN_FLASH_TIME) {
            winFlashCounter = DISP_WIN_FLASH_TIME;
        } else if (winFlashCounter > 0){
            //light on
            pio_output_high (LED_PIO);
            winFlashCounter--;
        } else {
            //light off
            pio_output_low (LED_PIO);
            winFlashCounter--;
        }
        tinygl_text (GAMEWON);
    } else if (dispGameLost) {
        tinygl_text (GAMELOST);
    } else {

        display_clear ();

        //draw players
        if (self.counter > 0) {
            tinygl_pixel_set(tinygl_point (self.x, self.y), OUTPUT_HIGH);
            self.counter--;
        }
        if (enemy.counter > 0) {
            tinygl_pixel_set(tinygl_point (enemy.x, enemy.y), OUTPUT_HIGH);
            enemy.counter--;
        }

        //draw lasers
        if (selfLaser.counter > 0) {
            disp_laser(selfLaser, level, false);
            selfLaser.counter--;
        }
        if (enemyLaser.counter > 0) {
            disp_laser(enemyLaser, level, false);
            enemyLaser.counter--;
        }
        if (have_npc_laser()) {
            //if npcLaser has been added
            if (npcLaser.counter > 0) {
                disp_laser(npcLaser, level, true);
                npcLaser.counter--;
            }
        }

        //draw level
        disp_bitmap(level);
    }
    //refresh frame
    tinygl_update ();
}
