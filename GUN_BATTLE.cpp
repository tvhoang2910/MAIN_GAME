#define SDL_MAIN_HANDLED
#include <iostream>
#include <SDL.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <SDL_ttf.h>
#include <string>
#include<SDL_image.h>
using namespace std;

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 600;
const int PLAYER_SPEED = 1;
const int BULLET_SPEED = 1;
const int ENEMY_SPEED = 1;
const int ENEMY_SPAWN_TIMER = 1000;

struct GameObject {
    int x, y, w, h;
};

struct Bullet : public GameObject {
    bool active;
};

struct Enemy : public GameObject {
    bool alive;
};
GameObject player = { SCREEN_WIDTH / 2, SCREEN_HEIGHT - 50, 40, 40 };

vector<Bullet> bullets;
vector<Enemy> enemies;
int score = 0;
Uint32 lastEnemySpawn = 0;

void handleInput(SDL_Renderer* renderer, GameObject& player);
void movePlayer(GameObject& player);
void fireBullet();
void spawnEnemy();
void moveBullets();
void moveEnemies();
void checkCollisions(SDL_Renderer* renderer);
void drawPlayer(SDL_Renderer* renderer, GameObject player, SDL_Texture* playerTexture);
void drawBullets(SDL_Renderer* renderer);
void drawEnemies(SDL_Renderer* renderer);
void drawScore(SDL_Renderer* renderer, TTF_Font* font, int score);

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("GUN BATTLE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    TTF_Init();
    TTF_Font* font = TTF_OpenFont("D:/C++/Visual_studio/SDL_GAME_1/lazy.ttf", 25);

    srand(static_cast<unsigned int>(time(NULL)));

    bool isRunning = true;
    SDL_Event event;

    // Tải và tạo kết cấu background
    SDL_Surface* backgroundSurface = SDL_LoadBMP("D:/C++/Visual_studio/SDL_GAME_1/x64/Debug/bkground.bmp");
    if (backgroundSurface == NULL) {
        cout << "failed";
    }
    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);
    SDL_Surface* playerSurface = IMG_Load("D:\\C++\\Visual_studio\\SDL_GAME_1\\x64\\Debug\\bkground.png");
    SDL_Texture* playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false; // Đặt biến isRunning thành false để kết thúc vòng lặp chính
            }
        }

        handleInput(renderer, player);
        movePlayer(player);
        fireBullet();

        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastEnemySpawn >= ENEMY_SPAWN_TIMER) {
            spawnEnemy();
            lastEnemySpawn = currentTime;
        }

        moveBullets();
        moveEnemies();
        checkCollisions(renderer);

        // Vẽ background
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        drawPlayer(renderer, player, playerTexture);
        drawBullets(renderer);
        drawEnemies(renderer);
        drawScore(renderer, font, score);

        SDL_RenderPresent(renderer);

    }

    TTF_CloseFont(font);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
void handleInput(SDL_Renderer* renderer, GameObject& player) {
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    if (keystate[SDL_SCANCODE_LEFT] && player.x > 0) {
        player.x -= PLAYER_SPEED;
    }
    if (keystate[SDL_SCANCODE_RIGHT] && player.x < SCREEN_WIDTH - player.w) {
        player.x += PLAYER_SPEED;
    }
    if (keystate[SDL_SCANCODE_SPACE]) {
        fireBullet();
    }
}

void movePlayer(GameObject& player) {
    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    if (keystate[SDL_SCANCODE_LEFT] && player.x > 0) {
        player.x -= PLAYER_SPEED;
    }

    if (keystate[SDL_SCANCODE_RIGHT] && player.x < SCREEN_WIDTH - player.w) {
        player.x += PLAYER_SPEED;
    }
}


void fireBullet() {
    static int bulletCooldown = 0;
    const int BULLET_COOLDOWN_TIME = 50; // Thời gian chờ giữa các lần bắn đạn

    if (bulletCooldown > 0) {
        bulletCooldown--;
    }

    if (bulletCooldown == 0) {
        Bullet bullet;
        bullet.x = player.x + player.w / 2 - 2;
        bullet.y = player.y;
        bullet.w = 20; // dan rong
        bullet.h = 20; // dan cao
        bullet.active = true; // danh dau dan dang hoat dong
        bullets.push_back(bullet);

        bulletCooldown = BULLET_COOLDOWN_TIME; // Thiết lập thời gian chờ giữa các lần bắn đạn
    }
}


void spawnEnemy() {
    Enemy enemy;
    enemy.x = rand() % (SCREEN_WIDTH - 40); // Random dich xuat hien
    enemy.y = 0;
    enemy.w = 40; // width
    enemy.h = 40; // height
    enemies.push_back(enemy); // Thêm đ?ch vào vector enemies
}

void moveBullets() {
    for (int i = 0; i < bullets.size(); i++) {
        if (bullets[i].active) {
            bullets[i].y -= BULLET_SPEED; // Di chuyển lên trên theo hướng y
            if (bullets[i].y < 0) {
                bullets[i].active = false; // Đánh dấu không còn hoạt động nếu ra khỏi màn hình
            }
        }
    }
}

void moveEnemies() {
    for (int i = 0; i < enemies.size(); i++) {
        enemies[i].y += ENEMY_SPEED; // Di chuyển địch xuống dưới theo hướng y
        if (enemies[i].y > SCREEN_HEIGHT) {
            enemies.erase(enemies.begin() + i); // Xóa địch ra khỏi màn hình nếu vượt quá màn hình
            i--; // Giảm chỉ số i đi 1 sau khi xóa phần tử để tránh lỗi
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


void checkCollisions(SDL_Renderer* renderer) {
    for (int i = 0; i < bullets.size(); i++) {
        if (bullets[i].active) {
            for (int j = 0; j < enemies.size(); j++) {
                if (enemies[j].alive) {
                    if (isColliding(bullets[i], enemies[j])) {
                        // xu li dan
                        bullets[i].active = false;
                        enemies[j].alive = false;
                        score += 10; // Tăng đi?m khi trúng đ?ch
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

void drawPlayer(SDL_Renderer* renderer, GameObject player, SDL_Texture* playerTexture) {
    SDL_Rect playerRect = { player.x, player.y, player.w, player.h };
    SDL_RenderCopy(renderer, playerTexture, NULL, &playerRect);
}

void drawBullets(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // xanh

    for (int i = 0; i < bullets.size(); i++) {
        if (bullets[i].active) {
            SDL_Rect bulletRect = { bullets[i].x, bullets[i].y, bullets[i].w, bullets[i].h };
            SDL_RenderFillRect(renderer, &bulletRect);
        }
    }
}


void drawEnemies(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 122, 255, 255); // Hồng

    for (int i = 0; i < enemies.size(); i++) {
        if (enemies[i].alive) {
            SDL_Rect enemyRect = { enemies[i].x, enemies[i].y, enemies[i].w, enemies[i].h };
            SDL_RenderFillRect(renderer, &enemyRect);
        }
    }
}


void drawScore(SDL_Renderer* renderer, TTF_Font* font, int score) {
    SDL_Color textColor = { 255, 255, 255 }; // Màu trắng
    std::string score_str = "Score: " + to_string(score);
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, score_str.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    int textWidth, textHeight;
    SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);
    SDL_Rect textRect = { 10, 10, textWidth, textHeight }; // Vị trí để hiển thị điểm số

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

