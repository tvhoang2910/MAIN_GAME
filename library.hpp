#pragma once

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
const int PLAYER_SPEED = 100;
const int BULLET_SPEED = 50;
const int ENEMY_SPEED = 40;
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

extern GameObject player;

struct GameProperties {
    vector<Bullet> bullets;
    vector<Enemy> enemies;
    int score = 0;
    Uint32 lastEnemySpawn = 0;
    GameObject player = { SCREEN_WIDTH / 2, SCREEN_HEIGHT - 50, 80, 80 };


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

};