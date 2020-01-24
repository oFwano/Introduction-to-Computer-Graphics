#include <stdlib.h>
#include <iostream>
#include <time.h>

#include "FruitTetris.h"
using namespace std;

const int shape_I[4][4][4] // I shape
{
  {
    { EMPTY, EMPTY, EMPTY, EMPTY},
    { COLORA, COLORB, COLORC, COLORD},
    { EMPTY, EMPTY, EMPTY, EMPTY},
    { EMPTY, EMPTY, EMPTY, EMPTY}
  },
  {
    { EMPTY, EMPTY, COLORD, EMPTY},
    { EMPTY, EMPTY, COLORC, EMPTY},
    { EMPTY, EMPTY, COLORB, EMPTY},
    { EMPTY, EMPTY, COLORA, EMPTY}
  },
  {
    { EMPTY, EMPTY, EMPTY, EMPTY},
    { COLORA, COLORB, COLORC, COLORD},
    { EMPTY, EMPTY, EMPTY, EMPTY},
    { EMPTY, EMPTY, EMPTY, EMPTY}
  },
  {
    { EMPTY, EMPTY, COLORD, EMPTY},
    { EMPTY, EMPTY, COLORC, EMPTY},
    { EMPTY, EMPTY, COLORB, EMPTY},
    { EMPTY, EMPTY, COLORA, EMPTY}
  }
};
const int shape_L[4][4][4] // L shape
{
  {
    { EMPTY, EMPTY, EMPTY, EMPTY},
    { EMPTY, COLORB, COLORC, COLORD},
    { EMPTY, COLORA, EMPTY, EMPTY},
    { EMPTY, EMPTY, EMPTY, EMPTY}
  },
  {
    { EMPTY, EMPTY, COLORD, EMPTY},
    { EMPTY, EMPTY, COLORC, EMPTY},
    { EMPTY, EMPTY, COLORB, COLORA},
    { EMPTY, EMPTY, EMPTY, EMPTY}
  },
  {
    { EMPTY, EMPTY, EMPTY, COLORA},
    { EMPTY, COLORD, COLORC, COLORB},
    { EMPTY, EMPTY, EMPTY, EMPTY},
    { EMPTY, EMPTY, EMPTY, EMPTY}
  },
  {
    { EMPTY, COLORA, COLORB, EMPTY},
    { EMPTY, EMPTY, COLORC, EMPTY},
    { EMPTY, EMPTY, COLORD, EMPTY},
    { EMPTY, EMPTY, EMPTY, EMPTY}
  }
};
const int shape_LM[4][4][4] //L mirrored
{
  {
    { EMPTY, EMPTY, EMPTY, EMPTY},
    { EMPTY, COLORD, COLORC, COLORB},
    { EMPTY, EMPTY, EMPTY, COLORA},
    { EMPTY, EMPTY, EMPTY, EMPTY}
  },
  {
      { EMPTY, EMPTY, COLORB, COLORA},
      { EMPTY, EMPTY, COLORC, EMPTY},
      { EMPTY, EMPTY, COLORD, EMPTY},
      { EMPTY, EMPTY, EMPTY, EMPTY}
  },
  {
    { EMPTY, COLORA, EMPTY, EMPTY},
    { EMPTY, COLORB, COLORC, COLORD},
    { EMPTY, EMPTY, EMPTY, EMPTY},
    { EMPTY, EMPTY, EMPTY, EMPTY}
  },
  {
    { EMPTY, EMPTY, COLORD, EMPTY},
    { EMPTY, EMPTY, COLORC, EMPTY},
    { EMPTY, COLORA, COLORB, EMPTY},
    { EMPTY, EMPTY, EMPTY, EMPTY}
  }
};
const int shape_S[4][4][4]  //S shape
{
  {
    { EMPTY, EMPTY, EMPTY, EMPTY},
    { EMPTY, EMPTY, COLORC, COLORD},
    { EMPTY, COLORA, COLORB, EMPTY},
    { EMPTY, EMPTY, EMPTY, EMPTY}
  },
  {
    { EMPTY, EMPTY, COLORD, EMPTY},
    { EMPTY, EMPTY, COLORC, COLORB},
    { EMPTY, EMPTY, EMPTY, COLORA},
    { EMPTY, EMPTY, EMPTY, EMPTY}
  },
  {
    { EMPTY, EMPTY, EMPTY, EMPTY},
    { EMPTY, EMPTY, COLORC, COLORD},
    { EMPTY, COLORA, COLORB, EMPTY},
    { EMPTY, EMPTY, EMPTY, EMPTY}
  },
  {
    { EMPTY, EMPTY, COLORD, EMPTY},
    { EMPTY, EMPTY, COLORC, COLORB},
    { EMPTY, EMPTY, EMPTY, COLORA},
    { EMPTY, EMPTY, EMPTY, EMPTY}
  }
};
const int shape_SM[4][4][4]  // S mirrored
{
  {
    { EMPTY, EMPTY, EMPTY, EMPTY},
    { EMPTY, COLORD, COLORC, EMPTY},
    { EMPTY, EMPTY, COLORB, COLORA},
    { EMPTY, EMPTY, EMPTY, EMPTY}
  },
  {
    { EMPTY, EMPTY, EMPTY, COLORA},
    { EMPTY, EMPTY, COLORC, COLORB},
    { EMPTY, EMPTY, COLORD, EMPTY},
    { EMPTY, EMPTY, EMPTY, EMPTY}
  },
  {
    { EMPTY, EMPTY, EMPTY, EMPTY},
    { EMPTY, COLORD, COLORC, EMPTY},
    { EMPTY, EMPTY, COLORB, COLORA},
    { EMPTY, EMPTY, EMPTY, EMPTY}
  },
  {
    { EMPTY, EMPTY, EMPTY, COLORA},
    { EMPTY, EMPTY, COLORC, COLORB},
    { EMPTY, EMPTY, COLORD, EMPTY},
    { EMPTY, EMPTY, EMPTY, EMPTY}
  },

};
const int shape_T[4][4][4]  // T Shape
{
    {
      { EMPTY, EMPTY, COLORA, EMPTY},
      { EMPTY, COLORB, COLORC, COLORD},
      { EMPTY, EMPTY, EMPTY, EMPTY},
      { EMPTY, EMPTY, EMPTY, EMPTY}
    },
    {
      { EMPTY, EMPTY, COLORD, EMPTY},
      { EMPTY, COLORA, COLORC, EMPTY},
      { EMPTY, EMPTY, COLORB, EMPTY},
      { EMPTY, EMPTY, EMPTY, EMPTY}
    },
    {
      { EMPTY, EMPTY, EMPTY,   EMPTY},
      { EMPTY, COLORD, COLORC, COLORB},
      { EMPTY, EMPTY, COLORA,  EMPTY},
      { EMPTY, EMPTY, EMPTY,   EMPTY}
    },
    {
      { EMPTY, EMPTY, COLORB, EMPTY},
      { EMPTY, EMPTY, COLORC, COLORA},
      { EMPTY, EMPTY, COLORD, EMPTY},
      { EMPTY, EMPTY, EMPTY, EMPTY}
    }
  };
FruitTetris::FruitTetris()
{
  srand(time(NULL));
  New_Game();
}
FruitTetris::~FruitTetris() {}
void FruitTetris::Draw_Falling_Piece() //draw a falling piece
{
  for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) { //spawn the piece 1 row above the grid - if condition checks if its outside
          if (falling_piecem[rotate_index][i][j] != EMPTY && (Y_DISPLACEMENT_BOT+(ypos+i)*BLOCK_SIZE) <650) {
              int x1 = X_DISPLACEMENT_RIGHT+(xpos+j)*BLOCK_SIZE;
              int y1 = Y_DISPLACEMENT_BOT+(ypos+i)*BLOCK_SIZE;
              int x2 = x1+BLOCK_SIZE;
              int y2 = y1+BLOCK_SIZE;
              //glColor3f(1,0.5,0);
              Color this_color = Color(falling_piecem[rotate_index][i][j]); // draw square
              SetColor(this_color);
              glBegin(GL_QUADS);
              glVertex2f(x1,y1);
              glVertex2f(x2,y1);
              glVertex2f(x2,y2);
              glVertex2f(x1,y2);
              glEnd();
              glColor3f(0,0,0); // draw border around the square
              glBegin(GL_LINE_LOOP);
              glVertex2f(x1,y1);
              glVertex2f(x2,y1);
              glVertex2f(x2,y2);
              glVertex2f(x1,y2);
              glEnd();
          }
      }
  }
}

void FruitTetris::Draw_Grid(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColor3f(1, 1, 1);
  for(int i = 25; i<=325; i+=BLOCK_SIZE){ //grid lines
    glBegin(GL_LINES);
      glVertex2i(i, 50);
      glVertex2i(i,650);
    glEnd();
  }
  for(int i = 50; i<=650; i+=BLOCK_SIZE){ //grid lines
    glBegin(GL_LINES);
      glVertex2i(25, i);
      glVertex2i(325,i);
    glEnd();
  }
 for (int i = 0; i < HEIGHT; i++) {
     for (int j = 0; j < WIDTH; j++) { //spawn the piece 1 row above the grid - if condition checks if its outside
         if (game_boardm[j][i] != EMPTY) {
             int x1 = X_DISPLACEMENT_RIGHT+(j)*BLOCK_SIZE;
             int y1 = Y_DISPLACEMENT_BOT+(i)*BLOCK_SIZE;
             int x2 = x1+BLOCK_SIZE;
             int y2 = y1+BLOCK_SIZE;
             //glColor3f(1,0.5,0);
             Color this_color = Color(game_boardm[j][i]); // draw square
             SetColor(this_color);
             glBegin(GL_QUADS);
             glVertex2f(x1,y1);
             glVertex2f(x2,y1);
             glVertex2f(x2,y2);
             glVertex2f(x1,y2);
             glEnd();
             glColor3f(0,0,0); // draw border around the square
             glBegin(GL_LINE_LOOP);
             glVertex2f(x1,y1);
             glVertex2f(x2,y1);
             glVertex2f(x2,y2);
             glVertex2f(x1,y2);
             glEnd();
         }
     }
 }
}
void FruitTetris::Rotate_CCW(){
  int tmp_index = rotate_index;
  if (rotate_index == 0){rotate_index = 3;}
  else rotate_index--;

  if (Collision()==1){
    xpos++; //'kick' piece away from left grid border
    if (Collision()==1) {//this kick is to check for I piece since 1 kick isn't enough
      xpos++;
      if (Collision()==3){
        xpos-=2;
        rotate_index=tmp_index;
      }
    }
    else if (Collision()==3){
      xpos--;
      rotate_index=tmp_index;
    }
  }
  else if (Collision()==2){
    xpos--; //'kick' piece away from left grid
    if (Collision()==3){
      xpos++;
      rotate_index=tmp_index;
    }
  }
  else if (Collision()==3){
    xpos++;
    if (Collision()==3){
      xpos--;
      rotate_index=tmp_index;
    }
  }
}

void FruitTetris::SetColor(Color this_color){ // function for setting glCOLOR3f
  switch(this_color){
    case PURPLE: glColor3f(1,0,1); break;
    case RED: glColor3f(1,0,0); break;
    case YELLOW: glColor3f(1,1,0); break;
    case GREEN : glColor3f(0,1,0); break;
    case ORANGE: glColor3f(1,0.5,0); break;
  }
}

void FruitTetris::New_Falling_Piece() //spawns a random shape: (I,L,L-mirrored,S,S-mirrored,T)
{
  rotate_index = 0;
  xpos = 3;
  ypos = 19;
  switch (rand()%6) { // create duplicate of random shape
    case 0:
      memcpy(falling_piecem, shape_I, 4*4*4*sizeof(int));break;
    case 1:
      memcpy(falling_piecem, shape_L, 4*4*4*sizeof(int));break;
    case 2:
      memcpy(falling_piecem, shape_LM, 4*4*4*sizeof(int));break;
    case 3:
      memcpy(falling_piecem, shape_S, 4*4*4*sizeof(int));break;
    case 4:
      memcpy(falling_piecem, shape_SM, 4*4*4*sizeof(int));break;
    case 5:
      memcpy(falling_piecem, shape_T, 4*4*4*sizeof(int));break;
    }
    Color x;
    int randc1 = rand() %5;
    int randc2 = rand() %5;
    int randc3 = rand() %5;
    int randc4 = rand() %5;

    for (int h = 0; h < 4; h++){ // assign colors(fruits) to each block
      for (int i =0 ; i < 4; i++){
        for (int j = 0; j < 4; j++){
          if(falling_piecem[h][i][j] == COLORA){
            falling_piecem[h][i][j] = randc1;
          }
          if(falling_piecem[h][i][j] == COLORB){
            falling_piecem[h][i][j] = randc2;
          }
          if(falling_piecem[h][i][j] == COLORC){
            falling_piecem[h][i][j] = randc3;
          }
          if(falling_piecem[h][i][j] == COLORD){
            falling_piecem[h][i][j] = randc4;
          }
      }
    }
  }
}
void FruitTetris::Run(){
  Draw_Grid();
  Draw_Falling_Piece();
}

void FruitTetris::Input_Left(){
  xpos--;
  if (Collision()==1){xpos++;}
  else if(Collision()==3){xpos++;}
}

void FruitTetris::Input_Right(){
  xpos++;
  if (Collision()==2){xpos--;}
  else if(Collision()==3){xpos--;}


}
void FruitTetris::Input_Down(){
  ypos--;
  if (Collision()==3){ypos++; Update();}
}

void FruitTetris::Tick_Down(){
  ypos--;
  if (Collision()==3){ypos++; Update();}
}
void FruitTetris::New_Game(){
  falling_piecem[4][4][4];
  game_boardm[10][22]; // height space of 22 to draw new pieces outside of the board
  for (int i = 0; i <WIDTH; i++){
    for (int j = 0; j<HEIGHT+2; j++){
      game_boardm[i][j] = EMPTY;
    }
  }
  New_Falling_Piece();
}

int FruitTetris::Collision(){
  for (int i = 0; i < 4; i++){
    for (int j = 0; j < 4; j++){
      if (falling_piecem[rotate_index][i][j] != EMPTY){
        int curr_xpos = X_DISPLACEMENT_RIGHT+(xpos+j)*BLOCK_SIZE;
        int curr_ypos = Y_DISPLACEMENT_BOT+(ypos+i)*BLOCK_SIZE;
        if (curr_xpos < X_DISPLACEMENT_RIGHT) return 1; //piece outside of grid
        else if (curr_xpos > X_DISPLACEMENT_LEFT) return 2; //piece outside of grid
        // piece reach bottom of the grid or reached a piece already placed
        else if (curr_ypos < Y_DISPLACEMENT_BOT || game_boardm[xpos+j][ypos+i] != EMPTY) return 3;
      }
    }
  }
  return 0;
}

void FruitTetris::Update(){
  for (int i=0; i<4; i++){
    for (int j=0; j<4;j++){
      if (falling_piecem[rotate_index][i][j] != EMPTY){
        game_boardm[xpos+j][ypos+i] = falling_piecem[rotate_index][i][j];
      }
    }
  }
  while(1){
    if(Clear_Function() == false){break;}
  }
  //Clear_Function();
  if (Check_GameOver() == true){
    exit(0);
  }
  else New_Falling_Piece();
}

void FruitTetris::HardDrop(){
  while(Collision()!=3){ypos--;}
  ypos++;
  Update();

}

bool FruitTetris::Check_GameOver(){
  for (int i=0; i<WIDTH;i++){
    if (game_boardm[i][20] != EMPTY) return true;
  }
  return false;
}

bool FruitTetris::Clear_line(){
  bool remove;
  bool flag = false;
  for(int i =0; i<HEIGHT;i++){
    remove = true;
    for(int j=0;j<WIDTH;j++){
      if(game_boardm[j][i] == EMPTY){
        remove = false;
        break;
        }
      }
    if (remove == true){
        for(int r = i; r<HEIGHT;r++){
          for(int c = 0;c<WIDTH;c++){
            game_boardm[c][r] = game_boardm[c][r+1];
          }
        }
        flag = true;
    }
    remove = true;
  }
  return flag;
}

bool FruitTetris::Clear_FruitsH(){ //clear fruits horizontally
  int start_j,end_j;
  bool flag = false;
  for (int i =0; i<HEIGHT;i++){ // for each column
    int count = 0;
    int curr_fruit = game_boardm[0][i]; // start at the beginning of the row
    start_j = 0;
    for(int j=0;j<WIDTH;j++){ // check the row to see if there is 3 consecutive fruits from bottom to up
      if (game_boardm[j][i] == EMPTY || game_boardm[j][i] != curr_fruit){ // check to see if the current fruit is matching
        count = 1;                                                        // if its not matching reset counter
        start_j = j;
        curr_fruit = game_boardm[j][i];
      }
      else if (game_boardm[j][i] == curr_fruit){ // if it is matching increase count by 1
        count++;
      }
     if (count == 3){ //if 3 fruits are consecutive shift pieces down
        end_j = j;
        flag = true;
        for (int index = start_j; index <= end_j; index++){
          game_boardm[index][i] = EMPTY;
          int count2 =0;
          while(game_boardm[index][i+count2+1] != EMPTY){
            game_boardm[index][i+count2] = game_boardm[index][i+count2+1];
            game_boardm[index][i+count2+1] = EMPTY;
            count2++;
          }
        }
        count = 0;
      }
    }
  }
  return flag;
}

bool FruitTetris::Clear_FruitsV(){ // clear fruits vertically
  int start_i,end_i;
  bool flag = false;
  for (int j = 0; j <WIDTH;j++){ // for each column
    int count = 0;
    int curr_fruit = game_boardm[j][0];
    start_i = 0;
    for(int i=0;i<HEIGHT;i++){ // check each row from bottom to up
      if(game_boardm[j][i] == EMPTY || game_boardm[j][i] != curr_fruit){
        count = 1;
        start_i = i;
        curr_fruit = game_boardm[j][i];
      }
      else if(game_boardm[j][i] == curr_fruit){
        count++;
      }
      if(count == 3){
        flag = true;
        end_i = i;
        count = 1;
        int count2 = 0;
        for (int index = start_i; index <= end_i; index++){
          game_boardm[j][index] = EMPTY;
          }
        while(game_boardm[j][end_i+count2+1] != EMPTY){
          game_boardm[j][start_i+count2] = game_boardm[j][end_i+count2+1];
          game_boardm[j][count2+end_i+1] = EMPTY;
          count2++;
        }
      }
    }
  }
  return flag;
}

bool FruitTetris::Clear_FruitsD(){ // diagnal clearing '\'
bool flag = false;
for (int i = 2; i<HEIGHT; i++){ // for each column
  int start_j = 0;
  for(int j=0; j<WIDTH; j++){
    int curr_fruit = game_boardm[j][i]; // start at the beginning of the row
    int count = 1;
    /////////* '\' Diagonal Clear */////////

    for(int k = 1; k<3;k++){
      if(game_boardm[j+k][i-k] == EMPTY || game_boardm[j+k][i-k] != curr_fruit){
        break;
      }
      else if(game_boardm[j+k][i-k] == curr_fruit){
        count++;
      }
      if (count == 3){
        flag = true;
        for (int offset = 0; offset>=-2;offset--){
          game_boardm[j-offset][i+offset] = EMPTY;
          int count2 =0;
          while(game_boardm[j-offset][i+offset+count2+1] != EMPTY){
            game_boardm[j-offset][i+offset+count2] = game_boardm[j-offset][i+offset+count2+1];
            game_boardm[j-offset][i+offset+count2+1] = EMPTY;
            count2++;
              }
            }
          }
        }
      }
    }
    return flag;
  }

  bool FruitTetris::Clear_FruitsD2(){ // diagnal clearing '\'
  bool flag = false;
  for (int i = 0; i<HEIGHT; i++){ // for each column
    int start_j = 0;
    for(int j=0; j<WIDTH-2; j++){
      int curr_fruit = game_boardm[j][i]; // start at the beginning of the row
      int count = 1;
      for(int k= 1; k<3;k++){
        if(game_boardm[j+k][i+k] == EMPTY || game_boardm[j+k][i+k] != curr_fruit){
          break;
        }
        else if(game_boardm[j+k][i+k] == curr_fruit){
          count++;
        }
        if (count == 3){
          flag = true;
          for (int offset = 0; offset>=-2;offset--){
            game_boardm[j-offset][i-offset] = EMPTY;
            int count2 =0;
            while(game_boardm[j-offset][i-offset+count2+1] != EMPTY){
              game_boardm[j-offset][i-offset+count2] = game_boardm[j-offset][i-offset+count2+1];
              game_boardm[j-offset][i-offset+count2+1] = EMPTY;
              count2++;
                }
              }
            }
          }
        }
      }
      return flag;
    }

bool FruitTetris::Clear_Function(){
  if(Clear_line()) return true;
  if(Clear_FruitsH()) return true;
  if(Clear_FruitsV()) return true;
  if(Clear_FruitsD()) return true;
  if(Clear_FruitsD2()) return true;
  return false;
}
