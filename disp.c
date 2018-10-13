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
#define MESSAGE_RATE 10

//AVR constants
#define OUTPUT_HIGH 1
#define LED_PIO PIO_DEFINE (PORT_C, 2)

//static objects
static Laser selfLaser = {0, 0, 0, 'N'};
static Laser enemyLaser = {0, 0, 0, 'N'};
static Player self = {0, 0, 0};
static Player enemy = {0, 0, 0};

//static game variables
static bool gameWon = false;
static bool gameLost = false;
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

/** display self for a time */
void disp_add_self(int position[2])
{
    self.counter = DISP_TIME;
    self.x = position[0];
    self.y = position[1];
}

/** display the enemy for a time */
void disp_add_enemy(int position[2])
{
    enemy.counter = DISP_TIME;
    enemy.x = position[0];
    enemy.y = position[1];
}

/** creates self laser */
void disp_add_self_laser(int laser[3])
{
    //generalize these
    selfLaser.counter = DISP_LASER_TIME;
    selfLaser.x = laser[0];
    selfLaser.y = laser[1];
    selfLaser.direction = laser[2];
    //also add self player
    int pos[2] = {laser[0], laser[1]};
    disp_add_self(pos);
}

/** creates enemy laser */
void disp_add_enemy_laser(int laser[3])
{
    enemyLaser.counter = DISP_LASER_TIME;
    enemyLaser.x = laser[0];
    enemyLaser.y = laser[1];
    enemyLaser.direction = laser[2];
    //also add enemy player
    int pos[2] = {laser[0], laser[1]};
    disp_add_enemy(pos);
}

/** Displays a laser for a frame*/
void disp_laser(Laser laser)
{
    int endx = laser.x;
    int endy = laser.y;
    int startx = laser.x;
    int starty = laser.y;
    switch (laser.direction) {
        case 'N':
            starty--;
            endy = 0;
            break;
        case 'E':
            startx++;
            endx = TINYGL_WIDTH;
            break;
        case 'S':
            starty++;
            endy = TINYGL_HEIGHT;
            break;
        case 'W':
            startx--;
            endx = 0;
            break;
    }
    tinygl_draw_line (tinygl_point (startx, starty),
                      tinygl_point (endx, endy),
                      OUTPUT_HIGH);
}

/** Returns whether an LED should be turned on to establish a fading
 * effect as counter ramps down from max. Causes lag and is basically
 * useless */
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
    gameWon = true;
}

/** will display text */
void disp_game_lost(void) {
    gameLost = true;
}

/** displays a character using tinygl */
void disp_character (char character)
{
    char buffer[2];
    buffer[0] = character;
    buffer[1] = '\0';
    tinygl_text (buffer);
}

/** deals with displaying current instances */
void disp_update(void)
{
    if (gameWon) {
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
    } else if (gameLost) {
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
            disp_laser(selfLaser);
            selfLaser.counter--;
        }
        if (enemyLaser.counter > 0) {
            disp_laser(enemyLaser);
            enemyLaser.counter--;
        }
    }
    //refresh frame
    tinygl_update ();
}
