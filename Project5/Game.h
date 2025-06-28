#ifndef GAME_H
#define GAME_H
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include <vector>
#include <cstddef> // For size_t
struct Star {
    float x;
    float y;
    float z;
};
class Game {
private:
    Player player;
    std::vector<Bullet> bullets;
    std::vector<Enemy> enemies;
    std::vector<Star> stars;
    int enemySpawnCounter;
    int playerEnergy;
    int score;
    bool gameOver;

    int mouseX;
    int mouseY;
    // Helper methods
    void drawBackground();
    void renderText(float x, float y, const char* text);
    bool checkCollision(const Bullet& bullet, const Enemy& enemy);
    bool checkPlayerCollision(const Bullet& bullet);
    void handleCollisions();
    void InitStars(int count);
    void screenToWorld(int screenX, int screenY, float& worldX, float& worldY);
    void shootBullet();
public:
    // Constructor
    Game();
    void init();
    // Main game functions
    void display();
    void update(int value);
    void handleKeys(unsigned char key, int x, int y);
    void handleMouseMove(int x, int y);
    void handleMouseClick(int button, int state, int x, int y);
    void resetGame();
};
#endif