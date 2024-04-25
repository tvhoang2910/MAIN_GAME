
#ifndef GAMERENDERING_H
#define GAMERENDERING_H

#include <SDL.h>
#include <vector>
#include <string>
#include <SDL_ttf.h>
#include "GameObject.h"


void drawPlayer(SDL_Renderer* renderer, GameObject player, SDL_Texture* playerTexture);
void drawBullets(SDL_Renderer* renderer);
void drawEnemies(SDL_Renderer* renderer);
void drawScore(SDL_Renderer* renderer, TTF_Font* font, int score);
void drawHearts(SDL_Renderer* renderer, int numHeartsRemaining);

#endif 