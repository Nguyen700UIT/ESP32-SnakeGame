#pragma once
#include <Arduino.h>
#include <snake_bait.h>

//Game 
extern const int moveDelay;
extern unsigned long prevTime;
extern unsigned long currTime;

extern int prevScore;
extern int score;
extern bool drewGameOverUI;
extern int bestScore;

struct Bait
{
    int x;
    int y;
    uint16_t baitColor;
};

extern Bait bait;
extern const uint16_t baitBmp[] PROGMEM;
extern const uint16_t headUp[] PROGMEM;
extern const uint16_t headDown[] PROGMEM;
extern const uint16_t headLeft[] PROGMEM;
extern const uint16_t headRight[] PROGMEM;
extern const uint16_t snakeBody[] PROGMEM;

void initBait();
void readDirection();
void move();
bool eat();
void drawSnake();
void eraseSnakeTail();
void drawBait();
void isGameOver();
void drawPlayingUI();
void drawScore();
void drawGameOverUI();
void renderBoard(bool& ate);
void gameReset();