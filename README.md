# Nycto

This is an arena shooter game, having multiple arenas to choose
from. The goal is to find and shoot the other player with your
laser. The catch however, is that each of you are only visible
(to you and your opponent) when you shoot, or bump into a wall.

### Getting started

Make sure to have a University of Canterbury FunKit Microcontroller
or a controller to the same spec. This program will not work on any
board that is not the the spec mentioned above.

### Installing

Clone the repository for this somewhere to your local machine.

Now open the terminal and traverse to the assignment/game folder.
With the ucfunkit plugged into the computer, Enter the command
'make program' (no quotes).
Reset the board if there are problems.

### How to play

- Navswitch directions to control player movement
- Press navswitch in to shoot the laser

##### Menu:

On the first screen (with the title), one of the two
boards must press the nav switch button in, this will set them
as Player One and tell the other board that they are now Player
Two. Player One can now look through levels by moving the
navswitch left and/or right. Once a level is selected by
pressing the navswitch in, the level is transmitted to Player
Two, and the game begins.

##### Gameplay:

When the game first starts it may look like nothing has happened,
but do not fear - you are *merely invisible*. Just bump into a wall
or shoot and you will appear for a moment (on both boards). The 
goal is to try to locate the other player and shoot them before 
they do the same to you.

##### End Screen:

When the game is over, it will freeze for a moment, showing both
players. Then on the winning player it will show 'WIN' and flash 
the blue LED, whereas on the losing player it will show 'LOSE'.

## Authors

* **Corbyn Greenwood** - *main menu, transmitting and receiving, nav switch*
* **Benjamin Scott** - *main game loop, display and lasers modules*
* **Joint Effort** - *bug fixing, make file, headers, readme*

## Acknowledgments

* **Electrical Engineering / Computer Engineering / Computer Science** - *ENCE260 Fun Kit repository, Fun Kit board*
