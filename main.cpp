#define SDL_MAIN_HANDLED
#include <iostream>
#include <SDL.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <SDL_ttf.h>
#include <string>
#include<SDL_image.h>


using namespace std;

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 600;
const int PLAYER_SPEED = 10;
const int BULLET_SPEED = 5;
const int ENEMY_SPEED = 4;
const int ENEMY_SPAWN_TIMER = 1000;

struct GameObject {
    int x, y, w, h;
};

struct Bullet : public GameObject {
    bool active;
};

struct Enemy : public GameObject {
    bool alive;
};
GameObject player = { SCREEN_WIDTH / 2, SCREEN_HEIGHT - 50, 80, 80 };

vector<Bullet> bullets;
vector<Enemy> enemies;
int score = 0;
Uint32 lastEnemySpawn = 0;
SDL_Texture* heartTexture = nullptr;
void handleInput(SDL_Renderer* renderer, GameObject& player, bool& isRunning);
void movePlayer(GameObject& player);
void fireBullet();
void spawnEnemy();
void moveBullets();
void moveEnemies();
void checkCollisions(SDL_Renderer* renderer);
void drawPlayer(SDL_Renderer* renderer, GameObject player, SDL_Texture* playerTexture);
void drawBullets(SDL_Renderer* renderer);
void drawEnemies(SDL_Renderer* renderer);
void drawScore(SDL_Renderer* renderer, TTF_Font* font, int score);
void initializeSDL(SDL_Window*& window, SDL_Renderer*& renderer, TTF_Font*& font, SDL_Texture*& backgroundTexture, SDL_Texture*& playerTexture);
void cleanup(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font);
void runGameLoop(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font, SDL_Texture* backgroundTexture, SDL_Texture* playerTexture,
    GameObject& player, Uint32& lastEnemySpawn, vector<Bullet>& bullets, vector<Enemy>& enemies, int& score, bool& isRunning);
void drawMenu(SDL_Renderer* renderer, TTF_Font* font, SDL_Window* window, const string& backgroundImagePath, SDL_Texture* playerTexture,
    GameObject& player, Uint32& lastEnemySpawn, vector<Bullet>& bullets, vector<Enemy>& enemies, int& score);
void countdownTimer(SDL_Renderer* renderer, TTF_Font* font);
void drawHearts(SDL_Renderer* renderer, int numHeartsRemaining);
void moveEnemiesAndCheckHearts(SDL_Renderer* renderer, int& numHeartsRemaining, bool& isRunning, int& score);
void resetGame(GameObject& player, Uint32& lastEnemySpawn, vector<Bullet>& bullets, vector<Enemy>& enemies, int& score, int& numHeartsRemaining);
void drawEndGameMenu(SDL_Renderer* renderer, TTF_Font* font, bool& isRunning, GameObject& player, Uint32& lastEnemySpawn, vector<Bullet>& bullets, vector<Enemy>& enemies, int& score);
int main(int argc, char* argv[]) {
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    SDL_Texture* playerTexture;
    SDL_Texture* backgroundTexture;
    const string backgroundImagePath = "D:\\C++\\Visual_studio\\SDL_GAME_1\\x64\\Debug\\bkgd_menu.png"; // Đường dẫn menu background

    initializeSDL(window, renderer, font, backgroundTexture, playerTexture);

    bool inMenu = true;
    SDL_Event event;

    while (inMenu) {
        drawMenu(renderer, font, window, backgroundImagePath, playerTexture, player, lastEnemySpawn, bullets, enemies, score);
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                inMenu = false; // Thoát khỏi menu nếu đóng cửa sổ
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                if (mouseX >= 400 && mouseX <= 600 && mouseY >= 200 && mouseY <= 250) {
                    //  chọn "Start"
                    bool isRunning = true;

                    // Hiển thị đếm ngược trước khi bắt đầu trò chơi
                    countdownTimer(renderer, font);

                    runGameLoop(renderer, window, font, backgroundTexture, playerTexture,
                        player, lastEnemySpawn, bullets, enemies, score, isRunning);
                    inMenu = false; // Thoát khỏi menu sau khi kết thúc trò chơi
                }
                else if (mouseX >= 400 && mouseX <= 600 && mouseY >= 300 && mouseY <= 350) {
                    //  chọn "Exit"
                    inMenu = false; // Thoát khỏi menu
                }
            }
        }
    }

    cleanup(window, renderer, font);

    return 0;
}
void drawMenu(SDL_Renderer* renderer, TTF_Font* font, SDL_Window* window, const string& backgroundImagePath, SDL_Texture* playerTexture,
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





void initializeSDL(SDL_Window*& window, SDL_Renderer*& renderer, TTF_Font*& font, SDL_Texture*& backgroundTexture, SDL_Texture*& playerTexture) {
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
    SDL_Surface* heartSurface = IMG_Load("D:\\C++\\Visual_studio\\SDL_GAME_1\\x64\\Debug\\heart.png");
    if (heartSurface == nullptr) {
        cerr << "Failed to load heart image: " << IMG_GetError() << endl;
        // Xử lý khi không thể tải hình ảnh
    }
    else {
        heartTexture = SDL_CreateTextureFromSurface(renderer, heartSurface);
        SDL_FreeSurface(heartSurface);
    }
}

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


void fireBullet() {
    static int bulletCooldown = 0;
    const int BULLET_COOLDOWN_TIME = 10; // Thời gian chờ giữa các lần bắn đạn

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


void spawnEnemy() {
    Enemy enemy;
    enemy.x = rand() % (SCREEN_WIDTH - 40); // Random dich xuat hien
    enemy.y = 0;
    enemy.w = 60; // width
    enemy.h = 60; // height
    enemies.push_back(enemy); // Thêm địch vào vector enemies
}

void moveBullets() {
    for (int i = 0; i < bullets.size(); i++) {
        if (bullets[i].active) {
            bullets[i].y -= BULLET_SPEED; // Di chuyển lên trên theo hướng y
            if (bullets[i].y < 0) {
                bullets[i].active = false; // out màn hình => mất
            }
        }
    }
}

void moveEnemies() {
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


void checkCollisions(SDL_Renderer* renderer) {
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

void drawPlayer(SDL_Renderer* renderer, GameObject player, SDL_Texture* playerTexture) {
    SDL_Rect playerRect = { player.x, player.y, player.w, player.h };
    SDL_RenderCopy(renderer, playerTexture, NULL, &playerRect);
}

void drawBullets(SDL_Renderer* renderer) {
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



void drawEnemies(SDL_Renderer* renderer) {
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


void drawScore(SDL_Renderer* renderer, TTF_Font* font, int score) {
    SDL_Color textColor = { 255, 255, 0 }; //yellow
    string score_str = "Score: " + to_string(score);
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, score_str.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    int textWidth, textHeight;
    SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);
    SDL_Rect textRect = { 10, 10, textWidth, textHeight }; // vi tri hien diem so

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}
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
        moveEnemiesAndCheckHearts(renderer, numHeartsRemaining, isRunning, score);// Di chuyển enemy và kiểm tra trái tim

        checkCollisions(renderer);

        // Vẽ background
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        drawPlayer(renderer, player, playerTexture);
        drawBullets(renderer);
        drawEnemies(renderer);
        drawScore(renderer, font, score);
        drawHearts(renderer, numHeartsRemaining); // Vẽ số lượng trái tim còn lại

        SDL_RenderPresent(renderer);

        if (numHeartsRemaining <= 0) {
            isRunning = false; // Kết thúc trò chơi khi hết trái tim
            // Hiển thị thông báo "Game Over" hoặc thực hiện các hành động khác khi trò chơi kết thúc
            SDL_Color textColor = { 0, 255, 0 };
            // Màu green
            TTF_SetFontSize(font, 50); // Đặt kích thước chữ cho thông báo "Game Over"
            SDL_Surface* gameOverSurface = TTF_RenderText_Blended(font, "Game Over!", textColor);
            SDL_Texture* gameOverTexture = SDL_CreateTextureFromSurface(renderer, gameOverSurface);
            int textWidth, textHeight;
            SDL_QueryTexture(gameOverTexture, NULL, NULL, &textWidth, &textHeight);
            SDL_Rect textRect = { (SCREEN_WIDTH - textWidth) / 2, (SCREEN_HEIGHT - textHeight) / 2, textWidth, textHeight }; // Vị trí để hiển thị thông báo "Game Over"
            SDL_RenderCopy(renderer, gameOverTexture, NULL, &textRect);
            SDL_FreeSurface(gameOverSurface);
            SDL_DestroyTexture(gameOverTexture);
            SDL_RenderPresent(renderer);

            SDL_Delay(3000); // Dừng chương trình trong 3 giây trước khi thoát
            drawEndGameMenu(renderer, font, isRunning, player, lastEnemySpawn, bullets, enemies, score);
            numHeartsRemaining = 3;
        }
        if (score >= 200) {
            isRunning = false; // Kết thúc trò chơi khi đạt điều kiện chiến thắng
            // Hiển thị thông báo "You Win" hoặc thực hiện các hành động khác khi trò chơi kết thúc
            SDL_Color textColor = { 0, 255, 0 }; // Màu xanh lá cây
            TTF_SetFontSize(font, 50); // Đặt kích thước chữ cho thông báo "You Win"
            SDL_Surface* youWinSurface = TTF_RenderText_Blended(font, "You Win", textColor);
            SDL_Texture* youWinTexture = SDL_CreateTextureFromSurface(renderer, youWinSurface);
            int textWidth, textHeight;
            SDL_QueryTexture(youWinTexture, NULL, NULL, &textWidth, &textHeight);
            SDL_Rect textRect = { (SCREEN_WIDTH - textWidth) / 2, (SCREEN_HEIGHT - textHeight) / 2, textWidth, textHeight }; // Vị trí để hiển thị thông báo "You Win"
            SDL_RenderCopy(renderer, youWinTexture, NULL, &textRect);
            SDL_FreeSurface(youWinSurface);
            SDL_DestroyTexture(youWinTexture);
            SDL_RenderPresent(renderer);

            SDL_Delay(3000); // Dừng chương trình trong 3 giây trước khi thoát
            drawEndGameMenu(renderer, font, isRunning, player, lastEnemySpawn, bullets, enemies, score);
            numHeartsRemaining = 3;
        }

    }

    // Dọn dẹp và thoát chương trình sau khi người dùng thoát
    cleanup(window, renderer, font);
}

void countdownTimer(SDL_Renderer* renderer, TTF_Font* font) {
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
void drawHearts(SDL_Renderer* renderer, int numHeartsRemaining) {
    //  heart's position and size
    int heartWidth = 50;
    int heartHeight = 50;
    int heartMargin = 5; // hearts's length

    // Vẽ số lượng trái tim còn lại
    int startX = SCREEN_WIDTH - (numHeartsRemaining * (heartWidth + heartMargin)) - 10;
    int startY = 10;
    for (int i = 0; i < numHeartsRemaining; i++) {
        SDL_Rect heartRect = { startX + i * (heartWidth + heartMargin), startY, heartWidth, heartHeight };
        SDL_RenderCopy(renderer, heartTexture, NULL, &heartRect);
    }
}
void moveEnemiesAndCheckHearts(SDL_Renderer* renderer, int& numHeartsRemaining, bool& isRunning, int& score) {
    for (int i = 0; i < enemies.size(); i++) {
        enemies[i].y += ENEMY_SPEED; // Di chuyển địch xuống dưới theo hướng y

        bool enemyHitByBullet = false; // Biến kiểm tra xem kẻ địch có bị đánh bởi đạn hay không

        for (int j = 0; j < bullets.size(); j++) {
            if (bullets[j].active && isColliding(bullets[j], enemies[i])) {
                bullets[j].active = false; // Đánh dấu đạn đã trúng kẻ địch
                enemies.erase(enemies.begin() + i); // Xóa kẻ địch khi bị trúng đạn
                i--; // Giảm chỉ số i đi 1 sau khi xóa phần tử
                enemyHitByBullet = true; // Đánh dấu kẻ địch bị trúng đạn
                score += 10; // Tăng điểm khi trúng địch
                break; // Thoát khỏi vòng lặp khi kẻ địch bị trúng đạn
            }
        }

        if (!enemyHitByBullet && enemies[i].y > SCREEN_HEIGHT) {
            enemies.erase(enemies.begin() + i); // Xóa kẻ địch khi thoát khỏi màn hình
            i--; // Giảm chỉ số i đi 1 sau khi xóa phần tử
            numHeartsRemaining--; // Giảm số lượng trái tim nếu kẻ địch thoát ra khỏi màn hình
            if (numHeartsRemaining <= 0) {
                isRunning = false; // Kết thúc trò chơi khi hết trái tim
                break; // Thoát khỏi vòng lặp khi trò chơi kết thúc
            }
        }
    }
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
                    // Chọn "Play again" - Reset trạng thái của trò chơi và bắt đầu một ván mới
                    inEndGameMenu = false;
                    resetGame(player, lastEnemySpawn, bullets, enemies, score, numHeartsRemaining);
                    isRunning = true;
                }
                else if (mouseX >= 400 && mouseX <= 600 && mouseY >= 300 && mouseY <= 350) {
                    // Chọn "Exit" - Dừng trò chơi
                    inEndGameMenu = false;
                    isRunning = false;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_Rect backgroundRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

        // Vẽ "Play again"
        SDL_Color textColor = { 255, 255, 255 }; // Màu trắng
        TTF_SetFontSize(font, 50);
        SDL_Surface* playAgainSurface = TTF_RenderText_Blended(font, "Play again", textColor);
        SDL_Texture* playAgainTexture = SDL_CreateTextureFromSurface(renderer, playAgainSurface);
        SDL_Rect playAgainRect = { (SCREEN_WIDTH - playAgainSurface->w) / 2, 200, playAgainSurface->w, playAgainSurface->h };
        SDL_RenderCopy(renderer, playAgainTexture, NULL, &playAgainRect);
        SDL_FreeSurface(playAgainSurface);
        SDL_DestroyTexture(playAgainTexture);

        // Vẽ "Exit"
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
void resetGame(GameObject& player, Uint32& lastEnemySpawn, vector<Bullet>& bullets, vector<Enemy>& enemies, int& score, int& numHeartsRemaining) {
    player.x = SCREEN_WIDTH / 2;
    player.y = SCREEN_HEIGHT - 50;
    lastEnemySpawn = 0;
    bullets.clear();
    enemies.clear();
    score = 0;
    numHeartsRemaining = 3; // Thiết lập số lượng trái tim ban đầu
}

