#include "GameLogic.h"
#include "Globals.h"
void handleInput(SDL_Renderer* renderer, GameObject& player, bool& isRunning) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_UP:
                player.y -= PLAYER_SPEED; 
                break;
            case SDLK_DOWN:
                player.y += PLAYER_SPEED; 
                break;
            case SDLK_LEFT:
                player.x -= PLAYER_SPEED; 
                break;
            case SDLK_RIGHT:
                player.x += PLAYER_SPEED; 
                break;
            }
            break;
        }
    }
}
void movePlayer(GameObject& player) {
    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    if (keystate[SDL_SCANCODE_UP] && player.y > 0) {
        player.y -= PLAYER_SPEED; 
    }
    if (keystate[SDL_SCANCODE_DOWN] && player.y < SCREEN_HEIGHT - player.h) {
        player.y += PLAYER_SPEED; 
    }
    if (keystate[SDL_SCANCODE_LEFT] && player.x > 0) {
        player.x -= PLAYER_SPEED; 
    }
    if (keystate[SDL_SCANCODE_RIGHT] && player.x < SCREEN_WIDTH - player.w) {
        player.x += PLAYER_SPEED; 
    }
}