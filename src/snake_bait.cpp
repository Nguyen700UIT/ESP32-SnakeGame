#include <snake_bait.h>

int headX = 64;
int headY = 32;
direction snakeDir = stop;
int nTail = 1;
int tailX[100];
int tailY[100];

const uint16_t baitColor[2] = {TFT_YELLOW, TFT_RED};