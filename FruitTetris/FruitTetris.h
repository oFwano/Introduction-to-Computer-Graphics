#pragma once
#include <GL/glut.h>


#define BLOCK_SIZE 30
#define X_DISPLACEMENT_RIGHT 25
#define X_DISPLACEMENT_LEFT 295

#define Y_DISPLACEMENT_TOP 590
#define Y_DISPLACEMENT_BOT 50
#define WIDTH 10
#define HEIGHT 20
#define EMPTY -1
#define COLORA -2
#define COLORB -3
#define COLORC -4
#define COLORD -5

enum Color {PURPLE,RED,YELLOW,GREEN,ORANGE};

class FruitTetris{
public:
  FruitTetris();
  ~FruitTetris();
  void Draw_Grid(); //draws grid and blocks placed onto the board
  void Draw_Falling_Piece(); //draws the current falling piece
  void Rotate_CCW(); //rotates falling piece counterclockwise by 90degrees
  void SetColor(Color this_color); //function for setting glColor3f to desired colour
  void New_Falling_Piece(); //spawns a new falling piece
  void Run(); //game loop
  void Input_Left(); //input processing
  void Input_Down(); //input processing
  void Input_Right(); //input processing
  void Tick_Down(); //move the piece down
  void New_Game(); //reset board, starting a new game
  int Collision(); //check for collusion error code 1: right wall 2: left wall
  void Update(); // update the game board, place falling piece on to the board
  void HardDrop(); //drops the current falling piece to the lowest possible positiong
  bool Check_GameOver(); //check if game is over. no legal moves
  bool Clear_line();
  bool Clear_FruitsH();
  bool Clear_FruitsV();
  bool Clear_FruitsD();
  bool Clear_FruitsD2();
  bool Clear_Function();


private:
  int rotate_index;
  int xpos;
  int ypos;
  int mxpos; // x pos relative to game_boardm matrix
  int mypos; // y pos relative to game_boardm matrix
  int falling_piecem[4][4][4]; //matrix for current falling piece
  int game_boardm[10][22]; //matrix for holding game board
  bool gameover;
};
