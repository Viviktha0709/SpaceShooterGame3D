#include <GL/freeglut.h>
#include "Game.h"
Game game;
void display() {
    game.display();
}
void update(int value) {
    game.update(value);
}
void keyboard(unsigned char key, int x, int y) {
    game.handleKeys(key, x, y);
}
void mouseMove(int x, int y) {
    game.handleMouseMove(x, y);
}
void mouseClick(int button, int state, int x, int y) {
    game.handleMouseClick(button, state, x, y);
}
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)width / (GLfloat)height, 0.1, 200.0);
    glMatrixMode(GL_MODELVIEW);
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    int window = glutCreateWindow("3D Space Shooter");
    glutSetWindowData(&game);  
    game.init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, update, 0);
    // Register mouse callbacks
    glutPassiveMotionFunc(mouseMove);  // Track mouse movement without clicks
    glutMouseFunc(mouseClick);         // Handle mouse clicks
    // Set cursor to crosshair for better aiming
    glutSetCursor(GLUT_CURSOR_CROSSHAIR);
    glutMainLoop();
    return 0;
}