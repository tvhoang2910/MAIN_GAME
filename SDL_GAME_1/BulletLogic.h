
#ifndef BULLETLOGIC_H
#define BULLETLOGIC_H

#include <SDL.h>
#include"GameObject.h"

void fireBullet();
void moveBullets();
bool isColliding(const GameObject& obj1, const GameObject& obj2);

#endif
