#include <display.h>
#include <config.h>

TFT_eSPI tft_snake;
TFT_eSprite snakeCanvas = TFT_eSprite(&tft_snake);
void initDisplay()
{
    tft_snake.init();
    tft_snake.setRotation(1);
    tft_snake.fillScreen(TFT_BLACK);
    
    // THÊM DÒNG NÀY ĐỂ ÉP MÀU 8-BIT (GIẢM 50% RAM)
    snakeCanvas.setColorDepth(8);
    
    // Khởi tạo bộ đệm (bây giờ chỉ tốn 57KB, chạy thoải mái)
    if (snakeCanvas.createSprite(SNAKE_BOARD_WIDTH, SNAKE_BOARD_HEIGHT) == nullptr) {
        Serial.println("Loi: Khong du RAM!");
    }
}