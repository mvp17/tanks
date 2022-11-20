#ifndef BULLET_H
#define BULLET_H

#include "assets/cylinder.h"

class Bullet {
   private:
    float vx, vy;
    long time_remaining;
    int translationX; 
    int translationY;

   public:
    float x, y;
    int state;
    char orientation, shooter;
    Bullet(float, float, char, char);
    void draw();
    void integrate(long);
    void initMovement(int, int, int);
};

#endif