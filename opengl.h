#ifdef __linux__
#include <GL/glut.h>
#elif __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#error "OS not supported!"
#endif

#include <cmath>
#include <iostream>
#include <vector>

#define COLUMNS 30
#define ROWS 20
#define WIDTH 300
#define HEIGHT 300
#define HEIGHT_WALL 20

#define DISTANCE_UNIT 16
#define NUMBER_SUB_BLOCKS 16
#define CENTER_SUB_UNIT 8
#define DISTANCE_SUB_UNIT 1  // DISTANCE_UNIT/NUMBER_SUB_BLOCKS
#define PI 3.141592653589793238462643383279502884197169

#define MOVE 1
#define QUIET 2
#define ROTATE 3
#define DEAD 4

#pragma once
