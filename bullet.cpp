#include "opengl.h"
#include "bullet.h"

Bullet::Bullet(float x, float y, char shooter, char orientation) {
    this->x = x;
    this->y = y;
    this->shooter = shooter;
    this->orientation = orientation;
    this->translationX =  -(int)round(WIDTH * 0.75f);  // WIDTH <--> COLUMNS
    this->translationY =  -(int)round(HEIGHT * 0.50f);  // HEIGHT <--> ROWS
}

void Bullet::draw() {
    glPushMatrix();
    glTranslatef(x * DISTANCE_UNIT + translationX, + y * DISTANCE_UNIT + translationY, 0);
    glDisable(GL_TEXTURE_2D);
    Cylinder *cyl = new Cylinder();
    cyl->setLength(3);
    cyl->setSection(1);
    cyl->setCoordinates(
        CENTER_SUB_UNIT,
        CENTER_SUB_UNIT,
        10.0f);

    if (orientation == 'W' || orientation == 'E') cyl->setOrientation(90.0f, new float[3]{0.0f, 1.0f, 0.0f});
    else if (orientation == 'N' || orientation == 'S') cyl->setOrientation(00.0f, new float[3]{0.0f, 1.0f, 0.0f});

    cyl->draw();
    delete cyl;
    glEnable(GL_TEXTURE_2D);

    glPopMatrix();
}

void Bullet::initMovement(int destination_x, int destination_y, int duration) {
    vx = (destination_x - x) / duration;
    vy = (destination_y - y) / duration;
    state = MOVE;
    time_remaining = duration;
}

void Bullet::integrate (long t) {
    if(state == MOVE && t < time_remaining) {
        x = x + vx * t;
        y = y + vy * t;
        time_remaining -= t;
    }
    else if (state == MOVE && t >= time_remaining) {
        x = x + vx * time_remaining;
        y = y + vy * time_remaining;
        this->state = QUIET; 
        x = round(x); 
        y = round(y);
    }
}