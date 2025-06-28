#ifndef PLAYER_H
#define PLAYER_H
class Player {
private:
    float x;
    float y;
    float z;
    // Aiming direction
    float aimDirX;
    float aimDirY;
    float aimDirZ;
public:
    // Constructor
    Player(float startX = 0.0f, float startY = 0.0f, float startZ = 0.0f)
        : x(startX), y(startY), z(startZ), aimDirX(0.0f), aimDirY(0.0f), aimDirZ(-1.0f) {
    }
    // Drawing method 
    void draw();
    // Movement methods 
    void moveLeft(float speed = 0.1f);
    void moveRight(float speed = 0.1f);
    void moveUp(float speed = 0.1f);
    void moveDown(float speed = 0.1f);
    void moveForward(float speed = 0.1f);
    void moveBackward(float speed = 0.1f);
    // Aiming methods
    void setAimDirection(float dirX, float dirY, float dirZ);
    void getAimDirection(float& dirX, float& dirY, float& dirZ) const;
    // Getter methods
    float getX() const { return x; }
    float getY() const { return y; }
    float getZ() const { return z; }
};
#endif