#include "Game.h"
#include <GL/freeglut.h>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <sstream>
#include <string>

Game::Game()
    : player(0.0f, -0.8f),
    enemySpawnCounter(0),
    playerEnergy(100),
    score(0),
    gameOver(false),
    mouseX(0),
    mouseY(0) {
}

void Game::InitStars(int count) {
    stars.clear();
    float fov = 60.0f;
    float aspect = 800.0f / 600.0f;
    float maxDepth = 100.0f;
    float minDepth = 1.0f;

    for (int i = 0; i < count; ++i) {
        Star s;
        s.z = -((rand() / (float)RAND_MAX) * (maxDepth - minDepth) + minDepth);
        float halfHeight = tan(fov * 0.5f * 3.14159f / 180.0f) * -s.z;
        float halfWidth = halfHeight * aspect;
        s.x = ((rand() / (float)RAND_MAX) * 2 - 1) * halfWidth;
        s.y = ((rand() / (float)RAND_MAX) * 2 - 1) * halfHeight;
        stars.push_back(s);
    }
}

void Game::init() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 800.0 / 600.0, 0.1, 200.0);
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat light_pos[] = { 0.0f, 0.0f, 5.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    srand(static_cast<unsigned int>(time(0)));
    InitStars(300);
}

void Game::drawBackground() {
    if (stars.empty()) return;

    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPointSize(1.5f);
    glBegin(GL_POINTS);
    glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
    for (const auto& star : stars) {
        glVertex3f(star.x, star.y, star.z);
    }
    glEnd();

    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}

// Helper function to render text on screen
void Game::renderText(float x, float y, const char* text) {
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

// Function to convert screen coordinates to world coordinates
void Game::screenToWorld(int screenX, int screenY, float& worldX, float& worldY) {
    // Get viewport, projection and modelview matrices
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLdouble winX, winY, winZ;
    GLdouble posX, posY, posZ;

    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);

    // Window coordinates to normalized device coordinates
    winX = screenX;
    winY = viewport[3] - screenY; // Flip Y coordinate

    // Using two depth points to create a ray
    // First point (near plane)
    winZ = 0.0;
    GLdouble nearX, nearY, nearZ;
    gluUnProject(winX, winY, winZ, modelview, projection, viewport, &nearX, &nearY, &nearZ);

    // Second point (far plane)
    winZ = 1.0;
    GLdouble farX, farY, farZ;
    gluUnProject(winX, winY, winZ, modelview, projection, viewport, &farX, &farY, &farZ);

    // Calculate direction vector
    GLdouble dirX = farX - nearX;
    GLdouble dirY = farY - nearY;
    GLdouble dirZ = farZ - nearZ;

    // Normalize direction
    GLdouble length = sqrt(dirX * dirX + dirY * dirY + dirZ * dirZ);
    dirX /= length;
    dirY /= length;
    dirZ /= length;

    // Set a target Z distance for aiming (negative value to aim into the screen)
    // Moving this closer to the player for more accurate aiming
    GLdouble targetZ = -10.0; // Changed from -5.0 to -10.0

    // Calculate t parameter for the ray equation: point = nearPoint + t * direction
    // We want to find where z = targetZ
    GLdouble t = (targetZ - nearZ) / dirZ;

    // Calculate the final world point at the target z-distance
    posX = nearX + t * dirX;
    posY = nearY + t * dirY;
    posZ = nearZ + t * dirZ;

    worldX = static_cast<float>(posX);
    worldY = static_cast<float>(posY);
}

void Game::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(0.0, 0.0, 5.0,  // Camera
        0.0, 0.0, 0.0,  // Look at
        0.0, 1.0, 0.0); // Up

    drawBackground();

    // Only draw player if not game over
    if (!gameOver) {
        // Calculate aim direction from mouse position with improved accuracy
        float worldX, worldY;
        screenToWorld(mouseX, mouseY, worldX, worldY);

        // Calculate aiming vector from player position to mouse cursor
        float aimDirX = worldX - player.getX();
        float aimDirY = worldY - player.getY();
        float aimDirZ = -10.0f - player.getZ(); // Adjusted depth for more accurate aiming

        // Normalize the direction vector
        float length = sqrt(aimDirX * aimDirX + aimDirY * aimDirY + aimDirZ * aimDirZ);
        if (length > 0) {
            aimDirX /= length;
            aimDirY /= length;
            aimDirZ /= length;
        }

        // Make the crosshair bigger and more visible
        glDisable(GL_LIGHTING);
        glPushMatrix();
        glTranslatef(worldX, worldY, -10.0f);
        glColor3f(1.0f, 0.0f, 0.0f); // Red for crosshair
        glLineWidth(2.0f); // Thicker lines for better visibility
        glBegin(GL_LINES);
        // Horizontal line
        glVertex3f(-0.1f, 0.0f, 0.0f); // Made crosshair twice as big
        glVertex3f(0.1f, 0.0f, 0.0f);
        // Vertical line
        glVertex3f(0.0f, -0.1f, 0.0f);
        glVertex3f(0.0f, 0.1f, 0.0f);
        glEnd();
        glLineWidth(1.0f); // Reset line width
        glPopMatrix();
        glEnable(GL_LIGHTING);

        // Set player's aim direction
        player.setAimDirection(aimDirX, aimDirY, aimDirZ);

        // Draw player with the calculated aiming direction
        player.draw();
    }

    // Draw all bullets
    for (auto& bullet : bullets) {
        bullet.draw();
    }

    // Draw all enemies
    for (auto& enemy : enemies) {
        enemy.draw();
    }

    // Draw UI elements
    std::stringstream ss;
    ss << "Energy: " << playerEnergy;
    renderText(10, 580, ss.str().c_str());

    ss.str("");
    ss << "Score: " << score;
    renderText(10, 550, ss.str().c_str());

    if (gameOver) {
        renderText(320, 300, "GAME OVER");
        renderText(260, 270, "Press 'R' to restart");
    }

    glutSwapBuffers();
}

bool Game::checkCollision(const Bullet& bullet, const Enemy& enemy) {
    // Simple distance-based collision detection (sphere-sphere)
    float bulletX = bullet.getX();
    float bulletY = bullet.getY();
    float bulletZ = bullet.getZ();

    float enemyX = enemy.getX();
    float enemyY = enemy.getY();
    float enemyZ = enemy.getZ();

    float dx = bulletX - enemyX;
    float dy = bulletY - enemyY;
    float dz = bulletZ - enemyZ;
    float distanceSquared = dx * dx + dy * dy + dz * dz;

    // Increased collision radius to make hitting enemies easier
    float collisionRadiusSum = 0.2f + 0.6f; // Bullet radius + half enemy size (increased for easier hits)

    return distanceSquared < (collisionRadiusSum * collisionRadiusSum);
}

bool Game::checkPlayerCollision(const Bullet& bullet) {
    // Check if an enemy bullet hit the player
    float bulletX = bullet.getX();
    float bulletY = bullet.getY();
    float bulletZ = bullet.getZ();

    float playerX = player.getX();
    float playerY = player.getY();
    float playerZ = player.getZ();

    float dx = bulletX - playerX;
    float dy = bulletY - playerY;
    float dz = bulletZ - playerZ;
    float distanceSquared = dx * dx + dy * dy + dz * dz;

    // Make player a bit harder to hit by making collision area smaller
    float collisionRadiusSum = 0.1f + 0.08f; // Slightly smaller player hitbox

    return distanceSquared < (collisionRadiusSum * collisionRadiusSum);
}

void Game::handleCollisions() {
    std::vector<int> bulletsToRemove;
    std::vector<int> enemiesToRemove;

    // Check player bullets against enemies
    for (int i = 0; i < static_cast<int>(bullets.size()); i++) {
        if (bullets[i].isEnemyBullet()) {
            // Check if enemy bullet hit player
            if (checkPlayerCollision(bullets[i])) {
                playerEnergy -= 10; // Decrease player energy
                bulletsToRemove.push_back(i);

                if (playerEnergy <= 0) {
                    playerEnergy = 0;
                    gameOver = true;
                }
            }
        }
        else {
            // Check player bullets against enemies
            for (int j = 0; j < static_cast<int>(enemies.size()); j++) {
                if (checkCollision(bullets[i], enemies[j])) {
                    bulletsToRemove.push_back(i);
                    // Increased damage to enemies to make them easier to destroy
                    enemies[j].decreaseEnergy(50); 

                    if (enemies[j].isDestroyed()) {
                        enemiesToRemove.push_back(j);
                        score += 100; 
                    }
                    break; // A bullet can only hit one enemy
                }
            }
        }
    }

    // Remove bullets (from highest index to lowest to avoid invalidation)
    std::sort(bulletsToRemove.rbegin(), bulletsToRemove.rend());
    for (auto idx : bulletsToRemove) {
        if (idx < static_cast<int>(bullets.size())) {
            bullets.erase(bullets.begin() + idx);
        }
    }

    // Remove enemies (from highest index to lowest to avoid invalidation)
    std::sort(enemiesToRemove.rbegin(), enemiesToRemove.rend());
    for (auto idx : enemiesToRemove) {
        if (idx < static_cast<int>(enemies.size())) {
            enemies.erase(enemies.begin() + idx);
        }
    }
}

void Game::update(int value) {
    if (!gameOver) {
        // Update star field
        for (auto& star : stars) {
            star.z += 0.05f;
            if (star.z > 0.0f) {
                float fov = 60.0f;
                float aspect = 800.0f / 600.0f;
                float newZ = -((rand() / (float)RAND_MAX) * (100.0f - 1.0f) + 1.0f);
                float halfHeight = tan(fov * 0.5f * 3.14159f / 180.0f) * -newZ;
                float halfWidth = halfHeight * aspect;

                star.z = newZ;
                star.x = ((rand() / (float)RAND_MAX) * 2 - 1) * halfWidth;
                star.y = ((rand() / (float)RAND_MAX) * 2 - 1) * halfHeight;
            }
        }

        if (stars.size() < 300 && rand() % 10 == 0) {
            float fov = 60.0f;
            float aspect = 800.0f / 600.0f;
            float z = -((rand() / (float)RAND_MAX) * (100.0f - 1.0f) + 1.0f);
            float halfHeight = tan(fov * 0.5f * 3.14159f / 180.0f) * -z;
            float halfWidth = halfHeight * aspect;

            Star s;
            s.z = z;
            s.x = ((rand() / (float)RAND_MAX) * 2 - 1) * halfWidth;
            s.y = ((rand() / (float)RAND_MAX) * 2 - 1) * halfHeight;

            stars.push_back(s);
        }

        enemySpawnCounter++;
        
        if (enemySpawnCounter >= 400) { 
            float spawnX = ((rand() % 600) - 300) / 100.0f; 
            float spawnY = ((rand() % 400) - 200) / 100.0f; 
            enemies.push_back(Enemy(spawnX, spawnY, -100.0f));
            enemySpawnCounter = 0; 
        }

        for (auto& bullet : bullets) {
            bullet.update();
        }

        for (auto& enemy : enemies) {
            enemy.update();

            // Make enemies shoot less often
            if (enemy.canShoot() && rand() % 30 == 0) { // Changed from 3 to 10 (less frequent shooting)
                // Create enemy bullet
                Bullet enemyBullet(enemy.getX(), enemy.getY(), enemy.getZ(), true); // true = enemy bullet

                // Calculate direction from enemy to player 
                float dirX = player.getX() - enemy.getX();
                float dirY = player.getY() - enemy.getY();
                float dirZ = player.getZ() - enemy.getZ();

                // Set direction (will be normalized in setDirection)
                enemyBullet.setDirection(dirX, dirY, dirZ);
                // Make enemy bullets slower to give player more time to react
                enemyBullet.setSpeed(3.0f); // Slower enemy bullets

                // Add to bullets vector
                bullets.push_back(enemyBullet);

                enemy.resetShootTimer();
            }
        }

        // Handle collisions
        handleCollisions();

        // Remove bullets that are too far
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
            [](const Bullet& b) {
                return (b.isEnemyBullet() && b.getZ() > 10.0f) ||
                    (!b.isEnemyBullet() && b.getZ() < -200.0f);
            }), bullets.end());

        // Remove enemies that are too close to camera
        enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
            [](const Enemy& e) { return e.getZ() > 5.0f; }), enemies.end());
    }

    glutPostRedisplay();
    glutTimerFunc(16, [](int val) {
        static Game* g = reinterpret_cast<Game*>(glutGetWindowData());
        if (g) g->update(val);
        }, 0);
}

void Game::handleKeys(unsigned char key, int x, int y) {
    if (gameOver) {
        if (key == 'r' || key == 'R') {
            // Reset game
            resetGame();
        }
        return;
    }

    // Increased movement speed for better player control
    float moveSpeed = 0.15f; // Increased from 0.1f implicit in the movement methods

    switch (key) {
    case 'w': player.moveUp(moveSpeed); break;
    case 's': player.moveDown(moveSpeed); break;
    case 'a': player.moveLeft(moveSpeed); break;
    case 'd': player.moveRight(moveSpeed); break;
    case 'q': player.moveForward(moveSpeed); break;
    case 'e': player.moveBackward(moveSpeed); break;
    case 32:  // Spacebar - player shoots
        shootBullet();
        break;
    }
    glutPostRedisplay();
}

void Game::handleMouseMove(int x, int y) {
    mouseX = x;
    mouseY = y;
    glutPostRedisplay();
}

void Game::handleMouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !gameOver) {
        shootBullet();
    }
}

void Game::shootBullet() {
    // Get player's current position
    float px = player.getX();
    float py = player.getY();
    float pz = player.getZ();

    // Calculate world coordinates from mouse position directly
    float worldX, worldY;
    screenToWorld(mouseX, mouseY, worldX, worldY);

    // Create a bullet using the player's position
    Bullet bullet(px, py, pz, false); // false = player bullet

    // Calculate direction vector from player to cursor
    float dirX = worldX - px;
    float dirY = worldY - py;
    float dirZ = -10.0f - pz; // Match the target Z used in screenToWorld for consistency

    // Set the bullet speed higher for better responsiveness
    bullet.setSpeed(10.0f); // Increased from 8.0f for faster bullets

    // Set direction (this will normalize it internally)
    bullet.setDirection(dirX, dirY, dirZ);

    // Add bullet to the list
    bullets.push_back(bullet);
}

void Game::resetGame() {
    // Clear all game objects
    bullets.clear();
    enemies.clear();

    // Reset player
    player = Player(0.0f, -0.8f);

    // Reset game variables
    playerEnergy = 100;
    score = 0;
    gameOver = false;
    enemySpawnCounter = 0;

    // Reinitialize stars
    InitStars(300);
}