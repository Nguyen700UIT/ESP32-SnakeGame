#pragma once
#include <Arduino.h>
#include <snake_bait.h>

//Game 
extern const int moveDelay;
extern unsigned long prevTime;
extern unsigned long currTime;

extern int prevScore;
extern int score;

struct Bait
{
    int x;
    int y;
    uint16_t baitColor;
};
extern Bait bait;
extern const uint8_t baitBmp[8] PROGMEM;
extern const uint8_t headUp[8] PROGMEM;
extern const uint8_t headDown[8] PROGMEM;
extern const uint8_t headLeft[8] PROGMEM;
extern const uint8_t headRight[8] PROGMEM;
extern const uint8_t snakeBody[8] PROGMEM;

void initBait();
void readDirection();
void move();
void eat();
void drawSnake();
void drawBait();
void isGameOver();
void drawPlayingUI();
void drawScore();
void drawGameOverUI();
void renderBoard();
void gameReset();