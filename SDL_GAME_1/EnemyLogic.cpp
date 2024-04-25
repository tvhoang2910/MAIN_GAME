#include "EnemyLogic.h"
#include "GameLogic.h"
#include "Globals.h"
#include "BulletLogic.h"
void spawnEnemy() {
    Enemy enemy;
    enemy.x = rand() % (SCREEN_WIDTH - 40); // Random dich xuat hien
    enemy.y = 0;
    enemy.w = 60; // width
    enemy.h = 60; // height
    enemies.push_back(enemy); // Th�m �?ch v�o vector enemies
}
void moveEnemies() {
    for (int i = 0; i < enemies.size(); i++) {
        enemies[i].y += ENEMY_SPEED; // Di chuy?n �?ch xu?ng d�?i theo h�?ng y
        if (enemies[i].y > SCREEN_HEIGHT) {
            enemies.erase(enemies.begin() + i); //out m�n h?nh = > m?t
            i--; // Gi?m ch? s? i �i 1 sau khi x�a ph?n t? 
        }
    }
}
void checkCollisions(SDL_Renderer* renderer) {
    for (int i = 0; i < bullets.size(); i++) {
        if (bullets[i].active) {
            for (int j = 0; j < enemies.size(); j++) {
                if (enemies[j].alive) {
                    if (isColliding(bullets[i], enemies[j])) {
                        
                        bullets[i].active = false;
                        enemies[j].alive = false;
                        score += 10; 
                    }
                }
            }
        }
    }

    for (int i = 0; i < enemies.size(); i++) {
        if (enemies[i].alive && isColliding(player, enemies[i])) {

        }
    }
}
void moveEnemiesAndCheckHearts(SDL_Renderer* renderer, int& numHeartsRemaining, bool& isRunning, int& score) {
    for (int i = 0; i < enemies.size(); i++) {
        enemies[i].y += ENEMY_SPEED; // Di chuy?n �?ch xu?ng d�?i theo h�?ng y

        bool enemyHitByBullet = false; // Bi?n ki?m tra xem k? �?ch c� b? ��nh b?i �?n hay kh�ng

        for (int j = 0; j < bullets.size(); j++) {
            if (bullets[j].active && isColliding(bullets[j], enemies[i])) {
                bullets[j].active = false; // ��nh d?u �?n �? tr�ng k? �?ch
                enemies.erase(enemies.begin() + i); // X�a k? �?ch khi b? tr�ng �?n
                i--; // Gi?m ch? s? i �i 1 sau khi x�a ph?n t?
                enemyHitByBullet = true; // ��nh d?u k? �?ch b? tr�ng �?n
                score += 10; // T�ng �i?m khi tr�ng �?ch
                break; // Tho�t kh?i v?ng l?p khi k? �?ch b? tr�ng �?n
            }
        }

        if (!enemyHitByBullet && enemies[i].y > SCREEN_HEIGHT) {
            enemies.erase(enemies.begin() + i); // X�a k? �?ch khi tho�t kh?i m�n h?nh
            i--; // Gi?m ch? s? i �i 1 sau khi x�a ph?n t?
            numHeartsRemaining--; // Gi?m s? l�?ng tr�i tim n?u k? �?ch tho�t ra kh?i m�n h?nh
            if (numHeartsRemaining <= 0) {
                isRunning = false; // K?t th�c tr? ch�i khi h?t tr�i tim
                break; // Tho�t kh?i v?ng l?p khi tr? ch�i k?t th�c
            }
        }
    }
}
 