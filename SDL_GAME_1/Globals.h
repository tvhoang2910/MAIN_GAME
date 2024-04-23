
#ifndef GLOBALS_H
#define GLOBALS_H
#include"D:\C++\Visual_studio\SDL_GAME_1\GameObject.h"
#include <iostream>
#include <SDL.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <SDL_ttf.h>
#include <string>
#include <SDL_image.h>

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int PLAYER_SPEED;
extern const int BULLET_SPEED;
extern const int ENEMY_SPEED;
extern const int ENEMY_SPAWN_TIMER;
extern int score;
extern Uint32 lastEnemySpawn;
extern SDL_Texture* heartTexture;
extern GameObject player;
extern std::vector<Bullet> bullets; 
extern std::vector<Enemy> enemies;
#endif 
