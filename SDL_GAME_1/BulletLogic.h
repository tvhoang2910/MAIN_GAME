
#ifndef BULLETLOGIC_H
#define BULLETLOGIC_H

#include <SDL.h>
#include"D:\C++\Visual_studio\SDL_GAME_1\GameObject.h"

void fireBullet();
void moveBullets();
bool isColliding(const GameObject& obj1, const GameObject& obj2);

#endif
