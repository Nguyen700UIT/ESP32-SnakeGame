#pragma once

//Screen definitions
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define GAME_HEIGHT 200

//Buttons
#define UP 25
#define DOWN 26  
#define LEFT 27
#define RIGHT 33
#define RESET 13
//Game definitions
#define SNAKE_BLOCK_SIZE 10
#define SNAKE_BOARD_NUM_ROW 25
#define SNAKE_BOARD_NUM_COL 40 
#define SNAKE_BOARD_WIDTH 320
#define SNAKE_BOARD_HEIGHT 200
#define SNAKE_UI_WIDTH (SCREEN_WIDTH - SNAKE_BOARD_WIDTH)
#define SNAKE_UI_HEIGHT SCREEN_HEIGHT
#define SNAKE_BOARD_OFFSET_X 0
#define SNAKE_BOARD_OFFSET_Y 0
//Stop ISR flag
extern bool gameOver;
extern volatile bool isUp; 
extern volatile bool isDown;
extern volatile bool isLeft;
extern volatile bool isRight;
extern volatile bool reseted;
extern volatile unsigned long lastInterruptTime[4];
extern volatile unsigned long lastResetInterruptTime;
