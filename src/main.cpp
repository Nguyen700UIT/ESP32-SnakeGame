#include <Arduino.h>
#include <display.h>
#include <snake_game.h>
#include <config.h>

void IRAM_ATTR upISR() {
  if(gameOver) return;
  unsigned long currInterrruptTime = millis();
  if (currInterrruptTime - lastInterruptTime[0] > 100) {
    isUp = true;
    lastInterruptTime[0] = currInterrruptTime;
  }
}

void IRAM_ATTR downISR() {
  if(gameOver) return;
  unsigned long currInterrruptTime = millis();
  if (currInterrruptTime - lastInterruptTime[1] > 100) {
    isDown = true;
    lastInterruptTime[1] = currInterrruptTime;
  }
}

void IRAM_ATTR leftISR() {
  if(gameOver) return;
  unsigned long currInterrruptTime = millis();
  if (currInterrruptTime - lastInterruptTime[2] > 100) {
    isLeft = true;
    lastInterruptTime[2] = currInterrruptTime;
  }
}

void IRAM_ATTR rightISR() {
  if(gameOver) return;
  unsigned long currInterrruptTime = millis();
  if (currInterrruptTime - lastInterruptTime[3] > 100) {
    isRight = true;
    lastInterruptTime[3] = currInterrruptTime;
  }
}

void IRAM_ATTR resetISR() {
  unsigned long currInterruptTime = millis();
  if(currInterruptTime - lastResetInterruptTime > 100) {
    reseted = true;
    lastResetInterruptTime = currInterruptTime;
  }
}

void setup() {
    Serial.begin(9600);
    // Initialize buttons
    pinMode(UP, INPUT_PULLUP);
    pinMode(DOWN, INPUT_PULLUP);
    pinMode(LEFT, INPUT_PULLUP);
    pinMode(RIGHT, INPUT_PULLUP);
    pinMode(RESET, INPUT_PULLUP);
    
    // Attach Interrupts
    attachInterrupt(UP, upISR, FALLING);
    attachInterrupt(DOWN, downISR, FALLING);
    attachInterrupt(LEFT, leftISR, FALLING);
    attachInterrupt(RIGHT, rightISR, FALLING);
    attachInterrupt(RESET, resetISR, FALLING);

    initDisplay();
    Serial.println("TFT Display initialized");

    gameReset(); // Khởi tạo toàn bộ Game
}

void loop() {
    currTime = millis();

    if(!gameOver)
    {
        // Prevent future reset flag lingering
        if (reseted) reseted = false;
        if(currTime - prevTime > moveDelay)
        {   
            prevTime = currTime;
            
            readDirection(); 
            move();         
            bool ate = eat();           
            isGameOver();   
            if (!gameOver)
            {
              // Xử lý điểm số
              score = (nTail - 1) * 10;
              if (score != prevScore)
              {
                  drawScore();
                  prevScore = score;
              }

              renderBoard(ate);  
            } 
            
        }
    }
    else
    {
        // Chết thì vẽ UI Game Over (có chốt khóa chống chớp bên trong hàm)
        drawGameOverUI();

        // Đợi người chơi bấm Reset
        if (reseted) {
            gameReset();
            reseted = false;
        }
    }
}


