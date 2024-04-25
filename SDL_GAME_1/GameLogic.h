#ifndef PLAYERLOGIC_H
#define PLAYERLOGIC_H

#include <SDL.h>
#include "GameObject.h"


void handleInput(SDL_Renderer* renderer, GameObject& player, bool& isRunning);
void movePlayer(GameObject& player);

#endif 