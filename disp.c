/** @file   disp.c
    @author Benjamin Scott
    @date   16 October 2018
    @brief  Display manager
*/

#include "disp.h"
#include "tinygl.h"
#include "pacer.h"
#include "../fonts/font5x7_1.h"
#include "pio.h"
#include "lasers.h"

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
static bool textSet = false; /**game end text should only be set once
                                when scrolling, otherwise it gets stuck */
static int winFlashCounter = DISP_WIN_FLASH_TIME;//length of blue light flash at end
static char* GAMEWON = " WIN";
static char* GAMELOST = " LOSE";

/** initializes tinygl and blue LED */
void disp_init(void)
{
    tinygl_init (PACER_RATE);
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    pio_config_set (LED_PIO, PIO_OUTPUT_LOW);
    pacer_init (PACER_RATE);// sets frame rate
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

/** sets the given laserPtr's Laser to the given laser*/
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

/** OBSOLETE: Returns whether an LED should be turned on to establish a
 * fading effect as counter ramps down from max. Barely visible at best
 * and causes flickering at worst*/
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
    pacer_wait();
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
        if (!textSet) {
            tinygl_text (GAMEWON);
            textSet = true;
        }
    } else if (dispGameLost) {
        if (!textSet) {
            tinygl_text (GAMELOST);
            textSet = true;
        }
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
