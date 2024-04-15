#include "library.hpp"

//vector<Bullet> bullets;
//vector<Enemy> enemies;
//int score = 0;
//Uint32 lastEnemySpawn = 0;
//
//GameObject player = { SCREEN_WIDTH / 2, SCREEN_HEIGHT - 50, 80, 80 };

void GameProperties::GameProperties::drawMenu(SDL_Renderer* renderer, TTF_Font* font, SDL_Window* window, const string& backgroundImagePath, SDL_Texture* playerTexture,
    GameObject& player, Uint32& lastEnemySpawn, vector<Bullet>& bullets, vector<Enemy>& enemies, int& score) {
    SDL_Surface* backgroundSurface = IMG_Load(backgroundImagePath.c_str());
    if (backgroundSurface == nullptr) {
        // Xử lý khi không thể tải hình ảnh
        cerr << "Failed to load background image: " << IMG_GetError() << endl;

    }
    else {
        SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
        SDL_FreeSurface(backgroundSurface);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Vẽ background
        SDL_Rect backgroundRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

        // Vẽ menu
        SDL_Color textColor = { 255, 255, 255 }; // Màu trắng

        // init "Start"
        TTF_SetFontStyle(font, TTF_STYLE_BOLD); // Đặt kiểu chữ in đậm
        TTF_SetFontSize(font, 50); //  kích thước của chữ "start" với "exit"
        SDL_Surface* startSurface = TTF_RenderText_Blended(font, "Start", textColor);
        SDL_Texture* startTexture = SDL_CreateTextureFromSurface(renderer, startSurface);
        SDL_Rect startRect = { (SCREEN_WIDTH - startSurface->w) / 2, 200, startSurface->w, startSurface->h }; // Căn giữa theo chiều ngang
        SDL_RenderCopy(renderer, startTexture, NULL, &startRect);
        SDL_FreeSurface(startSurface);
        SDL_DestroyTexture(startTexture);

        // init "Exit"
        SDL_Surface* exitSurface = TTF_RenderText_Blended(font, "Exit", textColor);
        SDL_Texture* exitTexture = SDL_CreateTextureFromSurface(renderer, exitSurface);
        SDL_Rect exitRect = { (SCREEN_WIDTH - exitSurface->w) / 2, 300, exitSurface->w, exitSurface->h }; // Căn giữa theo chiều ngang
        SDL_RenderCopy(renderer, exitTexture, NULL, &exitRect);
        SDL_FreeSurface(exitSurface);
        SDL_DestroyTexture(exitTexture);

        SDL_RenderPresent(renderer);

        SDL_DestroyTexture(backgroundTexture);
    }
}





void GameProperties::GameProperties::initializeSDL(SDL_Window*& window, SDL_Renderer*& renderer, TTF_Font*& font, SDL_Texture*& backgroundTexture, SDL_Texture*& playerTexture) {
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("GUN BATTLE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    TTF_Init();
    font = TTF_OpenFont("D:/C++/Visual_studio/SDL_GAME_1/lazy.ttf", 25);

    srand(static_cast<unsigned int>(time(NULL)));

    // Load hình ảnh
    SDL_Surface* backgroundSurface = IMG_Load("D:\\C++\\Visual_studio\\SDL_GAME_1\\x64\\Debug\\new_bkground.png");
    backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);

    SDL_Surface* playerSurface = IMG_Load("D:\\C++\\Visual_studio\\SDL_GAME_1\\x64\\Debug\\newbkground.png");
    playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
}

void GameProperties::GameProperties::handleInput(SDL_Renderer* renderer, GameObject& player, bool& isRunning) {
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



void GameProperties::movePlayer(GameObject& player) {
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


void GameProperties::fireBullet() {
    static int bulletCooldown = 0;
    const int BULLET_COOLDOWN_TIME = 1; // Thời gian chờ giữa các lần bắn đạn

    if (bulletCooldown > 0) {
        bulletCooldown--;
    }

    if (bulletCooldown == 0) {
        Bullet bullet;
        bullet.x = player.x + player.w / 2 - 2;
        bullet.y = player.y;
        //kích thước của đạn
        bullet.w = 25;
        bullet.h = 25;

        bullet.active = true; // đạn đang hoạt động
        bullets.push_back(bullet);

        bulletCooldown = BULLET_COOLDOWN_TIME; // init thời gian chờ giữa các lần bắn đạn
    }
}


void GameProperties::spawnEnemy() {
    Enemy enemy;
    enemy.x = rand() % (SCREEN_WIDTH - 40); // Random dich xuat hien
    enemy.y = 0;
    enemy.w = 60; // width
    enemy.h = 60; // height
    enemies.push_back(enemy); // Thêm địch vào vector enemies
}

void GameProperties::moveBullets() {
    for (int i = 0; i < bullets.size(); i++) {
        if (bullets[i].active) {
            bullets[i].y -= BULLET_SPEED; // Di chuyển lên trên theo hướng y
            if (bullets[i].y < 0) {
                bullets[i].active = false; // out màn hình => mất
            }
        }
    }
}

void GameProperties::moveEnemies() {
    for (int i = 0; i < enemies.size(); i++) {
        enemies[i].y += ENEMY_SPEED; // Di chuyển địch xuống dưới theo hướng y
        if (enemies[i].y > SCREEN_HEIGHT) {
            enemies.erase(enemies.begin() + i); //out màn hình = > mất
            i--; // Giảm chỉ số i đi 1 sau khi xóa phần tử 
        }
    }
}
//check va chạm
bool isColliding(const GameObject& obj1, const GameObject& obj2) {
    if (obj1.x < obj2.x + obj2.w &&
        obj1.x + obj1.w > obj2.x &&
        obj1.y < obj2.y + obj2.h &&
        obj1.y + obj1.h > obj2.y) {
        return true;
    }
    return false;
}


void GameProperties::checkCollisions(SDL_Renderer* renderer) {
    for (int i = 0; i < bullets.size(); i++) {
        if (bullets[i].active) {
            for (int j = 0; j < enemies.size(); j++) {
                if (enemies[j].alive) {
                    if (isColliding(bullets[i], enemies[j])) {
                        // process đạn
                        bullets[i].active = false;
                        enemies[j].alive = false;
                        score += 10; // Tăng điểm khi trúng địch
                    }
                }
            }
        }
    }

    for (int i = 0; i < enemies.size(); i++) {
        if (enemies[i].alive && isColliding(player, enemies[i])) {

        }
    }
}

void GameProperties::drawPlayer(SDL_Renderer* renderer, GameObject player, SDL_Texture* playerTexture) {
    SDL_Rect playerRect = { player.x, player.y, player.w, player.h };
    SDL_RenderCopy(renderer, playerTexture, NULL, &playerRect);
}

void GameProperties::drawBullets(SDL_Renderer* renderer) {
    // Load hình ảnh mới cho đạn
    SDL_Surface* bulletSurface = IMG_Load("D:\\C++\\Visual_studio\\SDL_GAME_1\\x64\\Debug\\bullet_bkground.png");
    SDL_Texture* bulletTexture = SDL_CreateTextureFromSurface(renderer, bulletSurface);
    SDL_FreeSurface(bulletSurface);

    for (int i = 0; i < bullets.size(); i++) {
        if (bullets[i].active) {
            // Vẽ đạn sử dụng texture mới
            SDL_Rect bulletRect = { bullets[i].x, bullets[i].y, bullets[i].w, bullets[i].h };
            SDL_RenderCopy(renderer, bulletTexture, NULL, &bulletRect);
        }
    }

    SDL_DestroyTexture(bulletTexture); // Giải phóng texture 
}



void GameProperties::drawEnemies(SDL_Renderer* renderer) {
    // Load hình ảnh mới cho kẻ địch
    SDL_Surface* enemySurface = IMG_Load("D:\\C++\\Visual_studio\\SDL_GAME_1\\x64\\Debug\\Enemies bkgrond.png");
    SDL_Texture* enemyTexture = SDL_CreateTextureFromSurface(renderer, enemySurface);
    SDL_FreeSurface(enemySurface);

    for (int i = 0; i < enemies.size(); i++) {
        if (enemies[i].alive) {
            // Vẽ kẻ địch sử dụng texture mới
            SDL_Rect enemyRect = { enemies[i].x, enemies[i].y, enemies[i].w, enemies[i].h };
            SDL_RenderCopy(renderer, enemyTexture, NULL, &enemyRect);
        }
    }

    SDL_DestroyTexture(enemyTexture); // Giải phóng texture sau khi sử dụng
}


void GameProperties::drawScore(SDL_Renderer* renderer, TTF_Font* font, int score) {
    SDL_Color textColor = { 255, 255, 0 }; //yellow
    string score_str = "Score: " + to_string(score);
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, score_str.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    int textWidth, textHeight;
    SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);
    SDL_Rect textRect = { 10, 10, textWidth, textHeight }; // Vị trí để hiển thị điểm số

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}
void GameProperties::cleanup(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font) {
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
void GameProperties::runGameLoop(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font, SDL_Texture* backgroundTexture,
    SDL_Texture* playerTexture, GameObject& player, Uint32& lastEnemySpawn, vector<Bullet>& bullets, vector<Enemy>& enemies,
    int& score, bool& isRunning) {
    SDL_Event event;
    int numHeartsRemaining = 3; // Số lượng trái tim ban đầu

    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false; // Đặt biến isRunning thành false để kết thúc vòng lặp chính
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
        moveEnemies();
        checkCollisions(renderer);

        // Vẽ background
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        drawPlayer(renderer, player, playerTexture);
        drawBullets(renderer);
        drawEnemies(renderer);
        drawScore(renderer, font, score);

        // Vẽ số lượng trái tim còn lại
        drawHearts(renderer, numHeartsRemaining);

        SDL_RenderPresent(renderer);

        // Kiểm tra khi nào trò chơi kết thúc do hết trái tim
        if (numHeartsRemaining <= 0) {
            isRunning = false;
            // Hiển thị thông báo "Game Over" hoặc thực hiện các hành động khác khi trò chơi kết thúc
        }
    }

    // Dọn dẹp và thoát chương trình sau khi người dùng thoát
    cleanup(window, renderer, font);
}
void GameProperties::countdownTimer(SDL_Renderer* renderer, TTF_Font* font) {
    // Load hình ảnh background
    SDL_Surface* backgroundSurface = IMG_Load("D:\\C++\\Visual_studio\\SDL_GAME_1\\x64\\Debug\\Screenshot 2024-04-06 121850.png");
    if (backgroundSurface == nullptr) {
        // Xử lý khi không thể tải hình ảnh
        cerr << "Failed to load background image: " << IMG_GetError() << endl;
        return;
    }

    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);

    int countdown = 3; // Thời gian đếm ngược ban đầu
    Uint32 startTime = SDL_GetTicks(); // Thời gian bắt đầu đếm ngược

    while (countdown > 0) {
        // Xóa màn hình
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Vẽ background lên cửa sổ
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

        // Hiển thị thời gian đếm ngược lên màn hình
        SDL_Color textColor = { 255, 255, 255 }; // Màu trắng
        string countdownText = "Countdown: " + to_string(countdown);
        SDL_Surface* textSurface = TTF_RenderText_Blended(font, countdownText.c_str(), textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect textRect = { 10, 10, textSurface->w, textSurface->h }; // Vị trí để hiển thị thời gian đếm ngược
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        // Giải phóng bộ nhớ
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);

        // Hiển thị lên màn hình
        SDL_RenderPresent(renderer);

        // Tính thời gian đã trôi qua
        Uint32 currentTime = SDL_GetTicks();
        Uint32 elapsedTime = currentTime - startTime;

        // Cập nhật thời gian đếm ngược
        if (elapsedTime >= 1000) {
            countdown--;
            startTime = currentTime;
        }
    }

    // Giải phóng texture của background
    SDL_DestroyTexture(backgroundTexture);
}
void GameProperties::drawHearts(SDL_Renderer* renderer, int numHeartsRemaining) {
    // Load hình ảnh heart
    SDL_Surface* heartSurface = IMG_Load("D:\\C++\\Visual_studio\\SDL_GAME_1\\x64\\Debug\\heart.png");
    if (heartSurface == nullptr) {
        // Xử lý khi không thể tải hình ảnh
        cerr << "Failed to load heart image: " << IMG_GetError() << endl;
        return;
    }

    SDL_Texture* heartTexture = SDL_CreateTextureFromSurface(renderer, heartSurface);
    SDL_FreeSurface(heartSurface);

    // Vị trí và kích thước của heart
    int heartWidth = 50;
    int heartHeight = 50;
    int heartMargin = 5; // Khoảng cách giữa các heart

    // Vẽ số lượng trái tim còn lại
    int startX = SCREEN_WIDTH - (numHeartsRemaining * (heartWidth + heartMargin)) - 10; // 10 là khoảng cách từ mép phải
    int startY = 10; // 10 là khoảng cách từ mép trên
    for (int i = 0; i < numHeartsRemaining; i++) {
        SDL_Rect heartRect = { startX + i * (heartWidth + heartMargin), startY, heartWidth, heartHeight };
        SDL_RenderCopy(renderer, heartTexture, NULL, &heartRect);
    }

    SDL_DestroyTexture(heartTexture);
}
