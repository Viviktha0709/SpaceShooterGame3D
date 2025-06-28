#ifndef ENEMY_H
#define ENEMY_H
class Enemy {
private:
    float x;
    float y;
    float z;
    int energy;
    int shootTimer;
public:
    // Constructor
    Enemy(float startX, float startY, float startZ, int startEnergy = 100)
        : x(startX), y(startY), z(startZ), energy(startEnergy), shootTimer(0) {
    }
    // Drawing method
    void draw();
    // Update method for movement
    void update();
    // Shooting methods
    bool canShoot() const { return shootTimer >= 100; }
    void resetShootTimer() { shootTimer = 0; }
    // Energy methods
    void decreaseEnergy(int amount) { energy -= amount; }
    bool isDestroyed() const { return energy <= 0; }
    // Getter methods
    float getX() const { return x; }
    float getY() const { return y; }
    float getZ() const { return z; }
};
#endif