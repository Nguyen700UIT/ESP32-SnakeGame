#pragma once
#include <Arduino.h>
#include <display.h>

enum direction {stop = 0, up, down, left, right};

//Snake
extern int headX;
extern int headY;
extern int prevTailX;
extern int prevTailY;
extern direction snakeDir;
extern int nTail;
extern int tailX[100];
extern int tailY[100];

//Bait
extern const uint16_t baitColor[2];