#ifndef TANK_H
#define TANK_H

#include "opengl.h"
#include "bullet.h"
#include "assets/cylinder.cpp"
#include "board.h"

#define MAIN_GUN 0x0A
#define WHEEL 0x00

class Tank {
   private:
    float x, y;       //-- Current position
    float vx, vy;     //-- Velocity vector
    float alpha;      //-- Current angle
    float w_angular;  //-- Angular speed;

    long time_remaining;
    float *color;
    char id;
    int state, orientation, translationX, translationY;

    void initMovement(int, int, int);
    void initRotation(int, int);
    char getNextCardinal(bool);
    int getFinalOrientation();
    void drawBox(int[][3]);
    void drawCylinder(int);
    int mod(int, int);

   public:
    Tank(float *, char);
    int getState();
    void setState(int);
    void setPosition(int, int);
    void setTranslation(int, int);
    void integrate(long);
    void draw();
    void keyPressed(unsigned char, Board *);
};

#endif