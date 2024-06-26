#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <cstdlib>
#define PI 3.14159265f

GLfloat ballRadius = 0.2f;
GLfloat ballX = 0.0f;
GLfloat ballY = 0.0f;
GLfloat ballXMax, ballXMin, ballYMax, ballYMin;
GLfloat xSpeed = 0.02f;
GLfloat ySpeed = 0.007f;
GLfloat xAcceleration = 0.0000f;
GLfloat yAcceleration = -0.0005f;
int refreshMills = 30;
int score = 0;
int cur_mx = 0, cur_my = 0;
bool arcball_on = false;
GLdouble clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop;
GLdouble windowWidth = 800;
GLdouble windowHeight = 800;

void color() {
    if (score <= 5)
        glColor3f(1.0, 0.0, 0.0);
    else
        glColor3ub(rand() % 250, rand() % 250, rand() % 250);
}

void balldisp() {
    glLoadIdentity();
    glTranslatef(ballX, ballY, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
    color();
    glVertex2f(0.0f, 0.0f);
    int numSegments = 100;
    GLfloat angle;
    for (int i = 0; i <= numSegments; i++) {
        angle = i * 2.0f * PI / numSegments;
        glVertex2f(cos(angle) * ballRadius, sin(angle) * ballRadius);
    }
    glEnd();

    xSpeed += xAcceleration;
    ySpeed += yAcceleration;
    ballX += xSpeed;
    ballY += ySpeed;

    if (ballX > ballXMax) {
        ballX = ballXMax;
        xSpeed = -xSpeed;
    } else if (ballX < ballXMin) {
        ballX = ballXMin;
        xSpeed = -xSpeed;
    }
    if (ballY > ballYMax) {
        ballY = ballYMax;
        ySpeed = -ySpeed;
    } else if (ballY < ballYMin) {
        ballY = ballYMin;
        ySpeed = -ySpeed;
    }
    if (ballY == ballYMin) {
        std::cout << "Final Score: " << score << std::endl;
        exit(0);
    }
}

void scoredisp() {
    int z = score;
    glColor3f(1.0, 0.0, 0.0);
    glLoadIdentity();
    glRasterPos2f(-1, 1);
    const char *scoreText = "SCORE:";
    for (const char *c = scoreText; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ' ');

    std::string scoreStr = std::to_string(score);
    for (char c : scoreStr) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    balldisp();
    scoredisp();
    glutSwapBuffers();
}

void onMotion(int x, int y) {
    if (arcball_on) {
        cur_mx = x;
        cur_my = y;
    }
}

void reshape(GLsizei width, GLsizei height) {
    if (height == 0) height = 1;
    GLfloat aspect = (GLfloat)width / (GLfloat)height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (width >= height) {
        clipAreaXLeft = -1.0 * aspect;
        clipAreaXRight = 1.0 * aspect;
        clipAreaYBottom = -1.0;
        clipAreaYTop = 1.0;
    } else {
        clipAreaXLeft = -1.0;
        clipAreaXRight = 1.0;
        clipAreaYBottom = -1.0 / aspect;
        clipAreaYTop = 1.0 / aspect;
    }
    gluOrtho2D(clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop);
    ballXMin = clipAreaXLeft + ballRadius;
    ballXMax = clipAreaXRight - ballRadius;
    ballYMin = clipAreaYBottom + ballRadius;
    ballYMax = clipAreaYTop - ballRadius;
}

void Timer(int value) {
    glutPostRedisplay();
    glutTimerFunc(refreshMills, Timer, 0);
}

void onMouse(int button, int state, int x, int y) {
    cur_mx = x;
    cur_my = y;
    GLfloat X = ((-((float)windowWidth / 2) + float(cur_mx)) / (float)(windowWidth))*2;
    GLfloat Y = ((+((float)windowHeight / 2) - float(cur_my)) / (float)(windowHeight))*2;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        arcball_on = true;
        if ((X >= (ballX - ballRadius) && Y >= (ballY - ballRadius)) &&
            (X <= (ballX + ballRadius) && Y <= (ballY + ballRadius))) {
            xSpeed += 0.01f;
            ySpeed += 0.02f;
        }
    } else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        arcball_on = false;
        if ((X >= (ballX - ballRadius) && Y >= (ballY - ballRadius)) &&
            (X <= (ballX + ballRadius) && Y <= (ballY + ballRadius))) {
            score += 1;
        }
    }
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Bouncing Ball");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(onMouse);
    glutMotionFunc(onMotion);
    glutTimerFunc(0, Timer, 0);
    glutMainLoop();
    return 0;
}
