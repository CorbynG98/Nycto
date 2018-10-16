# Space Gladiator (Name TBD)

This is an arena shooter game, having multiple arenas to choose
from. The goal is to find and shoot the other player with your
laser. The catch however, is that each of you are only visible
(to you and your opponent) when you shoot, or bump into a wall.

### Getting started

Make sure to have cloned the ENCE260 funkit repository. This
repo has all the required drivers, fonts, utils, etc. required
to run this game on the Electrical Computer Engineering UC Fun
Kit 4.

### Installing

Clone the repository for this game into a folder in the
assignment folder. This is located in the previously cloned
directory, ence260-ucfk4. For example, this README should be
located at ence260-ucfk4/assignment/folder-name/README.md

Now open the terminal and traverse to this directory.
With the ucfunkit plugged into the computer, Enter the command
'make program' (no quotes).
Reset the board if there are problems.

### How to play

- Navswitch directions to control player movement
- Press navswitch in to shoot the laser

On the first screen (with a big 'S' for Start), one of the two
boards must press the nav switch button in, this will set them
as Player One and tell the other board that they are now Player
Two. Player One can now look through levels by moving the
navswitch left and/or right. Once a level is selected by
pressing the navswitch in, the level is transmitted to Player
Two, and the game begins.

When the game first starts it may look like nothing has happened,
but do not fear - you are merely invisible. The goal is to try to
locate the other player and shoot them before they do the same to
you.
When the game is over, it will freeze for about a second, showing
both players. Then on the winning player it will show a W and
flash the blue LED. And on the losing player it will show only an
L.

## Authors

* **Corbyn Greenwood** - *main menu, transmitting and receiving, nav switch*
* **Benjamin Scott** - *main game loop, display and lasers modules*
* **Joint Effort** - *bug fixing, make file, headers, readme*

## Acknowledgments

* **Eletrical Engineering / Computer Engineering / Computer Science** - *ENCE260 Fun Kit repository, Fun Kit board*
