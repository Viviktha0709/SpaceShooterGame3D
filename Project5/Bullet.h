#ifndef BULLET_H
#define BULLET_H
class Bullet {
private:
    float x;
    float y;
    float z;
    float dirX;
    float dirY;
    float dirZ;
    float speed;
    bool enemyBullet;
public:
    // Constructors
    Bullet();
    Bullet(float xPos, float yPos, float zPos, float bulletSpeed, bool isEnemy);
    Bullet(float xPos, float yPos, float zPos, bool isEnemy); // 4-argument constructor
    // Getters
    float getX() const { return x; }
    float getY() const { return y; }
    float getZ() const { return z; }
    float getDirX() const { return dirX; }
    float getDirY() const { return dirY; }
    float getDirZ() const { return dirZ; }
    float getSpeed() const { return speed; }
    bool isEnemyBullet() const { return enemyBullet; }
    // Setters
    void setX(float xPos) { x = xPos; }
    void setY(float yPos) { y = yPos; }
    void setZ(float zPos) { z = zPos; }
    void setSpeed(float newSpeed) { speed = newSpeed; }
    void setEnemyBullet(bool isEnemy) { enemyBullet = isEnemy; }
    // Methods
    void draw();
    void update();
    void setDirection(float xDir, float yDir, float zDir);
};
#endif