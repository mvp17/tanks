#ifndef BOARD_H
#define BOARD_H

#include <stdlib.h>
#include "assets/maze.cpp"
#include "assets/texture.cpp"
#include "bullet.h"
#include "opengl.h"

typedef vector<vector<char>> Map;
typedef vector<char> Row;
typedef vector<Bullet *> Bullets;

class Board {
   public:
    Board(int, int);
    void draw();
    bool isValid(int, int, char);
    void setPositionBoard(int, int, char);
    int getTranslationX();
    int getTranslationY();
    void pushBullet(Bullet *);
    vector<int> bulletFinalCoordinates(int, int, char);
    Bullets getBullets();
    char winner = ' ';
    int posP[2] = {};
    int posE[2] = {};

   private:
    Map map;
    Bullets bulletList;
    Texture *wallTex;
    Texture *corrTex;

    void loadTexture(Texture, int);
    void respawnPointGenerator();
    bool isOccupied(int, int, char);
    char getWinner();
};

#endif