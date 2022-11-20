#ifndef CYLINDER_H
#define CYLINDER_H
#include "../opengl.h"

class Cylinder {
   private:
    int length, section;
    float *coordinates;
    GLfloat angle;
    float *angelNorm;
    int rotation;

   public:
    Cylinder(){};
    void setLength(int);
    void setSection(int);
    void setCoordinates(float, float, float);
    void setOrientation(float, float[3]);
    void draw();
};
#endif