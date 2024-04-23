#include"D:\C++\Visual_studio\SDL_GAME_1\SDL_GAME_1\PlayerLogic.h"

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
                player.y -= PLAYER_SPEED; // Di chuyển lên khi nhấn phím mũi tên lên
                break;
            case SDLK_DOWN:
                player.y += PLAYER_SPEED; // Di chuyển xuống khi nhấn phím mũi tên xuống
                break;
            case SDLK_LEFT:
                player.x -= PLAYER_SPEED; // Di chuyển sang trái khi nhấn phím mũi tên trái
                break;
            case SDLK_RIGHT:
                player.x += PLAYER_SPEED; // Di chuyển sang phải khi nhấn phím mũi tên phải
                break;
            }
            break;
        }
    }
}
void movePlayer(GameObject& player) {
    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    if (keystate[SDL_SCANCODE_UP] && player.y > 0) {
        player.y -= PLAYER_SPEED; // Di chuyển lên khi nhấn phím mũi tên lên
    }
    if (keystate[SDL_SCANCODE_DOWN] && player.y < SCREEN_HEIGHT - player.h) {
        player.y += PLAYER_SPEED; // Di chuyển xuống khi nhấn phím mũi tên xuống
    }
    if (keystate[SDL_SCANCODE_LEFT] && player.x > 0) {
        player.x -= PLAYER_SPEED; // Di chuyển sang trái khi nhấn phím mũi tên trái
    }
    if (keystate[SDL_SCANCODE_RIGHT] && player.x < SCREEN_WIDTH - player.w) {
        player.x += PLAYER_SPEED; // Di chuyển sang phải khi nhấn phím mũi tên phải
    }
}