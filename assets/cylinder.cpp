#include "cylinder.h"

void Cylinder::setLength(int length) {
    this->length = length;
}
void Cylinder::setSection(int section) {
    this->section = section;
}
void Cylinder::setCoordinates(float x, float y, float z) {
    this->coordinates = new float[3]{x, y, z};
}
void Cylinder::setOrientation(float alpha, float angleNorm[3]) {
    this->angle = alpha;
    this->angelNorm = angleNorm;
}

void Cylinder::draw() {
    /*
        1) glPushMatrix();          //save the current matrix
        2) glTranslatef(0, 0, 90);  //move to the desired location
        3) draw-whatever();
        4) glPopMatrix();           //restore the old matrix
    */

    static GLUquadric *gluQuadric = NULL;  //"Context" to draw circle based polygons
    if (gluQuadric == NULL)
        gluQuadric = gluNewQuadric();

    glPushMatrix();

    glTranslatef(coordinates[0], coordinates[1], coordinates[2]);
    glRotatef(angle, angelNorm[0], angelNorm[1], angelNorm[2]);

    GLfloat material[4] = {0.01f, 0.01f, 0.01f, 1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, material);
    gluDisk(gluQuadric, 0, section, 10, 1);                       //Base black ;  inner = 0 --> No hollow
    gluCylinder(gluQuadric, section, section, length, 200, 200);  //Walls black 
    glTranslatef(0, 0, length);
    gluDisk(gluQuadric, 0, section, 10, 1);                       //Top black

    glPopMatrix(); 

}


