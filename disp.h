#ifndef DISP_H
#define DISP_H

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
void disp_add_self(int[2] position);

/** display the enemy for a time */
void disp_add_enemy(int[2] position);

/** creates new laser */
void disp_add_laser(int[3] laser);

/** Displays a laser for a frame*/
void disp_laser(Laser laser);

/** deals with displaying current instances */
void disp_update(void);

#endif
