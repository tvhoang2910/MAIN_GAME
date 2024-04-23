#include"BulletLogic.h"
#include"D:\C++\Visual_studio\SDL_GAME_1\SDL_GAME_1\GameLogic.h"
#include"D:\C++\Visual_studio\SDL_GAME_1\SDL_GAME_1\Globals.h"
void fireBullet() {
    static int bulletCooldown = 0;
    const int BULLET_COOLDOWN_TIME = 10; // Th?i gian ch? gi?a các l?n b?n ð?n

    if (bulletCooldown > 0) {
        bulletCooldown--;
    }

    if (bulletCooldown == 0) {
        Bullet bullet;
        bullet.x = player.x + player.w / 2 - 2;
        bullet.y = player.y;
        //kích thý?c c?a ð?n
        bullet.w = 25;
        bullet.h = 25;

        bullet.active = true; // ð?n ðang ho?t ð?ng
        bullets.push_back(bullet);

        bulletCooldown = BULLET_COOLDOWN_TIME; // init th?i gian ch? gi?a các l?n b?n ð?n
    }
}
void moveBullets() {
    for (int i = 0; i < bullets.size(); i++) {
        if (bullets[i].active) {
            bullets[i].y -= BULLET_SPEED; // Di chuy?n lên trên theo hý?ng y
            if (bullets[i].y < 0) {
                bullets[i].active = false; // out màn h?nh => m?t
            }
        }
    }
}
bool isColliding(const GameObject& obj1, const GameObject& obj2) {
    if (obj1.x < obj2.x + obj2.w &&
        obj1.x + obj1.w > obj2.x &&
        obj1.y < obj2.y + obj2.h &&
        obj1.y + obj1.h > obj2.y) {
        return true;
    }
    return false;
}
