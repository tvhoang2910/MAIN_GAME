#include "GameRendering.h"
#include "Globals.h"

using namespace std;
void drawPlayer(SDL_Renderer* renderer, GameObject player, SDL_Texture* playerTexture) {
    SDL_Rect playerRect = { player.x, player.y, player.w, player.h };
    SDL_RenderCopy(renderer, playerTexture, NULL, &playerRect);
}

void drawBullets(SDL_Renderer* renderer) {
    
    SDL_Surface* bulletSurface = IMG_Load("resources\\bullet_bkground.png");
    SDL_Texture* bulletTexture = SDL_CreateTextureFromSurface(renderer, bulletSurface);
    SDL_FreeSurface(bulletSurface);

    for (int i = 0; i < bullets.size(); i++) {
        if (bullets[i].active) {
            
            SDL_Rect bulletRect = { bullets[i].x, bullets[i].y, bullets[i].w, bullets[i].h };
            SDL_RenderCopy(renderer, bulletTexture, NULL, &bulletRect);
        }
    }

    SDL_DestroyTexture(bulletTexture); 
}



void drawEnemies(SDL_Renderer* renderer) {
    
    SDL_Surface* enemySurface = IMG_Load("resources\\Enemies bkgrond.png");
    SDL_Texture* enemyTexture = SDL_CreateTextureFromSurface(renderer, enemySurface);
    SDL_FreeSurface(enemySurface);

    for (int i = 0; i < enemies.size(); i++) {
        if (enemies[i].alive) {
            
            SDL_Rect enemyRect = { enemies[i].x, enemies[i].y, enemies[i].w, enemies[i].h };
            SDL_RenderCopy(renderer, enemyTexture, NULL, &enemyRect);
        }
    }

    SDL_DestroyTexture(enemyTexture); 
}


void drawScore(SDL_Renderer* renderer, TTF_Font* font, int score) {
    SDL_Color textColor = { 255, 255, 0 }; 
    string score_str = "Score: " + to_string(score);
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, score_str.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    int textWidth, textHeight;
    SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);
    SDL_Rect textRect = { 10, 10, textWidth, textHeight }; 

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}
void drawHearts(SDL_Renderer* renderer, int numHeartsRemaining) {
    
    int heartWidth = 50;
    int heartHeight = 50;
    int heartMargin = 5; 

    
    int startX = SCREEN_WIDTH - (numHeartsRemaining * (heartWidth + heartMargin)) - 10;
    int startY = 10;
    for (int i = 0; i < numHeartsRemaining; i++) {
        SDL_Rect heartRect = { startX + i * (heartWidth + heartMargin), startY, heartWidth, heartHeight };
        SDL_RenderCopy(renderer, heartTexture, NULL, &heartRect);
    }
}