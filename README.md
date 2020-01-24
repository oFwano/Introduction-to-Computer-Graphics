### Introduction to Computer Graphics

This repository contains two projects I have completed from scratch for SFU's CMPT361 - Intro to Computer Graphiccs

The projects include
  1. FruitTetris
  2. Basic RayTracer
  
  
  ** FruitTetris: ** 
  
 FruitTetris is a 2D Tetris clone combined with "falling fruit" for colour matching gameplay. In addition to clearing full rows like 
 standard Tetris, Fruit Tetris adds addionital complexities from Falling Fruits. Each tetromino defined by four blocks, now have a random colour 
 assigned (yellow, orange, red, blue, or purple) to represent a fruit. In addition to erasing a full row of blocks, FruitTetris will also erase three
 fruits which are connected diagonally, horizontally, or vertically. On erasing a full row, the blocks above will all move down one row. One clearing
 consecutive fruits, each block above them will move down by one. 
 
 Controls

  Up Arrow Key: Rotates the falling piece counter clockwise
  Down Arrow Key: Moves the falling piece down by one block
  Left Arrow Key: Moves the  falling piece to the left by one block
  Right Arrow Key: Move the falling piece right by one block

  'D' or 'd': Toggle - Developer mode
  'P' or 'p': Toggle - pauses the game.
  'R' or 'r': Restarts the game.
  'Q' or 'q': Closes the game window

  Space Bar key: Moves the falling piece down until it is placed. Space bar is meant to be pressed only once, do not hold it.

 The game starts instantly upon opening the program. The player may pause with the by pressing 'P' or 'p'.
 By pressing 'D' or 'd' after entering pause mode, the player can turn on/off developer mode which enables the player to move around
 while there is no timer to force the piece down. 

** Basic RayTracer: **

Basic RayTracer made from scratch using OpenGL with C++. This project involved learning sphere and plane intersection, Phong local lighting 
illumination model used for combining ambient, diffuse and specular reflections, recursive raytracing, raycasting and refraction. 
Included in the project involves four scene showcasing different effects the ray tracer can produce. 

![alt text](https://github.com/oFwano/Introduction_to_Computer_Graphics/blob/master/rayT/s4.jpg "Custom scene for RayTracer project")


To run a desired scene enter one of the following into the command line:

./rayT 1

./rayT 2

./rayT 3

./rayT 4

Scene 1: specular and diffuse effects with change q values
Scene 2: reflection and shadow
Scene 3: refraction
Scene 4: custom scene
