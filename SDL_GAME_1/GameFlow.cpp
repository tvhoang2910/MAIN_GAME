#include"D:\C++\Visual_studio\SDL_GAME_1\SDL_GAME_1\GameFlow.h"

#include"D:\C++\Visual_studio\SDL_GAME_1\SDL_GAME_1\GameLogic.h"
#include"D:\C++\Visual_studio\SDL_GAME_1\SDL_GAME_1\Globals.h"
#include"D:\C++\Visual_studio\SDL_GAME_1\SDL_GAME_1\BulletLogic.h"
#include"D:\C++\Visual_studio\SDL_GAME_1\SDL_GAME_1\EnemyLogic.h"
#include"D:\C++\Visual_studio\SDL_GAME_1\SDL_GAME_1\GameRendering.h"
void cleanup(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font) {
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void runGameLoop(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font, SDL_Texture* backgroundTexture,
    SDL_Texture* playerTexture, GameObject& player, Uint32& lastEnemySpawn, vector<Bullet>& bullets, vector<Enemy>& enemies,
    int& score, bool& isRunning) {
    SDL_Event event;
    int numHeartsRemaining = 3; // S? lý?ng trái tim ban ð?u

    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false; // Ð?t bi?n isRunning thành false ð? k?t thúc v?ng l?p chính
            }
        }

        handleInput(renderer, player, isRunning);
        movePlayer(player);
        fireBullet();

        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastEnemySpawn >= ENEMY_SPAWN_TIMER) {
            spawnEnemy();
            lastEnemySpawn = currentTime;
        }

        moveBullets();
        moveEnemiesAndCheckHearts(renderer, numHeartsRemaining, isRunning, score);// Di chuy?n enemy và ki?m tra trái tim

        checkCollisions(renderer);

        // V? background
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        drawPlayer(renderer, player, playerTexture);
        drawBullets(renderer);
        drawEnemies(renderer);
        drawScore(renderer, font, score);
        drawHearts(renderer, numHeartsRemaining); // V? s? lý?ng trái tim c?n l?i

        SDL_RenderPresent(renderer);

        if (numHeartsRemaining <= 0) {
            isRunning = false; // K?t thúc tr? chõi khi h?t trái tim
            // Hi?n th? thông báo "Game Over" ho?c th?c hi?n các hành ð?ng khác khi tr? chõi k?t thúc
            SDL_Color textColor = { 0, 255, 0 };
            // Màu green
            TTF_SetFontSize(font, 50); // Ð?t kích thý?c ch? cho thông báo "Game Over"
            SDL_Surface* gameOverSurface = TTF_RenderText_Blended(font, "Game Over!", textColor);
            SDL_Texture* gameOverTexture = SDL_CreateTextureFromSurface(renderer, gameOverSurface);
            int textWidth, textHeight;
            SDL_QueryTexture(gameOverTexture, NULL, NULL, &textWidth, &textHeight);
            SDL_Rect textRect = { (SCREEN_WIDTH - textWidth) / 2, (SCREEN_HEIGHT - textHeight) / 2, textWidth, textHeight }; // V? trí ð? hi?n th? thông báo "Game Over"
            SDL_RenderCopy(renderer, gameOverTexture, NULL, &textRect);
            SDL_FreeSurface(gameOverSurface);
            SDL_DestroyTexture(gameOverTexture);
            SDL_RenderPresent(renderer);

            SDL_Delay(3000); // D?ng chýõng tr?nh trong 3 giây trý?c khi thoát
            drawEndGameMenu(renderer, font, isRunning, player, lastEnemySpawn, bullets, enemies, score);
            numHeartsRemaining = 3;
        }
        if (score >= 200) {
            isRunning = false; // K?t thúc tr? chõi khi ð?t ði?u ki?n chi?n th?ng
            // Hi?n th? thông báo "You Win" ho?c th?c hi?n các hành ð?ng khác khi tr? chõi k?t thúc
            SDL_Color textColor = { 0, 255, 0 }; // Màu xanh lá cây
            TTF_SetFontSize(font, 50); // Ð?t kích thý?c ch? cho thông báo "You Win"
            SDL_Surface* youWinSurface = TTF_RenderText_Blended(font, "You Win", textColor);
            SDL_Texture* youWinTexture = SDL_CreateTextureFromSurface(renderer, youWinSurface);
            int textWidth, textHeight;
            SDL_QueryTexture(youWinTexture, NULL, NULL, &textWidth, &textHeight);
            SDL_Rect textRect = { (SCREEN_WIDTH - textWidth) / 2, (SCREEN_HEIGHT - textHeight) / 2, textWidth, textHeight }; // V? trí ð? hi?n th? thông báo "You Win"
            SDL_RenderCopy(renderer, youWinTexture, NULL, &textRect);
            SDL_FreeSurface(youWinSurface);
            SDL_DestroyTexture(youWinTexture);
            SDL_RenderPresent(renderer);

            SDL_Delay(3000); // D?ng chýõng tr?nh trong 3 giây trý?c khi thoát
            drawEndGameMenu(renderer, font, isRunning, player, lastEnemySpawn, bullets, enemies, score);
            numHeartsRemaining = 3;
        }

    }

    // D?n d?p và thoát chýõng tr?nh sau khi ngý?i dùng thoát
    cleanup(window, renderer, font);
}

void countdownTimer(SDL_Renderer* renderer, TTF_Font* font) {
    // Load h?nh ?nh background
    SDL_Surface* backgroundSurface = IMG_Load("D:\\C++\\Visual_studio\\SDL_GAME_1\\x64\\Debug\\Screenshot 2024-04-06 121850.png");
    if (backgroundSurface == nullptr) {
        // X? l? khi không th? t?i h?nh ?nh
        cerr << "Failed to load background image: " << IMG_GetError() << endl;
        return;
    }

    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);

    int countdown = 10; // Th?i gian ð?m ngý?c ban ð?u
    Uint32 startTime = SDL_GetTicks(); // Th?i gian b?t ð?u ð?m ngý?c
    bool welcomeDisplayed = false;
    bool ruleDisplayed = false;
    bool heartLossDisplayed = false;
    while (countdown > 0) {
        // Xóa màn h?nh
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // V? background lên c?a s?
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

        // Hi?n th? th?i gian ð?m ngý?c lên màn h?nh
        SDL_Color textColor = { 255, 255, 255 }; // Màu tr?ng
        string countdownText = "Countdown: " + to_string(countdown);
        SDL_Surface* textSurface = TTF_RenderText_Blended(font, countdownText.c_str(), textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect textRect = { 10, 10, textSurface->w, textSurface->h }; // V? trí ð? hi?n th? th?i gian ð?m ngý?c
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);



        // Hi?n th? thông báo "Welcome to Gun Battle!"
        TTF_SetFontSize(font, 35);
        SDL_Surface* welcomeSurface = TTF_RenderText_Blended(font, "Welcome to Gun Battle!", textColor);
        SDL_Texture* welcomeTexture = SDL_CreateTextureFromSurface(renderer, welcomeSurface);

        int welcomeTextWidth, welcomeTextHeight;
        SDL_QueryTexture(welcomeTexture, NULL, NULL, &welcomeTextWidth, &welcomeTextHeight);
        SDL_Rect welcomeRect = { (SCREEN_WIDTH - welcomeTextWidth) / 2, (SCREEN_HEIGHT - welcomeTextHeight) / 2 - 100, welcomeTextWidth, welcomeTextHeight };
        SDL_RenderCopy(renderer, welcomeTexture, NULL, &welcomeRect);
        SDL_FreeSurface(welcomeSurface);
        SDL_DestroyTexture(welcomeTexture);

        // Hi?n th? lu?t chõi "Get 200 points and you win"
        TTF_SetFontSize(font, 35);
        SDL_Surface* ruleSurface = TTF_RenderText_Blended(font, "Reach 200 points to emerge victorious!", textColor);
        SDL_Texture* ruleTexture = SDL_CreateTextureFromSurface(renderer, ruleSurface);

        int ruleTextWidth, ruleTextHeight;
        SDL_QueryTexture(ruleTexture, NULL, NULL, &ruleTextWidth, &ruleTextHeight);
        SDL_Rect ruleRect = { (SCREEN_WIDTH - ruleTextWidth) / 2, (SCREEN_HEIGHT - ruleTextHeight) / 2 + 75 - 100 , ruleTextWidth, ruleTextHeight };
        SDL_RenderCopy(renderer, ruleTexture, NULL, &ruleRect);
        SDL_FreeSurface(ruleSurface);
        SDL_DestroyTexture(ruleTexture);

        // Hi?n th? thông báo "N?u m?t 3 trái tim th? game over!"
        TTF_SetFontSize(font, 35);
        SDL_Surface* heartLossSurface = TTF_RenderText_Blended(font, "Lose 3 hearts and face defeat!", textColor);
        SDL_Texture* heartLossTexture = SDL_CreateTextureFromSurface(renderer, heartLossSurface);

        int heartLossTextWidth, heartLossTextHeight;
        SDL_QueryTexture(heartLossTexture, NULL, NULL, &heartLossTextWidth, &heartLossTextHeight);
        SDL_Rect heartLossRect = { (SCREEN_WIDTH - heartLossTextWidth) / 2, (SCREEN_HEIGHT - heartLossTextHeight) / 2 + 150 - 100, heartLossTextWidth, heartLossTextHeight };
        SDL_RenderCopy(renderer, heartLossTexture, NULL, &heartLossRect);
        SDL_FreeSurface(heartLossSurface);
        SDL_DestroyTexture(heartLossTexture);

        TTF_SetFontSize(font, 35);
        SDL_Surface* moveInstructionSurface = TTF_RenderText_Blended(font, "Use the arrow keys to navigate your path.", textColor);
        SDL_Texture* moveInstructionTexture = SDL_CreateTextureFromSurface(renderer, moveInstructionSurface);

        int moveInstructionTextWidth, moveInstructionTextHeight;
        SDL_QueryTexture(moveInstructionTexture, NULL, NULL, &moveInstructionTextWidth, &moveInstructionTextHeight);
        SDL_Rect moveInstructionRect = { (SCREEN_WIDTH - moveInstructionTextWidth) / 2, (SCREEN_HEIGHT - moveInstructionTextHeight) / 2 + 225 - 100, moveInstructionTextWidth, moveInstructionTextHeight };
        SDL_RenderCopy(renderer, moveInstructionTexture, NULL, &moveInstructionRect);
        SDL_FreeSurface(moveInstructionSurface);
        SDL_DestroyTexture(moveInstructionTexture);

        SDL_RenderPresent(renderer);

        // Tính th?i gian ð? trôi qua
        Uint32 currentTime = SDL_GetTicks();
        Uint32 elapsedTime = currentTime - startTime;

        // C?p nh?t th?i gian ð?m ngý?c
        if (elapsedTime >= 1000) {
            countdown--;
            startTime = currentTime;
        }
    }



    // Gi?i phóng texture c?a background
    SDL_DestroyTexture(backgroundTexture);
}
void drawMenu(SDL_Renderer* renderer, TTF_Font* font, SDL_Window* window, const string& backgroundImagePath, SDL_Texture* playerTexture,
    GameObject& player, Uint32& lastEnemySpawn, vector<Bullet>& bullets, vector<Enemy>& enemies, int& score) {
    SDL_Surface* backgroundSurface = IMG_Load(backgroundImagePath.c_str());
    if (backgroundSurface == nullptr) {
        // X? l? khi không th? t?i h?nh ?nh
        cerr << "Failed to load background image: " << IMG_GetError() << endl;

    }
    else {
        SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
        SDL_FreeSurface(backgroundSurface);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // V? background
        SDL_Rect backgroundRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

        // V? menu
        SDL_Color textColor = { 255, 255, 255 }; // Màu tr?ng

        // init "Start"
        TTF_SetFontStyle(font, TTF_STYLE_BOLD); // Ð?t ki?u ch? in ð?m
        TTF_SetFontSize(font, 50); //  kích thý?c c?a ch? "start" v?i "exit"
        SDL_Surface* startSurface = TTF_RenderText_Blended(font, "Start", textColor);
        SDL_Texture* startTexture = SDL_CreateTextureFromSurface(renderer, startSurface);
        SDL_Rect startRect = { (SCREEN_WIDTH - startSurface->w) / 2, 200, startSurface->w, startSurface->h }; // Cãn gi?a theo chi?u ngang
        SDL_RenderCopy(renderer, startTexture, NULL, &startRect);
        SDL_FreeSurface(startSurface);
        SDL_DestroyTexture(startTexture);

        // init "Exit"
        SDL_Surface* exitSurface = TTF_RenderText_Blended(font, "Exit", textColor);
        SDL_Texture* exitTexture = SDL_CreateTextureFromSurface(renderer, exitSurface);
        SDL_Rect exitRect = { (SCREEN_WIDTH - exitSurface->w) / 2, 300, exitSurface->w, exitSurface->h }; // Cãn gi?a theo chi?u ngang
        SDL_RenderCopy(renderer, exitTexture, NULL, &exitRect);
        SDL_FreeSurface(exitSurface);
        SDL_DestroyTexture(exitTexture);

        SDL_RenderPresent(renderer);

        SDL_DestroyTexture(backgroundTexture);
    }
}

void resetGame(GameObject& player, Uint32& lastEnemySpawn, vector<Bullet>& bullets, vector<Enemy>& enemies, int& score, int& numHeartsRemaining) {
    player.x = SCREEN_WIDTH / 2;
    player.y = SCREEN_HEIGHT - 50;
    lastEnemySpawn = 0;
    bullets.clear();
    enemies.clear();
    score = 0;
    numHeartsRemaining = 3; // Thi?t l?p s? lý?ng trái tim ban ð?u
}
void drawEndGameMenu(SDL_Renderer* renderer, TTF_Font* font, bool& isRunning, GameObject& player, Uint32& lastEnemySpawn, vector<Bullet>& bullets, vector<Enemy>& enemies, int& score) {
    bool inEndGameMenu = true;
    SDL_Event event;
    int numHeartsRemaining;
    const string backgroundImagePath = "D:\\C++\\Visual_studio\\SDL_GAME_1\\x64\\Debug\\bkground.bmp";

    SDL_Surface* backgroundSurface = IMG_Load(backgroundImagePath.c_str());
    if (backgroundSurface == nullptr) {
        cerr << "Failed to load background image: " << IMG_GetError() << endl;
        return;
    }

    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);

    while (inEndGameMenu) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                inEndGameMenu = false;
                isRunning = false;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                if (mouseX >= 400 && mouseX <= 600 && mouseY >= 200 && mouseY <= 250) {
                    // Ch?n "Play again" - Reset tr?ng thái c?a tr? chõi và b?t ð?u m?t ván m?i
                    inEndGameMenu = false;
                    resetGame(player, lastEnemySpawn, bullets, enemies, score, numHeartsRemaining);
                    isRunning = true;
                }
                else if (mouseX >= 400 && mouseX <= 600 && mouseY >= 300 && mouseY <= 350) {
                    // Ch?n "Exit" - D?ng tr? chõi
                    inEndGameMenu = false;
                    isRunning = false;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_Rect backgroundRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

        // V? "Play again"
        SDL_Color textColor = { 255, 255, 255 }; // Màu tr?ng
        TTF_SetFontSize(font, 50);
        SDL_Surface* playAgainSurface = TTF_RenderText_Blended(font, "Play again", textColor);
        SDL_Texture* playAgainTexture = SDL_CreateTextureFromSurface(renderer, playAgainSurface);
        SDL_Rect playAgainRect = { (SCREEN_WIDTH - playAgainSurface->w) / 2, 200, playAgainSurface->w, playAgainSurface->h };
        SDL_RenderCopy(renderer, playAgainTexture, NULL, &playAgainRect);
        SDL_FreeSurface(playAgainSurface);
        SDL_DestroyTexture(playAgainTexture);

        // V? "Exit"
        SDL_Surface* exitSurface = TTF_RenderText_Blended(font, "Exit", textColor);
        SDL_Texture* exitTexture = SDL_CreateTextureFromSurface(renderer, exitSurface);
        SDL_Rect exitRect = { (SCREEN_WIDTH - exitSurface->w) / 2, 300, exitSurface->w, exitSurface->h };
        SDL_RenderCopy(renderer, exitTexture, NULL, &exitRect);
        SDL_FreeSurface(exitSurface);
        SDL_DestroyTexture(exitTexture);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(backgroundTexture);
}
