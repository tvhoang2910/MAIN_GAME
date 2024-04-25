#ifndef ENEMYLOGIC_H
#define ENEMYLOGIC_H

#include <SDL.h>
#include <vector>

#include "GameObject.h"


void spawnEnemy();
void moveEnemies();
void checkCollisions(SDL_Renderer* renderer);
void moveEnemiesAndCheckHearts(SDL_Renderer* renderer, int& numHeartsRemaining, bool& isRunning, int& score);

#endif