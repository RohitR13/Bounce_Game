#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include "gba.h"

#include "images/bounce.h"
#include "images/gon.h"
#include "images/basketball.h"

/* TODO: */
// Include any header files for title screen or exit
// screen images generated by nin10kit. Example for the provided garbage
// image:
// #include "images/garbage.h"

/* TODO: */
// Add any additional states you need for your app.
typedef enum {
  START,
  INSTRUCTIONS,
  PLAY,
  END,
} GBAState;

//Initialize the paddle object
struct slider paddle = {120, 50};


//Draws paddle object
void drawPaddle(void){
    drawRectDMA(paddle.row, paddle.col, 30, 5, BLACK);
}

//Smoothens movement of objects
void delay(int n) {
	volatile int x = 0;
	for(int i=0; i<n*8000; i++) {
		x++;
	}
}

//moves paddle left by decreasing col leaving row constant
void movePaddleLeft(void) {

  if (paddle.col < 3) {
     ;
  } else {
    paddle.col = paddle.col - 3;
  }

  drawRectDMA(paddle.row, paddle.col, 30, 5, BLACK);
  delay(1);
  drawRectDMA(paddle.row, paddle.col + 3, 30, 5, WHITE);

}

//moves paddle right by increasing col leaving row constant
void movePaddleRight(void) {

  if (paddle.col > 205) {
     ;
  } else {
    paddle.col = paddle.col + 3;
  }

  drawRectDMA(paddle.row, paddle.col, 30, 5, BLACK);
  delay(1);
  drawRectDMA(paddle.row, paddle.col - 3, 30, 5, WHITE);

}

//moves paddle up by decreasing row leaving col constant
void movePaddleUp(void) {
  if (paddle.row < 60 - 5 + 1) {
    ;
  } else {
    paddle.row = paddle.row - 2;
  }
  drawRectDMA(paddle.row, paddle.col, 30, 5, BLACK);
  delay(1);
  drawRectDMA(paddle.row + 2, paddle.col, 30, 5, WHITE);
}

//moves paddle down by increasing row leaving col constant
void movePaddleDown(void) {
  if (paddle.row > 140 - 5 + 1) {
    ;
  } else {
    paddle.row = paddle.row + 2;
  }
  drawRectDMA(paddle.row, paddle.col, 30, 5, BLACK);
  delay(1);
  drawRectDMA(paddle.row - 2, paddle.col, 30, 5, WHITE);
}

//Initializing the ball object
struct gameBall ball = {10, 10, 10, 1, 1};


//Draws and moves ball
void moveBall(void) {
    drawRectDMA(ball.row, ball.col, ball.size, ball.size, WHITE);
    ball.row += ball.rd;
		ball.col += ball.cd;
		if(ball.row < 0) {
			ball.row = 0;
			ball.rd = -ball.rd;
		}
		if(ball.row > 159 - ball.size + 1) {
			ball.row = 159 - ball.size + 1;
			ball.rd = -ball.rd;
		}
		if(ball.col < 0) {
			ball.col = 0;
			ball.cd = -ball.cd;
		}
		if(ball.col > 239 - ball.size + 1) {
			ball.col = 239 - ball.size + 1;
			ball.cd = -ball.cd;
		}
		drawImageDMA(ball.row, ball.col, ball.size, ball.size, basketball);
}


// Collision Detection
int counter = 0;
void detectCol(void) {
  if (ball.row < paddle.row + 5 &&
      ball.row + ball.size > paddle.row &&
      ball.col < paddle.col + 30 &&
      ball.col + ball.size > paddle.col) {
        ball.rd = -ball.rd;
        ball.cd = -ball.cd;


        counter += 1;
        moveBall();
  }
}





int main(void) {
  /* TODO: */
  // Manipulate REG_DISPCNT here to set Mode 3. //
  REG_DISPCNT = MODE3 | BG2_ENABLE;

  // Save current and previous state of button input.
  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;

  // Load initial game state
  GBAState state = START;

  while (1) {
    currentButtons = BUTTONS;  // Load the current state of the buttons

    /* TODO: */
    // Manipulate the state machine below as needed //
    // NOTE: Call waitForVBlank() before you draw


    switch (state) {
      case START:

      // state --> Title Screen
        waitForVBlank();
        drawFullScreenImageDMA(bounce);
        drawCenteredString(100, 150, 50, 50, "PRESS A TO START", BLACK);
        drawCenteredString(110, 150, 50, 50, "(Z ON THE KEYBOARD)", BLUE);

        if (KEY_JUST_PRESSED(BUTTON_A, currentButtons, previousButtons) == 1) {
          state = INSTRUCTIONS;
          fillScreenDMA(WHITE);
        }


        break;

      case INSTRUCTIONS:

        // state --> General Instructions For the Game
        waitForVBlank();
        drawString(18, 10, "INSTRUCTIONS:", BLACK);
        drawString(48, 20, "* MOVE SLIDER USING D-PAD", BLACK);
        drawString(78, 20, "* YOU NEED 5 BOUNCES TO WIN!!", BLACK);
        drawString(108, 20, "* PRESS START TO BEGIN!!!", BLACK);



        if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons) == 1) {
          state = START;
        }

        if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons) == 1) {
          paddle.row = 120;
          paddle.col = 50;
          ball.row = 10;
          ball.col = 10;
          state = PLAY;
          fillScreenDMA(WHITE);

        }


        break;

      case PLAY:

      // state --> The Actual Game

      moveBall();
      detectCol();
      drawPaddle();
      waitForVBlank();

        if (KEY_DOWN(BUTTON_LEFT, currentButtons)) {
          movePaddleLeft();
        }

        if (KEY_DOWN(BUTTON_RIGHT, currentButtons)) {
          movePaddleRight();
        }

        if (KEY_DOWN(BUTTON_UP, currentButtons)) {
          movePaddleUp();
        }

        if (KEY_DOWN(BUTTON_DOWN, currentButtons)) {
          movePaddleDown();
        }

        if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons) == 1) {
          counter = 0;
          state = START;
        }

        if (counter == 5) {
          counter = 0;
          fillScreenDMA(WHITE);
          state = END;
        }
        break;

      case END:

        drawFullScreenImageDMA(gon);

        if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons) == 1) {
          state = START;
          fillScreenDMA(WHITE);
        }

        break;
    }

    previousButtons = currentButtons;  // Store the current state of the buttons
  }

  return 0;
}