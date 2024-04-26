#define SDL_MAIN_HANDLED
#include "Globals.h"
#include "GameFlow.h"

#include <filesystem>
#include <iostream>

using namespace std;

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 600;
const int PLAYER_SPEED = 10;
const int BULLET_SPEED = 5;
const int ENEMY_SPEED = 4;
const int ENEMY_SPAWN_TIMER = 1000;

GameObject player = { SCREEN_WIDTH / 2, SCREEN_HEIGHT - 50, 80, 80 };

vector<Bullet> bullets;
vector<Enemy> enemies;
int score = 0;
Uint32 lastEnemySpawn = 0;
SDL_Texture* heartTexture = nullptr;

void initializeSDL(SDL_Window*& window, SDL_Renderer*& renderer, TTF_Font*& font, SDL_Texture*& backgroundTexture, SDL_Texture*& playerTexture);

int main(int argc, char* argv[]) {
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    SDL_Texture* playerTexture;
    SDL_Texture* backgroundTexture;
    const string backgroundImagePath = "resources\\bkgd_menu.png"; //fisrt_bkgd

    initializeSDL(window, renderer, font, backgroundTexture, playerTexture);

    bool inMenu = true;
    SDL_Event event;

    while (inMenu) {
        drawMenu(renderer, font, window, backgroundImagePath, playerTexture, player, lastEnemySpawn, bullets, enemies, score);
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                inMenu = false;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                if (mouseX >= 400 && mouseX <= 600 && mouseY >= 200 && mouseY <= 250) {
                   
                    bool isRunning = true;

                    
                    countdownTimer(renderer, font);

                    runGameLoop(renderer, window, font, backgroundTexture, playerTexture,
                        player, lastEnemySpawn, bullets, enemies, score, isRunning);
                    inMenu = false; 
                }
                else if (mouseX >= 400 && mouseX <= 600 && mouseY >= 300 && mouseY <= 350) {
                    
                    inMenu = false; 
                }
            }
        }
    }

    cleanup(window, renderer, font);

    return 0;
}

void initializeSDL(SDL_Window*& window, SDL_Renderer*& renderer, TTF_Font*& font, SDL_Texture*& backgroundTexture, SDL_Texture*& playerTexture) {
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("GUN BATTLE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    TTF_Init();
    font = TTF_OpenFont("resources\\OpenSans-ExtraBold.ttf", 25);



    srand(static_cast<unsigned int>(time(NULL)));

   
    SDL_Surface* backgroundSurface = IMG_Load("resources\\new_bkground.png");                // main_bkgd
    backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);

    SDL_Surface* playerSurface = IMG_Load("resources\\newbkground.png");                     //player_bkgd
    playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);

    std::cout << "Current path is " << std::filesystem::current_path() << '\n'; // (1)

    SDL_Surface* heartSurface = IMG_Load("resources\\heart.png");                               //heart
    if (heartSurface == nullptr) {
        cerr << "Failed to load heart image: " << IMG_GetError() << endl;
       
    }
    else {
        heartTexture = SDL_CreateTextureFromSurface(renderer, heartSurface);
        SDL_FreeSurface(heartSurface);
    }
}

