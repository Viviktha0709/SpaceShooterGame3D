#include "Player.h"
#include <GL/freeglut.h>
#include <algorithm> // For std::clamp
#include <cmath> // For vector operations
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
void Player::draw() {
    glPushMatrix();
    glTranslatef(x, y, z);
    // Calculate rotation angles based on aim direction
    float rotX, rotY;
    // Calculate the rotation angle around Y-axis (yaw)
    rotY = atan2(aimDirX, aimDirZ) * 180.0f / M_PI;
    // Calculate the rotation angle around X-axis (pitch)
    // We need to calculate the length of the vector projected on the XZ plane
    float aimDirLength = sqrt(aimDirX * aimDirX + aimDirZ * aimDirZ);
    rotX = atan2(aimDirY, aimDirLength) * 180.0f / M_PI;
    // Apply rotations
    glRotatef(rotY, 0.0f, 1.0f, 0.0f); // Yaw rotation (around Y-axis)
    glRotatef(-rotX, 1.0f, 0.0f, 0.0f); // Pitch rotation (around X-axis)
    // Draw the player ship base (cone pointing in the -Z direction)
    glColor3f(0.0f, 1.0f, 0.0f); // Green color
    glutSolidCone(0.2f, 0.6f, 16, 16); // radius, height, slices, stacks
    // Draw a small sphere at the base of the cone to make it look more like a ship
    glTranslatef(0.0f, 0.0f, 0.2f);
    glColor3f(0.0f, 0.8f, 0.0f); 
    glutSolidSphere(0.15f, 12, 12);
    glPopMatrix();
}

void Player::moveLeft(float speed) {
    x = std::max(x - speed, -3.5f); 
}
void Player::moveRight(float speed) {
    x = std::min(x + speed, 3.5f); 
}
void Player::moveUp(float speed) {
    y = std::min(y + speed, 2.5f); 
}
void Player::moveDown(float speed) {
    y = std::max(y - speed, -2.5f); 
}
void Player::moveForward(float speed) {
    z = std::max(z - speed, -3.0f); 
}
void Player::moveBackward(float speed) {
    z = std::min(z + speed, 4.0f); 
}
// Set the player's aim direction
void Player::setAimDirection(float dirX, float dirY, float dirZ) {
    aimDirX = dirX;
    aimDirY = dirY;
    aimDirZ = dirZ;
    // Normalize the aim direction
    float length = sqrt(aimDirX * aimDirX + aimDirY * aimDirY + aimDirZ * aimDirZ);
    if (length > 0) {
        aimDirX /= length;
        aimDirY /= length;
        aimDirZ /= length;
    }
}
// Get the player's current aim direction
void Player::getAimDirection(float& dirX, float& dirY, float& dirZ) const {
    dirX = aimDirX;
    dirY = aimDirY;
    dirZ = aimDirZ;
}