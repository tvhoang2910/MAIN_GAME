#include"D:\C++\Visual_studio\SDL_GAME_1\SDL_GAME_1\GameLogic.h"
#include"D:\C++\Visual_studio\SDL_GAME_1\SDL_GAME_1\Globals.h"
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
                player.y -= PLAYER_SPEED; // Di chuy?n l�n khi nh?n ph�m m?i t�n l�n
                break;
            case SDLK_DOWN:
                player.y += PLAYER_SPEED; // Di chuy?n xu?ng khi nh?n ph�m m?i t�n xu?ng
                break;
            case SDLK_LEFT:
                player.x -= PLAYER_SPEED; // Di chuy?n sang tr�i khi nh?n ph�m m?i t�n tr�i
                break;
            case SDLK_RIGHT:
                player.x += PLAYER_SPEED; // Di chuy?n sang ph?i khi nh?n ph�m m?i t�n ph?i
                break;
            }
            break;
        }
    }
}
void movePlayer(GameObject& player) {
    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    if (keystate[SDL_SCANCODE_UP] && player.y > 0) {
        player.y -= PLAYER_SPEED; // Di chuy?n l�n khi nh?n ph�m m?i t�n l�n
    }
    if (keystate[SDL_SCANCODE_DOWN] && player.y < SCREEN_HEIGHT - player.h) {
        player.y += PLAYER_SPEED; // Di chuy?n xu?ng khi nh?n ph�m m?i t�n xu?ng
    }
    if (keystate[SDL_SCANCODE_LEFT] && player.x > 0) {
        player.x -= PLAYER_SPEED; // Di chuy?n sang tr�i khi nh?n ph�m m?i t�n tr�i
    }
    if (keystate[SDL_SCANCODE_RIGHT] && player.x < SCREEN_WIDTH - player.w) {
        player.x += PLAYER_SPEED; // Di chuy?n sang ph?i khi nh?n ph�m m?i t�n ph?i
    }
}