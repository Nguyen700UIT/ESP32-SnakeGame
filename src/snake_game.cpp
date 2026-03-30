#include <snake_game.h>
#include <snake_bait.h>
#include <display.h>
#include <config.h>

const int moveDelay = 150;
unsigned long prevTime = 0;
unsigned long currTime = 0;
int prevScore = -1;
int score = 0;
bool drewGameOverUI = false;
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
    bool valid = false;

    while(!valid)
    {
        valid = true;
        bait.x = random(0, maxCol) * SNAKE_BLOCK_SIZE;
        bait.y = random(0, maxRow) * SNAKE_BLOCK_SIZE;
        if(headX == bait.x && headY == bait.y)
        {
            valid = false;
            continue;
        }
        for (int i = 0; i < nTail; ++i)
        {
            if(tailX[i] == bait.x && tailY[i] == bait.y)
            {
                valid = false;
                break;
            }
        }
    }
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
    prevTailX = tailX[nTail-1];
    prevTailY = tailY[nTail-1];
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
    // if (headX < 0) headX = SNAKE_BOARD_WIDTH - SNAKE_BLOCK_SIZE; else if (headX >= SNAKE_BOARD_WIDTH) headX = 0;
    // if (headY < 0) headY = SNAKE_BOARD_HEIGHT - SNAKE_BLOCK_SIZE; else if (headY >= SNAKE_BOARD_HEIGHT) headY = 0;

}

void isGameOver()
{
    if(headX < 0 || headX >= SNAKE_BOARD_WIDTH || headY < 0 || headY >= SNAKE_BOARD_HEIGHT)
       gameOver = true;
    for (int i = 0; i < nTail - 1; ++i)
    {
        if (headX == tailX[i] && headY == tailY[i]) 
        {
            gameOver = true;
            break;
        }
    }
}

bool eat()
{
    if (headX == bait.x && headY == bait.y) 
    { 
        tft_snake.drawBitmap(bait.x, bait.y, baitBmp, SNAKE_BLOCK_SIZE, SNAKE_BLOCK_SIZE, TFT_BLACK); //Delete bait
        nTail++;
        initBait();
        return true;
    }
    return false;
}

void drawSnake(bool& ate)
{
    if(true)
    {
        tft_snake.fillRect(prevTailX, prevTailY, SNAKE_BLOCK_SIZE, SNAKE_BLOCK_SIZE, TFT_BLACK);
    }
    // 1. CHỌN MẢNG ĐẦU RẮN THEO HƯỚNG
    const uint8_t* currentHeadBmp = headRight; // Mặc định nếu mới vào game
    if (snakeDir == up) currentHeadBmp = headUp;
    else if (snakeDir == down) currentHeadBmp = headDown;
    else if (snakeDir == left) currentHeadBmp = headLeft;
    else if (snakeDir == right) currentHeadBmp = headRight;
    // 3. VẼ THÂN RẮN BẰNG BITMAP
    tft_snake.drawBitmap(tailX[0], tailY[0], snakeBody, SNAKE_BLOCK_SIZE, SNAKE_BLOCK_SIZE, TFT_GREEN);

    // 2. VẼ ĐẦU RẮN
    tft_snake.drawBitmap(headX, headY, currentHeadBmp, SNAKE_BLOCK_SIZE, SNAKE_BLOCK_SIZE, TFT_DARKGREEN);


}


void drawBait()
{
    tft_snake.drawBitmap(bait.x, bait.y, baitBmp, SNAKE_BLOCK_SIZE, SNAKE_BLOCK_SIZE, bait.baitColor);
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

void renderBoard(bool &ate)
{
    drawSnake(ate);
    if(ate) 
        
        drawBait();
}

void drawGameOverUI()
{
    if(!drewGameOverUI)
    {
        tft_snake.setTextSize(2); 
        tft_snake.setTextColor(TFT_WHITE);
        tft_snake.setCursor(SCREEN_WIDTH/2 - 50, SCREEN_HEIGHT/2);
        tft_snake.println("GAME OVER");
        drewGameOverUI = true;
    }

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
    drewGameOverUI = false;
    drawPlayingUI(); // Khởi tạo lại khung UI tĩnh
    initBait();
    drawBait();
}