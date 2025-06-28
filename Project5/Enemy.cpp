#include "Enemy.h"
#include <GL/freeglut.h>
#include <cstdlib> // For rand()
#include <cmath>
void Enemy::draw() {
    glPushMatrix();
    glTranslatef(x, y, z);
    // Add some material properties to make the cube look better
    GLfloat mat_ambient[] = { 0.8f, 0.0f, 0.0f, 1.0f };
    GLfloat mat_diffuse[] = { 1.0f, 0.2f, 0.2f, 1.0f };
    GLfloat mat_specular[] = { 1.0f, 0.5f, 0.5f, 1.0f };
    GLfloat mat_shininess[] = { 50.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    
    glutSolidCube(2.0f);  
    glPopMatrix();
}


void Enemy::update() {
    // Move enemy toward the player (positive Z direction)..... meaning we have right hand coordinate system
    z += 0.1f;
    // Slight random sideways movement
    x += (((rand() % 100) - 50) / 1000.0f);

    
    // Increment shoot timer
    shootTimer++;
}