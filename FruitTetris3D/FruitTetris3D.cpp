#include "Angel.h"
#include <GL/gl.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <math.h>


typedef Angel::vec4 point4;
typedef Angel::vec4 color4;
const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

//======================= tetriminos and rotations ==========================//
const vec2 shape_I[4][4]{
  {vec2(-2,0),vec2(-1,0), vec2(0,0), vec2(1,0)},  // colouring scheme follows assignment 1
  {vec2(0,-2),vec2(0,-1), vec2(0,0), vec2(0,1)},
  {vec2(-2,0),vec2(-1,0), vec2(0,0), vec2(1,0)},
  {vec2(0,-2), vec2(0,-1), vec2(0,0), vec2(0,1)}
};
const vec2 shape_L[4][4]{
  {vec2(-1,-1), vec2(-1,0), vec2(0,0), vec2(1,0)},
  {vec2(1,-1), vec2(0,-1), vec2(0,0), vec2(0,1)},
  {vec2(1,1), vec2(1,0), vec2(0,0), vec2(-1,0)},
  {vec2(-1,1), vec2(0,1), vec2(0,0), vec2(0,-1)}
};
const vec2 shape_LM[4][4]{
  {vec2(1,-1), vec2(1,0), vec2(0,0), vec2(-1,0)},
  {vec2(1,1), vec2(0,1), vec2(0,0), vec2(0,-1)},
  {vec2(-1,1), vec2(-1,0), vec2(0,0), vec2(1,0)},
  {vec2(-1,-1), vec2(0,-1), vec2(0,0), vec2(0,1)}
};
const vec2 shape_T[4][4]{
  {vec2(0,0), vec2(1,0), vec2(-1,0), vec2(0,1)},
  {vec2(0,0), vec2(0,1), vec2(0,-1), vec2(-1,0)},
  {vec2(0,0), vec2(-1,0), vec2(1,0), vec2(0,-1)},
  {vec2(0,0), vec2(0,-1), vec2(0,1), vec2(1,0)}
};
const vec2 shape_S[4][4]{
  {vec2(-1,-1), vec2(0,-1),vec2(0,0), vec2(1,0)},
  {vec2(1,-1), vec2(1,0),  vec2(0,0), vec2(0,1)},
  {vec2(-1,-1), vec2(0,-1),vec2(0,0), vec2(1,0)},
  {vec2(1,-1), vec2(1,0),  vec2(0,0), vec2(0,1)}
};
const vec2 shape_SM[4][4]{
  {vec2(1,-1), vec2(0,-1),vec2(0,0), vec2(-1,0)},
  {vec2(1,1), vec2(1,0), vec2(0,0), vec2(0,-1)},
  {vec2(1,-1), vec2(0,-1),vec2(0,0), vec2(-1,0)},
  {vec2(1,1), vec2(1,0), vec2(0,0), vec2(0,-1)}
};

vec2 timer_shape[6][10] = {
  {vec2(-1,2),vec2(-1,-2),vec2(1,2),vec2(1,-2),vec2(-1,2),vec2(1,2),vec2(-1,-2),vec2(1,-2),vec2(-1,-2),vec2(1,-2)},
  {vec2(-1.5,-1),vec2(0,-1),vec2(0,-1),vec2(1.5,-1),vec2(0,-1),vec2(0,1),vec2(0,1),vec2(0,2),vec2(0,2),vec2(-0.7,2)},
  {vec2(-1,2),vec2(1,2),vec2(1,2),vec2(1,0),vec2(1,0),vec2(-1,0),vec2(-1,0),vec2(-1,-2),vec2(-1,-2),vec2(1,-2)},
  {vec2(0,0),vec2(1,0),vec2(1,0),vec2(1,2),vec2(1,0),vec2(1,-2),vec2(1,2),vec2(-1,2),vec2(1,-2),vec2(-1,-2)},
  {vec2(-1,2),vec2(-1,1),vec2(-1,1),vec2(-1,0),vec2(-1,0),vec2(1,0),vec2(1,2),vec2(1,0),vec2(1,0),vec2(1,-2)},
  {vec2(-1,2),vec2(1,2),vec2(-1,2),vec2(-1,0),vec2(1,0),vec2(-1,0),vec2(1,0),vec2(1,-2),vec2(-1,-2),vec2(1,-2)}

};


vec2 current_tile[4]; //vectors of the current shape
vec4 current_tile_colour[4]; //colour of the vectors
vec4 board_vertex_colours[7200];
vec2 tilepos;
mat4 view_matrix,projection_matrix;
// height and width of the view port
int global_width = 1320; // to maintain aspect ratio
int global_height = 750; // to maintain aspect ratio
int rotate_index = 0; // current rotation index for the current piece
int shape_index;   // current shape index
int board[10][20]; // row x column array to keep track of where pieces gets held
bool game_over=false;
// robot arm attributes
point4 points[NumVertices];
color4 colors[NumVertices];
// global variables
GLuint buffer, vPosition, vColor,vao;
// vao for objects grid, board, triles
// vbo for grid postion, gridcolor,boardpos,board color
// current tile position, current tile color
GLuint vao_ids[4];
GLuint vbo_ids[8];
int time_index = 5;

point4 vertices[8] = {
 point4( -0.5, -0.5, 0.5, 1.0 ),
 point4( -0.5, 0.5, 0.5, 1.0 ),
 point4( 0.5, 0.5, 0.5, 1.0 ),
 point4( 0.5, -0.5, 0.5, 1.0 ),
 point4( -0.5, -0.5, -0.5, 1.0 ),
 point4( -0.5, 0.5, -0.5, 1.0 ),
 point4( 0.5, 0.5, -0.5, 1.0 ),
 point4( 0.5, -0.5, -0.5, 1.0 )
};
// colors used are defined here
const color4 robot_color = color4( 0.137255, 0.137255, 0.556863, 1.0 ); // navyblue
const color4 grid_color  = color4(1.0,1.0,1.0,1.0); //white
color4 fruit_colors[5] =
{
 color4( 1.0, 0.0, 1.0, 1.0 ), // purple
 color4( 1.0, 0.0, 0.0, 1.0 ), // red
 color4( 1.0, 1.0, 0.0, 1.0 ), // yellow
 color4( 0.0, 1.0, 0.0, 1.0 ), // green
 color4( 1.0, 0.5, 0.0, 1.0 ) // orange
};

// Parameters controlling the size of the Robot's arm
const GLfloat BASE_HEIGHT = 2.0;
const GLfloat BASE_WIDTH = 5.0;
const GLfloat LOWER_ARM_HEIGHT = 12.0;
const GLfloat LOWER_ARM_WIDTH = 0.5;
const GLfloat UPPER_ARM_HEIGHT = 12.0;
const GLfloat UPPER_ARM_WIDTH = 0.5;
// Shader transformation matrices
mat4 model_view;
GLuint ModelView, Projection;
// Array of rotation angles (in degrees) for each rotation axis
enum { Base = 0, LowerArm = 1, UpperArm = 2, NumAngles = 3 };
int Axis = Base;
GLfloat Theta[NumAngles] = { 0.0 };
// Menu option values
const int Quit = 4;

//----------------------------------------------------------------------------
int Index = 0;
void
quad( int a, int b, int c, int d )
{
 colors[Index] = robot_color; points[Index] = vertices[a]; Index++;
 colors[Index] = robot_color; points[Index] = vertices[b]; Index++;
 colors[Index] = robot_color; points[Index] = vertices[c]; Index++;
 colors[Index] = fruit_colors[0]; points[Index] = vertices[a]; Index++; //different color to make the robot arm easier to see
 colors[Index] = robot_color; points[Index] = vertices[c]; Index++;
 colors[Index] = robot_color; points[Index] = vertices[d]; Index++;
}
void
colorcube()
{
 quad( 1, 0, 3, 2 );
 quad( 2, 3, 7, 6 );
 quad( 3, 0, 4, 7 );
 quad( 6, 5, 1, 2 );
 quad( 4, 5, 6, 7 );
 quad( 5, 4, 0, 1 );
}

//----------------------------------------------------------------------------
/* Define the three parts */
/* Note use of push/pop to return modelview matrix
to its state before functions were entered and use
rotation, translation, and scaling to create instances
of symbols (cube and cylinder */
void
base(mat4 transformation)  //angel robot arm
{
 mat4 instance = ( Translate( 0.0, 0.5 * BASE_HEIGHT, 0.0 ) *
Scale( BASE_WIDTH,
BASE_HEIGHT,
BASE_WIDTH ) );
 glUniformMatrix4fv( ModelView, 1, GL_TRUE,transformation* model_view * instance );
 glDrawArrays( GL_TRIANGLES, 0, NumVertices );
}
//----------------------------------------------------------------------------
void
upper_arm(mat4 transformation) //angel robot arm
{
 mat4 instance = ( Translate( 0.0, 0.5 * UPPER_ARM_HEIGHT, 0.0 ) *
 Scale( UPPER_ARM_WIDTH,
 UPPER_ARM_HEIGHT,
 UPPER_ARM_WIDTH ) );

 glUniformMatrix4fv( ModelView, 1, GL_TRUE,transformation* model_view * instance );
 glDrawArrays( GL_TRIANGLES, 0, NumVertices );
}

//----------------------------------------------------------------------------
void
lower_arm(mat4 transformation) //angel robot arm
{
 mat4 instance = ( Translate( 0.0, 0.5 * LOWER_ARM_HEIGHT, 0.0 ) *
 Scale( LOWER_ARM_WIDTH,
 LOWER_ARM_HEIGHT,
 LOWER_ARM_WIDTH ) );

 glUniformMatrix4fv( ModelView, 1, GL_TRUE,transformation* model_view * instance );
 glDrawArrays( GL_TRIANGLES, 0, NumVertices );
}

//----------------------------------------------------------------------------
//==========================================================================//
//================= tetris game logic functions ============================//

bool check_legal(){ // the move is legal if it is within the grid and is not colliding with an other piece
  float xpos = tilepos.x; //object space
  float ypos = tilepos.y; //object space
  bool flag = true;
  bool flag2 = true;
  for(int i= 0; i<4;i++){
    int xpos2 = xpos + current_tile[i].x;  //object in world space
    int ypos2 = ypos + current_tile[i].y; //object in world space
    if (xpos2 < 0 || xpos2 > 9 || ypos2 >19 || ypos2 < 0) flag = false;
    else if(board[xpos2][ypos2] == true) flag2 = false;
  }
  return (flag&&flag2);
}


bool check_tilebelow(vec2 &postile){ // check if there is tile below the current tile
  bool flag = true;
  bool flag2 = true;
  for(int i=0;i<4;i++){
    int xpos = postile.x + current_tile[i].x;  //object in world space
    int ypos = postile.y + current_tile[i].y - 1; //object in world space
    if (xpos < 0 || xpos > 9 || ypos >19 || ypos < 0) flag = false;
    else if(board[xpos][ypos] == true) flag2 = false;
  }
  return (flag&&flag2);
}



vec2 getTilepos() //get coordinate of tile in world space
{
  float tmp = round(-5+LOWER_ARM_HEIGHT *-sin(M_PI/180 * Theta[1]) +(UPPER_ARM_HEIGHT-0.5) *-cos(M_PI/180 * (90-Theta[1]-Theta[2])));
  float tmp2 = round(-1+BASE_HEIGHT+LOWER_ARM_HEIGHT *cos(M_PI/180* Theta[1]) +(UPPER_ARM_HEIGHT-0.5) *sin(M_PI/180 * (90-Theta[1]-Theta[2])));
	return vec2(tmp,tmp2); // return x,y
}

void update_tilevbo(){ //update the current tile
  tilepos = getTilepos();
  if(game_over){
    vec4 vbo_colours[144];
    for(int i =0; i<144;i++){
      vbo_colours[i] = vec4(0,0,1,1.0); //game over = blue
    }
    glBindBuffer(GL_ARRAY_BUFFER,vbo_ids[5]);
    glBufferSubData(GL_ARRAY_BUFFER,0,144*sizeof(vec4),vbo_colours); // put in vbo buffer
  }
  else if(check_legal()){
    vec4 vbo_colours[144];
    for(int i =0; i<144;i++){
      vbo_colours[i] = current_tile_colour[i/36];
    }
    glBindBuffer(GL_ARRAY_BUFFER,vbo_ids[5]);
    glBufferSubData(GL_ARRAY_BUFFER,0,144*sizeof(vec4),vbo_colours); // put in vbo buffer
    glBindBuffer(GL_ARRAY_BUFFER,0);
  }
  else{
    vec4 vbo_colours[144];
    for(int i =0; i<144;i++){
      vbo_colours[i] = vec4(0.5,0.5,0.5,1.0); //grey = not legal
    }
    glBindBuffer(GL_ARRAY_BUFFER,vbo_ids[5]);
    glBufferSubData(GL_ARRAY_BUFFER,0,144*sizeof(vec4),vbo_colours);// put in vbo buffer
  }
  glBindBuffer(GL_ARRAY_BUFFER,vbo_ids[4]);
  for(int i=0; i<4; i++)
	{
    //object space to world space
		GLfloat x = tilepos.x+current_tile[i].x;
		GLfloat y = tilepos.y+current_tile[i].y;

		//create 8 corners of the cube
		vec4 p1 = vec4(1.0+x, 1.0+y, 0.5, 1.0); // front left B
		vec4 p2 = vec4(1.0+x, 2.0+y, 0.5, 1.0); // front left T
		vec4 p3 = vec4(2.0+x, 1.0+y, 0.5, 1.0); // front right B
		vec4 p4 = vec4(2.0+x, 2.0+y, 0.5, 1.0); // front right T
		vec4 p5 = vec4(1.0+x, 1.0+y, -0.5, 1.0); // back left B
		vec4 p6 = vec4(1.0+x, 2.0+y, -0.5, 1.0); // back left T
		vec4 p7 = vec4(2.0+x, 1.0+y, -0.5, 1.0); // back right B
		vec4 p8 = vec4(2.0+x, 2.0+y, -0.5, 1.0); // back right T

		// turn points into trinagles for drawing
		vec4 triangulated_points[36] =
    { p1, p2, p3,
       p2, p3, p4,
       p5, p6, p7,
       p6, p7, p8,
       p1, p2, p5,
       p2, p5, p6,
		   p3, p4, p7,
       p4, p7, p8,
		   p2, p4, p6,
       p4, p6, p8,
		   p1, p3, p5,
       p3, p5, p7 };
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(triangulated_points)*i, sizeof(triangulated_points), triangulated_points);
	}
}


void spawn_newtile(){
  rotate_index = 0;
  int x = 0;
  shape_index = rand()%6;
  switch(shape_index){
    case 0:
      current_tile[0] = shape_I[rotate_index][0];
      current_tile[1] = shape_I[rotate_index][1];  // set shapes and their vertices
      current_tile[2] = shape_I[rotate_index][2];
      current_tile[3] = shape_I[rotate_index][3];
      break;
    case 1:
      current_tile[0] = shape_L[rotate_index][0];
      current_tile[1] = shape_L[rotate_index][1];
      current_tile[2] = shape_L[rotate_index][2];
      current_tile[3] = shape_L[rotate_index][3];
      break;
    case 2:
      current_tile[0] = shape_S[rotate_index][0];
      current_tile[1] = shape_S[rotate_index][1];
      current_tile[2] = shape_S[rotate_index][2];
      current_tile[3] = shape_S[rotate_index][3];
      break;
    case 3:
      current_tile[0] = shape_SM[rotate_index][0];
      current_tile[1] = shape_SM[rotate_index][1];
      current_tile[2] = shape_SM[rotate_index][2];
      current_tile[3] = shape_SM[rotate_index][3];
      break;
    case 4:
      current_tile[0] = shape_T[rotate_index][0];
      current_tile[1] = shape_T[rotate_index][1];
      current_tile[2] = shape_T[rotate_index][2];
      current_tile[3] = shape_T[rotate_index][3];
      break;
    case 5:
      current_tile[0] = shape_LM[rotate_index][0];
      current_tile[1] = shape_LM[rotate_index][1];
      current_tile[2] = shape_LM[rotate_index][2];
      current_tile[3] = shape_LM[rotate_index][3];
      break;
    default:
    break;
  }
  for(int i=0; i<4;i++){
    x = rand()%5;
    current_tile_colour[i] = fruit_colors[x];  // give each tile a random colour
  }
  update_tilevbo();                            // update the new tile
  glBindVertexArray(vao);
}





void Rotate_CCW(){ // rotate counterclockwise
  if (rotate_index == 3){rotate_index = 0;}
  else{
    rotate_index ++;
  }
  switch(shape_index){
    case 0:
      current_tile[0] = shape_I[rotate_index][0];
      current_tile[1] = shape_I[rotate_index][1];  // initiate new rotation
      current_tile[2] = shape_I[rotate_index][2];
      current_tile[3] = shape_I[rotate_index][3];
      break;
    case 1:
      current_tile[0] = shape_L[rotate_index][0];
      current_tile[1] = shape_L[rotate_index][1];
      current_tile[2] = shape_L[rotate_index][2];
      current_tile[3] = shape_L[rotate_index][3];
      break;
    case 2:
      current_tile[0] = shape_S[rotate_index][0];
      current_tile[1] = shape_S[rotate_index][1];
      current_tile[2] = shape_S[rotate_index][2];
      current_tile[3] = shape_S[rotate_index][3];
      break;
    case 3:
      current_tile[0] = shape_SM[rotate_index][0];
      current_tile[1] = shape_SM[rotate_index][1];
      current_tile[2] = shape_SM[rotate_index][2];
      current_tile[3] = shape_SM[rotate_index][3];
      break;
    case 4:
      current_tile[0] = shape_T[rotate_index][0];
      current_tile[1] = shape_T[rotate_index][1];
      current_tile[2] = shape_T[rotate_index][2];
      current_tile[3] = shape_T[rotate_index][3];
      break;
    case 5:
      current_tile[0] = shape_LM[rotate_index][0];
      current_tile[1] = shape_LM[rotate_index][1];
      current_tile[2] = shape_LM[rotate_index][2];
      current_tile[3] = shape_LM[rotate_index][3];
      break;
    default:
    break;
  }
}

//================= tetris game logic functions end  ===========================
//==============================================================================

void
display( void )
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  // Accumulate ModelView Matrix as we traverse the tree
  glBindVertexArray( vao );

  mat4 matrix_M = view_matrix*Translate(-10,0,0);  // draw angel robot
  model_view = RotateY(Theta[Base]);
  base(matrix_M);
  model_view *= ( Translate(0.0, BASE_HEIGHT, 0.0) *
  RotateZ(Theta[LowerArm]) );
  lower_arm(matrix_M);
  model_view *= ( Translate(0.0, LOWER_ARM_HEIGHT, 0.0) *
  RotateZ(Theta[UpperArm]) );
  upper_arm(matrix_M);

  glUniformMatrix4fv(ModelView,1,GL_TRUE,view_matrix*Translate(-6,0,0));  // view model
  glBindVertexArray(vao_ids[0]);  //draw grid, then the board, then current tile
  glDrawArrays(GL_LINES,0,590);
  glBindVertexArray(vao_ids[1]);
  glDrawArrays(GL_TRIANGLES,0,7200);
  glBindVertexArray(vao_ids[2]);
  glDrawArrays(GL_TRIANGLES,0,144);

  glBindVertexArray(vao_ids[3]);  // draw timer
  glDrawArrays(GL_LINES,0,10);


  glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[3]);
  glBufferData(GL_ARRAY_BUFFER, 7200*sizeof(vec4), board_vertex_colours, GL_DYNAMIC_DRAW); // draw tiles of the board

  glutSwapBuffers();
}


//==============================================================================
//==============================inits===========================================

void init_Time(){
  vec4 time_4points[10];
  for (int i=0; i<10; i++){
    time_4points[i] = vec4(timer_shape[time_index][i].x+15,timer_shape[time_index][i].y+10,1,1);
  }

  vec4  time_colors[10];
  for (int i=0; i<10; i++) time_colors[i] = fruit_colors[0];
  // vao setup for grid
  glBindVertexArray(vao_ids[3]);
  glGenBuffers(2, &vbo_ids[6]);

  //grid vertex positions
  glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[6]);
  glBufferData(GL_ARRAY_BUFFER, (10)*sizeof(vec4), time_4points, GL_DYNAMIC_DRAW);
  glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vPosition);
  //grid vertex colours
  glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[7]);
  glBufferData(GL_ARRAY_BUFFER, (10)*sizeof(vec4), time_colors, GL_DYNAMIC_DRAW);
  glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vColor);

}
void HardDrop(){ // places the piece at the lowest y possible
  if(!check_legal()) {
    if (time_index == -1) game_over = true;
    return;
  }
  while(check_tilebelow(tilepos)){tilepos.y--;}

  for(int i=0; i<4; i++)	{
		int x = tilepos.x+current_tile[i].x;
		int y = tilepos.y+current_tile[i].y;

		board[x][y] = true;
    for(int j=0; j<36; j++)
  	{
  		int index = 36*(10*y+x)+j;
  		board_vertex_colours[index] = current_tile_colour[i];
  	}
	}
  spawn_newtile();
  time_index = 5;
  init_Time();
  glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[3]);
	glBufferData(GL_ARRAY_BUFFER, 7200*sizeof(vec4), board_vertex_colours, GL_DYNAMIC_DRAW);
}
void init_Grid(){
  // 2(64 triangles + (21x11 row x column tiles) )
	vec4 gridpoints[590];
	vec4 gridcolors[590];
  // store vertical vertices
	for (int i=0; i<=10; i++){
		gridpoints[2*i]    = vec4(i+1, 1.0,  0.5, 1);
		gridpoints[2*i+1]  = vec4(i+1, 21.0, 0.5, 1);
		gridpoints[2*i+64] = vec4(i+1, 1.0, -0.5, 1);
		gridpoints[2*i+65] = vec4(i+1, 21.0, -0.5, 1);
	}  // store horizontal vertices
	for (int i=0; i<=20; i++){
		gridpoints[22+2*i]    = vec4(1.0, i+1, 0.5, 1);
		gridpoints[22+2*i+1]  = vec4(11.0, i+1, 0.5, 1);
		gridpoints[22+2*i+64] = vec4(1.0, i+1, -0.5, 1);
		gridpoints[22+2*i+65] = vec4(11.0, i+1, -0.5, 1);
	}
	for (int i=0; i<=20; i++){
		for (int j=0; j<=10; j++){
			gridpoints[128+22*i+2*j]   = vec4(1.0+j, 1.0+i,  0.5, 1); // front left bottom
			gridpoints[128+22*i+2*j+1] = vec4(1.0+j, 1.0+i, -0.5, 1); // back left bottom
		}
  }
	for (int i=0; i<590; i++) gridcolors[i] = grid_color;
    // vao setup for grid
  	glBindVertexArray(vao_ids[0]);
  	glGenBuffers(2, vbo_ids); //create buffer for grid position and colour

  	//grid vertex positions
  	glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[0]); //bind buffer into vbo grid position
  	glBufferData(GL_ARRAY_BUFFER, (590)*sizeof(vec4), gridpoints, GL_STATIC_DRAW); //put gridpoints into buffer
  	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
  	glEnableVertexAttribArray(vPosition);
  	//grid vertex colours
  	glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[1]); //bind buffer into vbo grid colour
  	glBufferData(GL_ARRAY_BUFFER, (590)*sizeof(vec4), gridcolors, GL_STATIC_DRAW);
  	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
  	glEnableVertexAttribArray(vColor);
}

void init_Board(){
  vec4 board_vertex[7200];
  for (int i = 0; i<7200;i++){
    board_vertex_colours[i] = vec4(0.0,0.0,0.0,0.0);
  }
  for (int i=0;i<20;i++){
    for(int j=0;j<10;j++){
      //create 8 corners of the cube
      vec4 p1 = vec4(1+j, 1+i, 0.5, 1); // front left B
      vec4 p2 = vec4(1+j, 2+i, 0.5, 1); // front left T
      vec4 p3 = vec4(2+j, 1+i, 0.5, 1); // front right B
      vec4 p4 = vec4(2+j, 2+i, 0.5, 1); // front right T
      vec4 p5 = vec4(1+j, 1+i, -0.5, 1); // back left B
      vec4 p6 = vec4(1+j, 2+i, -0.5, 1); // back left T
      vec4 p7 = vec4(2+j, 1+i, -0.5, 1); // back right B
      vec4 p8 = vec4(2+j, 2+i, -0.5, 1); // back right T
      // turn points into trinagles for drawing
      vec4 triangulated_points[36] =
      { p1, p2, p3,
        p2, p3, p4,
        p5, p6, p7,
        p6, p7, p8,
        p1, p2, p5,
        p2, p5, p6,
        p3, p4, p7,
        p4, p7, p8,
        p2, p4, p6,
        p4, p6, p8,
        p1, p3, p5,
        p3, p5, p7 };
        for(int k=0;k<36;k++){board_vertex[36*(10*i+j)+k] = triangulated_points[k];}
      }
    }
    for (int i=0; i<10; i++)
  		for (int j=0; j<20; j++)
  			board[i][j] = false;
    glBindVertexArray(vao_ids[1]);
    glGenBuffers(2,&vbo_ids[2]);
    //buffer for board vertex positions
    glBindBuffer( GL_ARRAY_BUFFER, vbo_ids[2] );
    glBufferData( GL_ARRAY_BUFFER, 7200*sizeof(vec4), board_vertex, GL_STATIC_DRAW );
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray( vPosition );
    //buffer for board vertex colours
    glBindBuffer( GL_ARRAY_BUFFER, vbo_ids[3] );
    glBufferData( GL_ARRAY_BUFFER, 7200*sizeof(vec4), board_vertex_colours, GL_DYNAMIC_DRAW );
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray( vColor );
  }

void init_Robot(){ // initaite robot buffers
  Index = 0;
  //reset variables
  Theta[1] = 45;
  Theta[2] = -90;
  Theta[0] = 0;
  colorcube();
  // Create a vertex array object
  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );
  // Create and initialize a buffer object
  glGenBuffers( 1, &buffer );
  glBindBuffer( GL_ARRAY_BUFFER, buffer );
  glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), NULL, GL_DYNAMIC_DRAW );

  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors );
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  glEnableVertexAttribArray( vColor );
  glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)) );
}

void init_NewTileBuffer(){ // initiate tile buffers
  glBindVertexArray(vao_ids[2]);
  glGenBuffers(2,&vbo_ids[4]);
  //buffer for tile vertex positions
  glBindBuffer( GL_ARRAY_BUFFER, vbo_ids[4] );
  glBufferData( GL_ARRAY_BUFFER, 144*sizeof(vec4), NULL, GL_DYNAMIC_DRAW );
  glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray( vPosition );
  //buffer for tile vertex colours
  glBindBuffer( GL_ARRAY_BUFFER, vbo_ids[5] );
  glBufferData( GL_ARRAY_BUFFER, 144*sizeof(vec4), NULL, GL_DYNAMIC_DRAW );
  glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray( vColor );
}

void init( void )
{
  // Load shaders and use the resulting shader program
  GLuint program = InitShader( "vshader81.glsl", "fshader81.glsl" );
  glUseProgram( program );
  vPosition = glGetAttribLocation( program, "vPosition" );
  vColor = glGetAttribLocation( program, "vColor" );
  ModelView = glGetUniformLocation( program, "ModelView" );
  Projection = glGetUniformLocation( program, "Projection");
  glGenVertexArrays(4,&vao_ids[0]);
  game_over = false;
  time_index = 5;
  //initialization
  init_Grid(); // draw grid the farthest away
  init_Board();
  init_NewTileBuffer();
  init_Robot(); // the robot is the closest towards the camera
  init_Time();

  // projection camera
  glViewport( 0, 0, global_width, global_height);
  view_matrix = LookAt(vec3(0,30,30),vec3(0,10,0),vec3(0,1,0));
  projection_matrix = Perspective(45,1.0*global_width/global_height,10,200);
  glUniformMatrix4fv( Projection, 1, GL_TRUE,projection_matrix);

  spawn_newtile();

  //blend, depth colour, antiliasing
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glClearDepth(1.0);
  glEnable(GL_MULTISAMPLE);
  glHint(GL_MULTISAMPLE_FILTER_HINT_NV,GL_NICEST);
  glEnable(GL_LINE_SMOOTH);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glEnable(GL_POINT_SMOOTH);
  glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);


}
//==============================inits===========================================
//==============================================================================

void
reshape( int width, int height )
{
  global_width = width;
  global_height = height;
  glViewport( 0, 0, global_width, global_height);

}

//----------------------------------------------------------------------------
void
menu( int option )
{
 if ( option == Quit ) {
exit( EXIT_SUCCESS );
 }
 else {
Axis = option;
 }
}
//----------------------------------------------------------------------------
void
keyboard( unsigned char key, int x, int y )
{
 switch( key ) {
case 033: // Escape Key
case 'q': case 'Q':
 exit( EXIT_SUCCESS );
 break;
case 'a': case'A': //upper arm
  Theta[1] += 5.0;
  if ( Theta[1] > 360.0 ) { Theta[1] -= 360.0; }
  update_tilevbo();
 break;
case 'd': case'D': //upper arm
  Theta[1] -= 5.0;
  if ( Theta[1] < 0.0 ) { Theta[1] += 360.0; }
  update_tilevbo();
 break;
case 'w': case'W': //lower arm
 Theta[2] += 5.0;
 if ( Theta[2] > 360.0 ) { Theta[2] -= 360.0; }
 update_tilevbo();
 break;
case 's': case'S': //lower arm
 Theta[2] -= 5.0;
 if ( Theta[2] < 0.0 ) { Theta[2] += 360.0; }
 update_tilevbo();
 break;
case 'r': case'R': //restart
  init();
  break;
case 32:
  if(game_over){return;}
  HardDrop();
  break;
default:
  break;
 }
glutPostRedisplay();
}
//----------------------------------------------------------------------------
void special(int key, int x, int y)
{
  switch(key)
  {
    case GLUT_KEY_RIGHT:
      if(glutGetModifiers() == GLUT_ACTIVE_CTRL) view_matrix *= RotateY(10);
      break;
    case GLUT_KEY_LEFT:
      if(glutGetModifiers() == GLUT_ACTIVE_CTRL) view_matrix *= RotateY(-10);
      break;
    case GLUT_KEY_UP:
      Rotate_CCW();
      update_tilevbo();
    default:
      break;
  }
  glutPostRedisplay();
}
void idle(){
  display();
}

void timer(int t){ // glut Timer function, ticks every 10000 miliseconds
  glutTimerFunc(1000,timer,0);
  if (!game_over){
  time_index--;
 }
  if(!game_over && time_index == -1){
    HardDrop();
  }
  if (game_over){
    time_index = 0;
    update_tilevbo();
  }
  init_Time();
  glutPostRedisplay();
}

int main( int argc, char **argv )
{
 
 glutInit( &argc, argv );
 srand(time(NULL));
 glutInitDisplayMode( GLUT_MULTISAMPLE |GLUT_DEPTH| GLUT_RGBA| GLUT_DOUBLE);
 glutInitWindowSize( 1320,750 );
 glutInitContextVersion( 3, 2 );
 glutInitContextProfile( GLUT_CORE_PROFILE );
 glutCreateWindow( "Robot Arm 3D Tetris" );
 glewInit();
 init();
 std::cout << "Press Q to quit, R to restart, WASD to move robot, and UP_ARROW_KEY to rotate" << std::endl;
 glutDisplayFunc( display );
 glutReshapeFunc( reshape );
 glutKeyboardFunc( keyboard );
 glutSpecialFunc( special );
 glutTimerFunc(1000, timer,0);
 glutIdleFunc(idle);
 glutCreateMenu( menu );

 glutMainLoop();
 return 0;
}
