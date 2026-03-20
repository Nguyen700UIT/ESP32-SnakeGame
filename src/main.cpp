#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Button pins
#define UP 4    // Player UP
#define DOWN 5  // Player DOWN
#define LEFT 16
#define RIGHT 17
#define RESET 13

enum direction {stop = 0, up, down, left, right};


//Game 
bool gameOver = false;
const int moveDelay = 50;
int prevTime = 0;
int currTime = 0;
//Snake
int headX = 64;
int headY = 32;
volatile direction snakeDir = stop;
int nTail = 1;
int tailX[100];
int tailY[100];


//Bait
int baitX;
int baitY;

void baitSetup()
{
    baitX = random(0, SCREEN_WIDTH);
    baitY = random(0, SCREEN_HEIGHT);
}
volatile unsigned long lastInterruptTime = 0;
const unsigned long debounce = 150;

void IRAM_ATTR isrUP() {
    unsigned long curr = millis();
    if(curr - lastInterruptTime > debounce) {
        if (snakeDir != down) snakeDir = up; 
        lastInterruptTime = curr;
    }
}

void IRAM_ATTR isrDOWN() {
    unsigned long curr = millis();
    if(curr - lastInterruptTime > debounce) {
        if (snakeDir != up) snakeDir = down;
        lastInterruptTime = curr;
    }
}

void IRAM_ATTR isrLEFT() {
    unsigned long curr = millis();
    if(curr - lastInterruptTime > debounce) {
        if (snakeDir != right) snakeDir = left;
        lastInterruptTime = curr;
    }
}

void IRAM_ATTR isrRIGHT() {
    unsigned long curr = millis();
    if(curr - lastInterruptTime > debounce) {
        if (snakeDir != left) snakeDir = right;
        lastInterruptTime = curr;
    }
}
void setup() {
    Serial.begin(115200);
  
    // Initialize buttons
    pinMode(UP, INPUT_PULLUP);
    pinMode(DOWN, INPUT_PULLUP);
    pinMode(LEFT, INPUT_PULLUP);
    pinMode(RIGHT, INPUT_PULLUP);
    pinMode(RESET, INPUT_PULLUP);

    // Initialize OLED
    Wire.begin(21, 22); // SDA, SCL
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("SSD1306 allocation failed");
        while(true);
    }

    display.clearDisplay();
    display.display();
    Serial.println("Display initialized at 0x3C");
    baitSetup();
    attachInterrupt(digitalPinToInterrupt(UP), isrUP, FALLING);
    attachInterrupt(digitalPinToInterrupt(DOWN), isrDOWN, FALLING);
    attachInterrupt(digitalPinToInterrupt(LEFT), isrLEFT, FALLING);
    attachInterrupt(digitalPinToInterrupt(RIGHT), isrRIGHT, FALLING);
}

// Reset snake to normal state
void resetSnake() {
    headX = 64;
    headY = 32;
    nTail = 1;
    snakeDir = stop;
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
            headY-=2;
            break;
        case down:
            headY+=2;
            break;
        case left:
            headX-=2;
            break;
        case right:
            headX+=2;
            break;
    }
    if (headX < 0) headX = SCREEN_WIDTH - 1; else if (headX >= SCREEN_WIDTH) headX = 0;
    if (headY < 0) headY = SCREEN_HEIGHT - 1; else if (headY >= SCREEN_HEIGHT) headY = 0;

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
    if (headX >= baitX  && headX <= baitX + 4 && headY >= baitY && headY <= baitY + 4)
    {
        nTail++;
        baitSetup();
    }
}

void drawSnake()
{
    //Draw head
    display.drawRect(headX, headY, 4, 4, SSD1306_WHITE);

    //Draw tail
    for(int i = 0; i < nTail - 1; i++)
    {
        display.fillRect(tailX[i], tailY[i], 2, 2, SSD1306_WHITE);
    }

    // Draw bait
    display.fillRect(baitX, baitY, 4, 4, SSD1306_WHITE);
}

void loop() {
    currTime = millis();
    if(!gameOver)
    {
        if(currTime - prevTime > moveDelay)
        {   
            prevTime = currTime;
            move();
            eat();
        }
        display.clearDisplay();
        drawSnake();
        display.display();
        isGameOver();
        
    }
    else if (gameOver)
    {
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(15, 20);
        display.println("GAME OVER");
        
        display.setTextSize(1);
        display.setCursor(30, 45);
        display.print("Score: ");
        display.println(nTail*10);
        display.display();
    }

    if(digitalRead(RESET) == LOW)
    {
        gameOver = false;
        resetSnake();
    }

}


