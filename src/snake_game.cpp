#include <snake_game.h>
#include <snake_bait.h>
#include <display.h>
#include <config.h>

const int moveDelay = 150;
unsigned long prevTime = 0;
unsigned long currTime = 0;
int prevScore = -1;
int score = 0;
Bait bait;
const uint8_t baitBmp[8] PROGMEM = {0x18, 0x10, 0x7e, 0xff, 0xff, 0xfb, 0x7e, 0x3c};
const uint8_t headUp[8] PROGMEM = {0x00, 0x18, 0x3c, 0x7e, 0x7e, 0x5a, 0x7e, 0x7e};
const uint8_t headDown[8] PROGMEM = {0x7e, 0x7e, 0x5a, 0x7e, 0x7e, 0x3c, 0x18, 0x00};
const uint8_t headLeft[8] PROGMEM = {0x00, 0x1f, 0x3b, 0x7f, 0x7f, 0x3b, 0x1f, 0x00};
const uint8_t headRight[8] PROGMEM = {0x00, 0xf8, 0xdc, 0xfe, 0xfe, 0xdc, 0xf8, 0x00};
const uint8_t snakeBody[8] PROGMEM = {0x00, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x00};
void initBait()
{
    int type = random(0, 2);
    bait.baitColor = baitColor[type];
    // x, y must be even cause BLOCK_SIZE = 8
    int maxCol = SNAKE_BOARD_WIDTH / SNAKE_BLOCK_SIZE;
    int maxRow = SNAKE_BOARD_HEIGHT / SNAKE_BLOCK_SIZE;
    bait.x = random(0, maxCol) * SNAKE_BLOCK_SIZE;
    bait.y = random(0, maxRow) * SNAKE_BLOCK_SIZE;
}

// Reset snake to normal state
void resetSnake() {
    headX = 64;
    headY = 32;
    nTail = 1;
}

void readDirection()
{
    // Đọc cờ ngắt và chặn lỗi quay đầu 180 độ
    if (isUp && snakeDir != down) {
        snakeDir = up;
    }
    else if (isDown && snakeDir != up) {
        snakeDir = down;
    }
    else if (isLeft && snakeDir != right) {
        snakeDir = left;
    }
    else if (isRight && snakeDir != left) {
        snakeDir = right;
    }

    isUp = false;
    isDown = false;
    isLeft = false;
    isRight = false;
}

void move()
{
    int prevX = tailX[0];
    int prevY = tailY[0];
    tailX[0] = headX;
    tailY[0] = headY;
    int tempX, tempY;
    for (int i = 1; i < nTail; ++i)
    {
        tempX = tailX[i];
        tempY = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = tempX;
        prevY = tempY;
    }
    switch(snakeDir)
    {
        case up:
            headY-=SNAKE_BLOCK_SIZE;
            break;
        case down:
            headY+=SNAKE_BLOCK_SIZE;
            break;
        case left:
            headX-=SNAKE_BLOCK_SIZE;
            break;
        case right:
            headX+=SNAKE_BLOCK_SIZE;
            break;
    }
    if (headX < 0) headX = SNAKE_BOARD_WIDTH - SNAKE_BLOCK_SIZE; else if (headX >= SNAKE_BOARD_WIDTH) headX = 0;
    if (headY < 0) headY = SNAKE_BOARD_HEIGHT - SNAKE_BLOCK_SIZE; else if (headY >= SNAKE_BOARD_HEIGHT) headY = 0;

}

void isGameOver()
{
    //if(headX < 0 || headX >= SCREEN_WIDTH || headY < 0 || headY >= SCREEN_HEIGHT)
    //    gameOver = true;
    for (int i = 0; i < nTail - 1; ++i)
    {
        if (headX == tailX[i] && headY == tailY[i]) 
        gameOver = true;
    }
}

void eat()
{
    if (headX == bait.x && headY == bait.y) {
        nTail++;
        initBait();
    }
}

void drawSnake()
{
    // 1. CHỌN MẢNG ĐẦU RẮN THEO HƯỚNG
    const uint8_t* currentHeadBmp = headRight; // Mặc định nếu mới vào game
    if (snakeDir == up) currentHeadBmp = headUp;
    else if (snakeDir == down) currentHeadBmp = headDown;
    else if (snakeDir == left) currentHeadBmp = headLeft;
    else if (snakeDir == right) currentHeadBmp = headRight;

    // 2. VẼ ĐẦU RẮN
    snakeCanvas.drawBitmap(headX, headY, currentHeadBmp, SNAKE_BLOCK_SIZE, SNAKE_BLOCK_SIZE, TFT_DARKGREEN);

    // 3. VẼ ĐUÔI RẮN BẰNG BITMAP
    for(int i = 0; i < nTail - 1; i++)
    {
        snakeCanvas.drawBitmap(tailX[i], tailY[i], snakeBody, SNAKE_BLOCK_SIZE, SNAKE_BLOCK_SIZE, TFT_GREEN);
    }
}
void drawBait()
{
    snakeCanvas.drawBitmap(bait.x, bait.y, baitBmp, SNAKE_BLOCK_SIZE, SNAKE_BLOCK_SIZE, bait.baitColor);
}
void drawPlayingUI()
{
    tft_snake.drawFastVLine(SNAKE_BOARD_WIDTH, 0, SNAKE_BOARD_HEIGHT, TFT_WHITE);
    tft_snake.setTextSize(2);
    tft_snake.setTextColor(TFT_WHITE);
    tft_snake.setCursor(SNAKE_BOARD_WIDTH + 5, 20);
    tft_snake.println("Score: ");
}

void drawScore()
{
    tft_snake.setTextSize(2);
    tft_snake.setTextColor(TFT_WHITE, TFT_BLACK); // Xóa nền đen
    tft_snake.setCursor(SNAKE_BOARD_WIDTH + 5, 45);
    tft_snake.printf("%05d", score); // In 5 chữ số giống Tetris
}

void renderBoard()
{
    snakeCanvas.fillSprite(TFT_BLACK);

    drawSnake(); 
    drawBait();
    snakeCanvas.pushSprite(SNAKE_BOARD_OFFSET_X, SNAKE_BOARD_OFFSET_Y);
}

void drawGameOverUI()
{
    tft_snake.setTextSize(2); 
    tft_snake.setTextColor(TFT_WHITE);
    tft_snake.setCursor(SCREEN_WIDTH/2 - 50, SCREEN_HEIGHT/2);
    tft_snake.println("GAME OVER");
}

void gameReset()
{
    score = 0;
    prevScore = -1; // Ép vẽ lại điểm số lần đầu
    headX = 64;
    headY = 32;
    nTail = 1;
    snakeDir = right;
    gameOver = false;
    
    tft_snake.fillScreen(TFT_BLACK);
    drawPlayingUI(); // Khởi tạo lại khung UI tĩnh
    initBait();
}