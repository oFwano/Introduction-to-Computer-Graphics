// CPP program to render a triangle using Shaders
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include <iostream>
#include <string>
#include <thread>
#include <numeric>
#include <bits/stdc++.h>
#include <time.h>
#include <stdlib.h>

#include "FruitTetris.h"


using namespace std;
bool gamepaused = false;
bool developer_mode = false;
FruitTetris tf;

void timer(int t ){ // glut Timer function, ticks every 5000 miliseconds
  glutTimerFunc(500,timer,0);
  if(!gamepaused){
    tf.Tick_Down();
  }
  glutPostRedisplay();
}


void process_kbsFunc(int key, int xx, int yy){ //process keyboard functions for arrow keys
  if(!gamepaused){
  switch(key){
    case GLUT_KEY_UP:   tf.Rotate_CCW(); glutPostRedisplay(); break;
    case GLUT_KEY_DOWN: tf.Input_Down(); glutPostRedisplay(); break;
    case GLUT_KEY_LEFT: tf.Input_Left(); glutPostRedisplay(); break;
    case GLUT_KEY_RIGHT: tf.Input_Right(); glutPostRedisplay(); break;
    }
  }
  if(developer_mode){
    switch(key){
      case GLUT_KEY_UP:   tf.Rotate_CCW(); glutPostRedisplay(); break;
      case GLUT_KEY_DOWN: tf.Input_Down(); glutPostRedisplay(); break;
      case GLUT_KEY_LEFT: tf.Input_Left(); glutPostRedisplay(); break;
      case GLUT_KEY_RIGHT: tf.Input_Right(); glutPostRedisplay(); break;
    }
  }
}
void process_kbFunc(unsigned char key, int x, int y){ //process keyboard functions
  switch(key){
    case 'D':
    case 'd':
      if (gamepaused){
        if(developer_mode){
          developer_mode = false;
        }
        else developer_mode = true;
        break;
      }
    case 'P': //pause game
    case 'p':
      if (!gamepaused){
        gamepaused = true;
        break;
      }
      else{
        gamepaused = false;
        developer_mode = false;
        break;
      }
    case 'R': // Restart/new game
    case 'r':
      tf.New_Game(); glutPostRedisplay(); break;
    case 32:
      tf.HardDrop(); glutPostRedisplay(); break;
    case 'Q':
    case 'q':
      exit(0);
      break;
    default:
      break;
  }
}

void init(void) {
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glMatrixMode(GL_PROJECTION);
  gluOrtho2D(0,350,0,700);
}
void render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  tf.Run();
  glutSwapBuffers();
}

int main (int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowPosition(50, 100);
  glutInitWindowSize(350, 700);
  glutCreateWindow("FruitTetris");
  glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
  init();
  glutDisplayFunc(render);
  glutIdleFunc(render);
  glutSpecialFunc(process_kbsFunc);
  glutKeyboardFunc(process_kbFunc);
  glutTimerFunc(500, timer,0);
  glutMainLoop();
}
