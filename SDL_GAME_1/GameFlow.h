
#ifndef GAMEFLOW_H
#define GAMEFLOW_H

#include <SDL.h>
#include <vector>
#include <string>
#include <SDL_ttf.h>
#include "GameObject.h"

using namespace std;
void cleanup(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font);
void runGameLoop(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font, SDL_Texture* backgroundTexture, SDL_Texture* playerTexture,
    GameObject& player, Uint32& lastEnemySpawn, vector<Bullet>& bullets, vector<Enemy>& enemies, int& score, bool& isRunning);
void drawMenu(SDL_Renderer* renderer, TTF_Font* font, SDL_Window* window, const string& backgroundImagePath, SDL_Texture* playerTexture,
    GameObject& player, Uint32& lastEnemySpawn, vector<Bullet>& bullets, vector<Enemy>& enemies, int& score);
void countdownTimer(SDL_Renderer* renderer, TTF_Font* font);
void drawEndGameMenu(SDL_Renderer* renderer, TTF_Font* font, bool& isRunning, GameObject& player, Uint32& lastEnemySpawn, vector<Bullet>& bullets, vector<Enemy>& enemies, int& score);

#endif