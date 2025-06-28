#include <GL/freeglut.h>
#include "Bullet.h"
#include <cmath>
// Default constructor
Bullet::Bullet() : x(0), y(0), z(0), dirX(0), dirY(0), dirZ(0), speed(0), enemyBullet(false) {
}
// Constructor with speed parameter
Bullet::Bullet(float xPos, float yPos, float zPos, float bulletSpeed, bool isEnemy)
    : x(xPos), y(yPos), z(zPos), dirX(0), dirY(0), dirZ(0), speed(bulletSpeed), enemyBullet(isEnemy) {
}
// 4-parameter constructor (for the version being called with 4 args)
Bullet::Bullet(float xPos, float yPos, float zPos, bool isEnemy)
    : x(xPos), y(yPos), z(zPos), dirX(0), dirY(0), dirZ(0), speed(5.0f), enemyBullet(isEnemy) {
    // Using default speed of 5.0 when not specified
}
void Bullet::draw() {
    glPushMatrix();
    glTranslatef(x, y, z);  
    if (enemyBullet) {
        glColor3f(1.0f, 0.5f, 0.0f);  // Orange 
    }
    else {
        glColor3f(1.0f, 1.0f, 0.0f);   
    }
    glutSolidSphere(0.5, 20, 20);  // sphere
    glPopMatrix();
}
void Bullet::update() {
    // Move bullet based on its direction and speed
    x += dirX * speed;
    y += dirY * speed;
    z += dirZ * speed;
}
void Bullet::setDirection(float xDir, float yDir, float zDir) {
    dirX = xDir;
    dirY = yDir;
    dirZ = zDir;
    float length = std::sqrt(dirX * dirX + dirY * dirY + dirZ * dirZ);
    if (length > 0) {
        dirX /= length;
        dirY /= length;
        dirZ /= length;
    }
}