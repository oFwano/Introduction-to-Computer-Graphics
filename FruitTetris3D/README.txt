Francis (QiZhong) Wan
CMPT 361 Fall 2019
Francisw@sfu.ca


All requirements have been implemented. Specifically, upgrading assignment 1 to 3D, viewpoint changes, control of the 
robot arm, game logic, collision detection, and timer. Players will have 5 seconds to move each piece. The game is over
when the player runs out of time and makes an illegal move (collision or out of bounds). After the game is over the player may
not place another piece and the time pauses. A state of game over is displayed by the piece having all blue tiles. Press 'R' or
'r' to restart.

Since eliminating fulls rows and three fruits have been graded in assignment 1 I have left it out of this assignment.
Additionally, instead of implemnting the timer at the top of the tetris board, I have implemented it to the right of the board
for more visibility. 
If the game starts with a black screen after launching, pressing 'r' or 'R' will start the game instantly. 

Keyboard input:

Up Arrow Key: Rotates the falling piece counter clockwise

'A' or 'a': Moves the Upper robot arm right
'S' or 's': Moves the Upper robot arm lefft
'D' or 'd': Moves the lower robot arm right
'W' or 'w': Moves the lower robot arm lefft
'R' or 'r': Restarts the game.
'Q' or 'q': Closes the game window

Space Bar key: Moves the falling piece down until it is placed. Space bar is meant to be pressed only once, do not hold it.

