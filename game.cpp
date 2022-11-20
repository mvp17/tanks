#include <string.h>
#include <unistd.h>

#include <chrono>
#include <ctime>
#include <iostream>

#include "./opengl.h"
#include "board.cpp"
#include "bullet.cpp"
#include "tank.cpp"

using namespace std;

int keyflag = 0;
long last_t = 0;
int anglealpha = 0;
int anglebeta = 0;
std::clock_t init;
std::clock_t init_p;
std::clock_t init_e;

Board *board;
Tank *player;
Tank *enemy;

void display();
void keyboard(unsigned char, int, int);
void positionObserver(float, float, int);
void idle();
bool checkWall(int, int);
bool checkPlayerGoal(int, int);
bool checkEnemyGoal(int, int);
void moveEnemy();
void output(GLfloat, GLfloat, string);
void swap(int, int);
int mod(int, int);

int main(int argc, char *argv[]) {
    board = new Board(COLUMNS, ROWS);
    player = new Tank(new float[3]{0.20, 0.80, 0.20}, 'P');
    enemy = new Tank(new float[3]{0.83, 0.00, 0.00}, 'E');
    anglealpha = 90;
    anglebeta = 30;
    init = std::clock();


    player->setTranslation(board->getTranslationX(), board->getTranslationY());
    player->setPosition(1, ROWS - 1);
    board->setPositionBoard(1, ROWS - 1, 'P');

    enemy->setTranslation(board->getTranslationX(), board->getTranslationY());
    enemy->setPosition(COLUMNS - 2, 1);
    board->setPositionBoard(COLUMNS - 2, 1, 'E');

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Tanks board");
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);

    glBindTexture(GL_TEXTURE_2D, 0);
    glutMainLoop();
    return 0;
}

void display() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    positionObserver(anglealpha, anglebeta, 450);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-WIDTH, WIDTH, -HEIGHT, HEIGHT, 10, 1000);
    glMatrixMode(GL_MODELVIEW);

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);

    GLfloat position[4];
    GLfloat color[4];
    //-- Ambient light

    position[0] = 0;
    position[1] = 0;
    position[2] = 60;
    position[3] = 60;
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    color[0] = 0.01;
    color[1] = 0.01;
    color[2] = 0.01;
    color[3] = 1;
    glLightfv(GL_LIGHT0, GL_AMBIENT, color);
    glEnable(GL_LIGHT0);

    board->draw();
    player->draw();
    enemy->draw();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(2.0);

    std::clock_t end = std::clock();
    if (player->getState() == DEAD)
        if (mod((end - init_p) / CLOCKS_PER_SEC, 60) == 2){
            player->setState(QUIET);
            player->setPosition(1, ROWS - 1);
            board->setPositionBoard(1, ROWS - 1, 'P');
        }

    if (enemy->getState() == DEAD) {
        if (mod((end - init_e) / CLOCKS_PER_SEC, 60) == 2) {
            enemy->setState(QUIET);
            enemy->setPosition(COLUMNS - 2, 1);
            board->setPositionBoard(COLUMNS - 2, 1, 'E');
        }
    }

    int seconds = 60 * 3 - mod((end - init) / CLOCKS_PER_SEC, 60);
    GLfloat material[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, material);

    if(board->winner != ' ') {
        string message;
        if (board->winner == 'P') message = "Player wins!";
        else  message = "Enemy wins!"; 
        output(-300, 200, message);
        glutSwapBuffers();
        sleep(2);
        exit(0);
    } else if (seconds == 1) {
        string message = "GAME OVER !:(";
        output(-300, 200, message);
        glutSwapBuffers();
        sleep(2);
        exit(0);
    } else {
        string message = "Time left: " + to_string(seconds) + " s";
        output(-300, 200, message);
        glutSwapBuffers();
    }
    
}

void keyboard(unsigned char c, int x, int y) {
    if (player->getState() != QUIET) return;
    if (c == 'w')
        player->keyPressed(c, board);  // UP
    else if (c == 'd')
        player->keyPressed(c, board);  // RIGHT
    else if (c == 'a')
        player->keyPressed(c, board);  // LEFT
    else if (c == ' ')                 
        player->keyPressed(c,board);   // SHOOT 
    else if (c == 'i' && anglebeta <= (90 - 4))
        anglebeta = (anglebeta + 3);
    else if (c == 'k' && anglebeta >= (-90 + 4))
        anglebeta = anglebeta - 3;
    else if (c == 'j')
        anglealpha = (anglealpha + 3) % 360;
    else if (c == 'l')
        anglealpha = (anglealpha - 3 + 360) % 360;
    glutPostRedisplay();
}

void positionObserver(float alpha, float beta, int radi) {
    float x, y, z;
    float upx, upy, upz;
    float modul;

    x = (float)radi * cos(alpha * 2 * PI / 360.0) * cos(beta * 2 * PI / 360.0);
    y = (float)radi * sin(beta * 2 * PI / 360.0);
    z = (float)radi * sin(alpha * 2 * PI / 360.0) * cos(beta * 2 * PI / 360.0);

    if (beta > 0) {
        upx = -x;
        upz = -z;
        upy = (x * x + z * z) / y;
    } else if (beta == 0) {
        upx = 0;
        upy = 1;
        upz = 0;
    } else {
        upx = x;
        upz = z;
        upy = -(x * x + z * z) / y;
    }

    modul = sqrt(upx * upx + upy * upy + upz * upz);

    upx = upx / modul;
    upy = upy / modul;
    upz = upz / modul;

    gluLookAt(x, y, z, 0.0, 0.0, 0.0, upx, upy, upz);
}

void moveEnemy() {
    if (enemy->getState() != QUIET) return;
    int action[4];
    action[0] = 0;  // w
    action[1] = 1;  // d
    action[2] = 2;  // a
    action[3] = 3;  // ' '

    switch (action[rand() % 5]) {
        case 0:
            enemy->keyPressed('w', board);
            break;
        case 1:
            enemy->keyPressed('d', board);
            break;
        case 2:
            enemy->keyPressed('a', board);
            break;
        case 3:
            enemy->keyPressed(' ', board);
            break;
    }
}

void swap(int &a, int &b) {
    int c = a;
    a = b;
    b = c;
}

void idle() {
    long t;

    t = glutGet(GLUT_ELAPSED_TIME);
    moveEnemy();
    if (last_t == 0)
        last_t = t;
    else {
        for (auto &bullet : board->getBullets()) {
            bullet->integrate(t - last_t);
            if ((int)round(bullet->x) == board->posE[0] && (int)round(bullet->y) == board->posE[1] && bullet->shooter == 'P') {
                enemy->setState(DEAD);
                board->setPositionBoard(-1, -1, 'E');
                init_e = std::clock();
            }
            if ((int)round(bullet->x) == board->posP[0] && (int)round(bullet->y) == board->posP[1] && bullet->shooter == 'E') {
                player->setState(DEAD);
                board->setPositionBoard(-2, -2, 'P');
                init_p = std::clock();
            }
        }
        player->integrate(t - last_t);
        enemy->integrate(t - last_t);
        last_t = t;
    }

    glutPostRedisplay();
}
void output(GLfloat x, GLfloat y, string text) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glColor3f(1, 1, 1);
    glScalef(0.5, 0.5, 0.5);

    for (char s : text) 
        glutStrokeCharacter(GLUT_STROKE_ROMAN, s);

    glPopMatrix();
}

int mod(int n, int m) {
    return (n % m + m) % m;
}
