#include "board.h"
Maze maze;

Board::Board(int height, int width) {
    
    Row row = Row(width - 1);

    srand(time(0));
    for (int i = 0; i < height - 2; ++i) map.push_back(row);
    maze.maze(map);
    respawnPointGenerator();
    maze.showMaze(map);
    wallTex = new Texture();
    wallTex->readJPEG("./textures/wall.jpg");
    corrTex = new Texture();
    corrTex->readJPEG("./textures/bush.jpg");
}

void Board::respawnPointGenerator() {
    int i = map.size() - 2;
    int j = 1;
    map[i][j] = 'E';
    bool connected = false;

    while (!connected) {
        if (map[i - 1][j] == ' ' || map[i][j + 1] == ' ')
            connected = true;
        else if (map[i][j] == 'W')
            map[i][j] = ' ';
        i--;
    }

    connected = false;
    i = 1;
    j = map[i].size() - 2;
    map[i][j] = 'P';

    while (!connected) {
        if (map[i + 1][j] == ' ' || map[i][j - 1] == ' ')
            connected = true;
        else if (map[i][j] == 'W')
            map[i][j] = ' ';
        i++;
    }
}

bool Board::isValid(int x, int y, char id) {
    if ((map[x][y] == ' ' || map[x][y] == 'P' || map[x][y] == 'E') && !isOccupied(x, y, id)) {
        if (id == 'P') cout << "MAP ..> " << map[x][y] << " Coord: " << x << " " << y << endl;
        if ((map[x][y] == 'P' && id == 'E') || (map[x][y] == 'E' && id == 'P')) {  
            winner = id;
        }
        return true;
    } else {
        return false;
    }
}

char Board::getWinner(){
    return winner;
}

void Board::setPositionBoard(int x, int y, char id) {
    if (id == 'P') {
        posP[0] = x;
        posP[1] = y;
    } else if (id == 'E') {
        posE[0] = x;
        posE[1] = y;
    }
}

bool Board::isOccupied(int x, int y, char id) {
    if (id == 'P')
        return x == this->posE[0] && y == this->posE[1];
    else
        return x == this->posP[0] && y == this->posP[1];
}

int Board::getTranslationX() {
    return -(int)round(WIDTH * 0.75f);  // WIDTH <--> COLUMNS
}

int Board::getTranslationY() {
    return -(int)round(HEIGHT * 0.50f);  // HEIGHT <--> ROWS
}

void Board::draw() {
    GLfloat material[4];
    int translationX = getTranslationX();
    int translationY = getTranslationY();

    int vertex[8][3] = {
        {0, 1, 1},   // a
        {0, 1, 0},   // b
        {0, 0, 0},   // c
        {0, 0, 1},   // d
        {1, 1, 1},   // e
        {1, 0, 1},   // f
        {1, 0, 0},  // g
        {1, 1, 0}   // h
    };

    int cubeNormalNormalized[8][3] = {
        {-1, 1, 1},    // a
        {-1, 1, -1},   // b
        {-1, -1, -1},  // c
        {-1, -1, 1},   // d
        {1, 1, 1},     // e
        {1, -1, 1},    // f
        {1, -1, -1},   // g
        {1, 1, -1}     // h
    };

    for (int i = 0; i < map.size(); i++) {         
         for (int j = 0; j < map[i].size(); j++) {           
             switch (map[i][j]) {
                 case 'W':
                     material[0] = 1.000f;
                     material[1] = 1.000f;
                     material[2] = 1.000f;
                     material[3] = 1.000f;
                     glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, material);
                     loadTexture(*(wallTex), 64);
                     glBindTexture(GL_TEXTURE_2D, 0);
                     break;
                 case 'E':
                     material[0] = 0.666f;
                     material[1] = 0.000f;
                     material[2] = 0.000f;
                     material[3] = 1.000f;
                     glDisable(GL_TEXTURE_2D);
                     glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, material);
                     break;
                 case 'P':
                     glDisable(GL_TEXTURE_2D);
                     material[0] = 0.294f;
                     material[1] = 0.325f;
                     material[2] = 0.125f;
                     material[3] = 1.000f;
                     glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, material);
                     break;
                 case ' ':
                     material[0] = 1.000f;
                     material[1] = 1.000f;
                     material[2] = 1.000f;
                     material[3] = 1.000f;
                     glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, material);
                     loadTexture(*(corrTex), 64);  // save into a variable
                     glBindTexture(GL_TEXTURE_2D, 0);
                     glShadeModel(GL_FLAT);
                     glBegin(GL_QUADS);
                     glNormal3f(0, 0, 1);
                     glTexCoord2f(-4.0, 0.0); glVertex3i(i * DISTANCE_UNIT + translationX, j * DISTANCE_UNIT + translationY, HEIGHT_WALL * vertex[1][2]);
                     glTexCoord2f(4.0, 0.0); glVertex3i((i + 1) * DISTANCE_UNIT + translationX, j * DISTANCE_UNIT + translationY, HEIGHT_WALL * vertex[2][2]);
                     glTexCoord2f(4.0, 4.0); glVertex3i((i + 1) * DISTANCE_UNIT + translationX, (j + 1) * DISTANCE_UNIT + translationY, HEIGHT_WALL * vertex[6][2]);
                     glTexCoord2f(-4.0, 4.0); glVertex3i(i * DISTANCE_UNIT + translationX, (j + 1) * DISTANCE_UNIT + translationY, HEIGHT_WALL * vertex[7][2]);
                     glEnd();
                     break;
            }
            
            if (map[i][j] != ' ') {
                glShadeModel(GL_SMOOTH);
                glBegin(GL_QUADS);
                glNormal3f(cubeNormalNormalized[1][0], cubeNormalNormalized[1][1], cubeNormalNormalized[1][2]);
                glVertex3i((i + vertex[1][0]) * DISTANCE_UNIT + translationX, (j + vertex[1][1]) * DISTANCE_UNIT + translationY, HEIGHT_WALL * vertex[1][2]);
                glNormal3f(cubeNormalNormalized[2][0], cubeNormalNormalized[2][1], cubeNormalNormalized[2][2]);
                glVertex3i((i + vertex[2][0]) * DISTANCE_UNIT + translationX, (j + vertex[2][1]) * DISTANCE_UNIT + translationY, HEIGHT_WALL * vertex[2][2]);
                glNormal3f(cubeNormalNormalized[6][0], cubeNormalNormalized[6][1], cubeNormalNormalized[6][2]);
                glVertex3i((i + vertex[6][0]) * DISTANCE_UNIT + translationX, (j + vertex[6][1]) * DISTANCE_UNIT + translationY, HEIGHT_WALL * vertex[6][2]);
                glNormal3f(cubeNormalNormalized[7][0], cubeNormalNormalized[7][1], cubeNormalNormalized[7][2]);
                glVertex3i((i + vertex[7][0]) * DISTANCE_UNIT + translationX, (j + vertex[7][1]) * DISTANCE_UNIT + translationY, HEIGHT_WALL * vertex[7][2]);
                glEnd();
                glEnable(GL_TEXTURE_2D);
            }

            if (map[i][j] == 'W') {

                glShadeModel(GL_SMOOTH);
                glBegin(GL_QUADS);
                glNormal3f(cubeNormalNormalized[3][0], cubeNormalNormalized[3][1], cubeNormalNormalized[3][2]);
                glTexCoord2f(-4.0, 0.0);glVertex3i((i + vertex[3][0]) * DISTANCE_UNIT + translationX, (j + vertex[3][1]) * DISTANCE_UNIT + translationY, HEIGHT_WALL * vertex[3][2]);
                glNormal3f(cubeNormalNormalized[5][0], cubeNormalNormalized[5][1], cubeNormalNormalized[5][2]);
                glTexCoord2f(4.0, 4.0);glVertex3i((i + vertex[5][0]) * DISTANCE_UNIT + translationX, (j + vertex[5][1]) * DISTANCE_UNIT + translationY, HEIGHT_WALL * vertex[5][2]);
                glNormal3f(cubeNormalNormalized[4][0], cubeNormalNormalized[4][1], cubeNormalNormalized[4][2]);
                glTexCoord2f(-4.0, 4.0);glVertex3i((i + vertex[4][0]) * DISTANCE_UNIT + translationX, (j + vertex[4][1]) * DISTANCE_UNIT + translationY, HEIGHT_WALL * vertex[4][2]);
                glNormal3f(cubeNormalNormalized[0][0], cubeNormalNormalized[0][1], cubeNormalNormalized[0][2]);
                glTexCoord2f(4.0, 0.0);glVertex3i((i + vertex[0][0]) * DISTANCE_UNIT + translationX, (j + vertex[0][1]) * DISTANCE_UNIT + translationY, HEIGHT_WALL * vertex[0][2]);
                glEnd();

                glShadeModel(GL_SMOOTH);
                glBegin(GL_QUADS);
                glNormal3f(cubeNormalNormalized[3][0], cubeNormalNormalized[3][1], cubeNormalNormalized[3][2]);
                glTexCoord2f(-4.0, 0.0);glVertex3i((i + vertex[3][0]) * DISTANCE_UNIT + translationX, (j + vertex[3][1]) * DISTANCE_UNIT + translationY, HEIGHT_WALL * vertex[3][2]);
                glNormal3f(cubeNormalNormalized[0][0], cubeNormalNormalized[0][1], cubeNormalNormalized[0][2]);
                glTexCoord2f(4.0, 0.0);glVertex3i((i + vertex[0][0]) * DISTANCE_UNIT + translationX, (j + vertex[0][1]) * DISTANCE_UNIT + translationY, HEIGHT_WALL * vertex[0][2]);
                glNormal3f(cubeNormalNormalized[1][0], cubeNormalNormalized[1][1], cubeNormalNormalized[1][2]);
                glTexCoord2f(4.0, 4.0);glVertex3i((i + vertex[1][0]) * DISTANCE_UNIT + translationX, (j + vertex[1][1]) * DISTANCE_UNIT + translationY, HEIGHT_WALL * vertex[1][2]);
                glNormal3f(cubeNormalNormalized[2][0], cubeNormalNormalized[2][1], cubeNormalNormalized[2][2]);
                glTexCoord2f(-4.0, 4.0);glVertex3i((i + vertex[2][0]) * DISTANCE_UNIT + translationX, (j + vertex[2][1]) * DISTANCE_UNIT + translationY, HEIGHT_WALL * vertex[2][2]);
                glEnd();

                glShadeModel(GL_SMOOTH);
                glBegin(GL_QUADS);
                glNormal3f(cubeNormalNormalized[3][0], cubeNormalNormalized[3][1], cubeNormalNormalized[3][2]);
                glTexCoord2f(-4.0, 0.0);glVertex3i((i + vertex[3][0]) * DISTANCE_UNIT + translationX, (j + vertex[3][1]) * DISTANCE_UNIT + translationY, HEIGHT_WALL * vertex[3][2]);
                glNormal3f(cubeNormalNormalized[2][0], cubeNormalNormalized[2][1], cubeNormalNormalized[2][2]);
                glTexCoord2f(4.0, 0.0);glVertex3i((i + vertex[2][0]) * DISTANCE_UNIT + translationX, (j + vertex[2][1]) * DISTANCE_UNIT + translationY, HEIGHT_WALL * vertex[2][2]);
                glNormal3f(cubeNormalNormalized[6][0], cubeNormalNormalized[6][1], cubeNormalNormalized[6][2]);
                glTexCoord2f(4.0, 4.0);glVertex3i((i + vertex[6][0]) * DISTANCE_UNIT + translationX, (j + vertex[6][1]) * DISTANCE_UNIT + translationY, HEIGHT_WALL * vertex[6][2]);
                glNormal3f(cubeNormalNormalized[5][0], cubeNormalNormalized[5][1], cubeNormalNormalized[5][2]);
                glTexCoord2f(-4.0, 4.0);glVertex3i((i + vertex[5][0]) * DISTANCE_UNIT + translationX, (j + vertex[5][1]) * DISTANCE_UNIT + translationY, HEIGHT_WALL * vertex[5][2]);
                glEnd();

                glShadeModel(GL_SMOOTH);
                glBegin(GL_QUADS);
                glNormal3f(cubeNormalNormalized[5][0], cubeNormalNormalized[5][1], cubeNormalNormalized[5][2]);
                glTexCoord2f(-4.0, 0.0);glVertex3i((i + vertex[5][0]) * DISTANCE_UNIT + translationX, (j + vertex[5][1]) * DISTANCE_UNIT + translationY, HEIGHT_WALL * vertex[5][2]);
                glNormal3f(cubeNormalNormalized[6][0], cubeNormalNormalized[6][1], cubeNormalNormalized[6][2]);
                glTexCoord2f(4.0, 0.0);glVertex3i((i + vertex[6][0]) * DISTANCE_UNIT + translationX, (j + vertex[6][1]) * DISTANCE_UNIT + translationY, HEIGHT_WALL * vertex[6][2]);
                glNormal3f(cubeNormalNormalized[7][0], cubeNormalNormalized[7][1], cubeNormalNormalized[7][2]);
                glTexCoord2f(4.0, 4.0);glVertex3i((i + vertex[7][0]) * DISTANCE_UNIT + translationX, (j + vertex[7][1]) * DISTANCE_UNIT + translationY, HEIGHT_WALL * vertex[7][2]);
                glNormal3f(cubeNormalNormalized[4][0], cubeNormalNormalized[4][1], cubeNormalNormalized[4][2]);
                glTexCoord2f(-4.0, 4.0);glVertex3i((i + vertex[4][0]) * DISTANCE_UNIT + translationX, (j + vertex[4][1]) * DISTANCE_UNIT + translationY, HEIGHT_WALL * vertex[4][2]);
                glEnd();

                glShadeModel(GL_SMOOTH);
                glBegin(GL_QUADS);
                glNormal3f(cubeNormalNormalized[0][0], cubeNormalNormalized[0][1], cubeNormalNormalized[0][2]);
                glTexCoord2f(-4.0, 0.0);glVertex3i((i + vertex[0][0]) * DISTANCE_UNIT + translationX, (j + vertex[0][1]) * DISTANCE_UNIT + translationY, HEIGHT_WALL * vertex[0][2]);
                glNormal3f(cubeNormalNormalized[1][0], cubeNormalNormalized[1][1], cubeNormalNormalized[1][2]);
                glTexCoord2f(4.0, 0.0);glVertex3i((i + vertex[1][0]) * DISTANCE_UNIT + translationX, (j + vertex[1][1]) * DISTANCE_UNIT + translationY, HEIGHT_WALL * vertex[1][2]);
                glNormal3f(cubeNormalNormalized[7][0], cubeNormalNormalized[7][1], cubeNormalNormalized[7][2]);
                glTexCoord2f(4.0, 4.0);glVertex3i((i + vertex[7][0]) * DISTANCE_UNIT + translationX, (j + vertex[7][1]) * DISTANCE_UNIT + translationY, HEIGHT_WALL * vertex[7][2]);
                glNormal3f(cubeNormalNormalized[4][0], cubeNormalNormalized[4][1], cubeNormalNormalized[4][2]);
                glTexCoord2f(-4.0, 4.0);glVertex3i((i + vertex[4][0]) * DISTANCE_UNIT + translationX, (j + vertex[4][1]) * DISTANCE_UNIT + translationY, HEIGHT_WALL * vertex[4][2]);
                glEnd();
            }
        }
    }

    for (auto &bullet : bulletList) 
        bullet->draw();
       
    for (auto &bullet : bulletList) {
        if (bullet->state == QUIET) {
            delete bullet;
            bullet = nullptr;
        }
    }
    bulletList.erase(std::remove(bulletList.begin(), bulletList.end(), nullptr), bulletList.end()); 
}


void Board::loadTexture(Texture tex, int dim) {
    unsigned char *buffer2;
    long i, j;
    long k, h;

    buffer2 = (unsigned char *)malloc(dim * dim * 3);

    //-- The texture pattern is subsampled so that its dimensions become dim x dim --
    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++) {
            k = i * tex.height / dim;
            h = j * tex.width / dim;

            buffer2[3 * (i * dim + j)] = tex.image[3 * (k * tex.width + h)];
            buffer2[3 * (i * dim + j) + 1] = tex.image[3 * (k * tex.width + h) + 1];
            buffer2[3 * (i * dim + j) + 2] = tex.image[3 * (k * tex.width + h) + 2];
        }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, dim, dim, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer2);
    free(buffer2);
}

void Board::pushBullet(Bullet *bullet){
    vector<int> u = bulletFinalCoordinates((int)bullet->x, (int) bullet->y, bullet->orientation);
    if(u[2] != 0){
        bullet->initMovement(bullet->x + u[0], bullet->y + u[1], u[2] * 250);
        this->bulletList.push_back(bullet);
    }
}

vector<int> Board::bulletFinalCoordinates(int x, int y, char orientation){
    int i = x; 
    int j = y;
    int calc [] = {0,0,0};

    switch (orientation) {
        case 'N':
            while (map[i][j] == ' ') {
                calc[1] = calc[1] + 1;
                j++;
            }
            break;
        case 'E':
            while (map[i][j] == ' ') {
                calc[0] =  calc[0] + 1;
                i++;
            }
            break;
        case 'S':
            while (map[i][j] == ' ') {
                calc[1] = calc[1] - 1;
                j--;
            }
            break;
        case 'W':
            while (map[i][j] == ' ') {
                calc[0] = calc[0] - 1;
                i--;
            }
            break;
    }
    calc[2] = calc[0] + calc[1];
    if (calc[2] < 0) calc[2] = -calc[2];
    return {calc[0], calc[1], calc[2]};
}

vector<Bullet*> Board::getBullets(){
    return this->bulletList;
}

