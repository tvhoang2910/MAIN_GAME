
#include <iostream>
#include <SDL.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <SDL_ttf.h>
#include <string>
#include<SDL_image.h>
using namespace std;
#ifndef lib_h
#define lib_h
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int PLAYER_SPEED;
extern const int BULLET_SPEED;
extern const int ENEMY_SPEED;
extern const int ENEMY_SPAWN_TIMER;

struct GameObject {
    int x, y, w, h;
};

struct Bullet : public GameObject {
    bool active;
};

struct Enemy : public GameObject {
    bool alive;
};
extern GameObject player;

extern vector<Bullet> bullets;
extern vector<Enemy> enemies;
extern int score;
extern Uint32 lastEnemySpawn;
extern SDL_Texture* heartTexture;


void handleInput(SDL_Renderer* renderer, GameObject& player, bool& isRunning);
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
void initializeSDL(SDL_Window*& window, SDL_Renderer*& renderer, TTF_Font*& font, SDL_Texture*& backgroundTexture, SDL_Texture*& playerTexture);
void cleanup(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font);
void runGameLoop(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font, SDL_Texture* backgroundTexture, SDL_Texture* playerTexture,
    GameObject& player, Uint32& lastEnemySpawn, vector<Bullet>& bullets, vector<Enemy>& enemies, int& score, bool& isRunning);
void drawMenu(SDL_Renderer* renderer, TTF_Font* font, SDL_Window* window, const string& backgroundImagePath, SDL_Texture* playerTexture,
    GameObject& player, Uint32& lastEnemySpawn, vector<Bullet>& bullets, vector<Enemy>& enemies, int& score);
void countdownTimer(SDL_Renderer* renderer, TTF_Font* font);
void drawHearts(SDL_Renderer* renderer, int numHeartsRemaining);
void moveEnemiesAndCheckHearts(SDL_Renderer* renderer, int& numHeartsRemaining, bool& isRunning, int& score);
void drawEndGameMenu(SDL_Renderer* renderer, TTF_Font* font);
#endif lib_h
