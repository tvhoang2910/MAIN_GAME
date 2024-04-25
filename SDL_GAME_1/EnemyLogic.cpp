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
    enemies.push_back(enemy); // Thêm ð?ch vào vector enemies
}
void moveEnemies() {
    for (int i = 0; i < enemies.size(); i++) {
        enemies[i].y += ENEMY_SPEED; // Di chuy?n ð?ch xu?ng dý?i theo hý?ng y
        if (enemies[i].y > SCREEN_HEIGHT) {
            enemies.erase(enemies.begin() + i); //out màn h?nh = > m?t
            i--; // Gi?m ch? s? i ði 1 sau khi xóa ph?n t? 
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
        enemies[i].y += ENEMY_SPEED; // Di chuy?n ð?ch xu?ng dý?i theo hý?ng y

        bool enemyHitByBullet = false; // Bi?n ki?m tra xem k? ð?ch có b? ðánh b?i ð?n hay không

        for (int j = 0; j < bullets.size(); j++) {
            if (bullets[j].active && isColliding(bullets[j], enemies[i])) {
                bullets[j].active = false; // Ðánh d?u ð?n ð? trúng k? ð?ch
                enemies.erase(enemies.begin() + i); // Xóa k? ð?ch khi b? trúng ð?n
                i--; // Gi?m ch? s? i ði 1 sau khi xóa ph?n t?
                enemyHitByBullet = true; // Ðánh d?u k? ð?ch b? trúng ð?n
                score += 10; // Tãng ði?m khi trúng ð?ch
                break; // Thoát kh?i v?ng l?p khi k? ð?ch b? trúng ð?n
            }
        }

        if (!enemyHitByBullet && enemies[i].y > SCREEN_HEIGHT) {
            enemies.erase(enemies.begin() + i); // Xóa k? ð?ch khi thoát kh?i màn h?nh
            i--; // Gi?m ch? s? i ði 1 sau khi xóa ph?n t?
            numHeartsRemaining--; // Gi?m s? lý?ng trái tim n?u k? ð?ch thoát ra kh?i màn h?nh
            if (numHeartsRemaining <= 0) {
                isRunning = false; // K?t thúc tr? chõi khi h?t trái tim
                break; // Thoát kh?i v?ng l?p khi tr? chõi k?t thúc
            }
        }
    }
}
 