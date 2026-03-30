#include <display.h>
#include <config.h>

TFT_eSPI tft_snake;
TFT_eSprite snakeCanvas = TFT_eSprite(&tft_snake);
void initDisplay()
{
    tft_snake.init();
    tft_snake.setRotation(1);
    tft_snake.fillScreen(TFT_BLACK);
    

    

}