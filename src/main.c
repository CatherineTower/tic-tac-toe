#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <stdio.h>
#include <ctype.h>

#include "interface.h"
#include "board.h"

static void usage(int retval);
static int getBoardSize(char const *arg);
static void cpuTurn(GameBoard *board);
static int getPlayerInput(void);
static CursorDirection inputToDirection(int ch);
static inline int isValidKey(int ch);
static inline int canPlacePlayerPiece(GameBoard const *board);
static void playerTurn(GameBoard *board);
static void printMessage(char const *message, int yCoordinate);

/* Name of the program, as passed in by the OS */
static char const *PROGNAME;

static void usage(int retval) {
  fprintf(retval == 0 ? stdout : stderr, "Usage: %s [OPTIONAL BOARD SIZE]\n", PROGNAME);
  exit(retval);
}

/* A return value of zero means it's not valid */
static int getBoardSize(char const *arg) {
  char const *ch = arg;
  while(*ch != '\0') {
    if(!isdigit(*ch)) {
      return 0;
    }
    ch++;
  }
  return atoi(arg);
}

static void cpuTurn(GameBoard *board) {
  while(1) {
    int xChoice = rand() % board->dimension;
    int yChoice = rand() % board->dimension;

    if(getSpace(board, xChoice, yChoice) == SPACE_FREE) {
      setSpace(board, xChoice, yChoice, SPACE_CPU);
      drawPiece(xChoice, yChoice, CPUPIECE);
      return;
    } else {
      continue;
    }
  }
}

static int getPlayerInput(void) {
  while(1) {
    int ch = getch();
    if(isValidKey(ch)) {
      return ch;
    }
  }
}

static CursorDirection inputToDirection(int ch) {
  switch(ch) {
  default:
    return ch;
  case KEY_DOWN:
    return CURSOR_DOWN;
  case KEY_UP:
    return CURSOR_UP;
  case KEY_RIGHT:
    return CURSOR_RIGHT;
  case KEY_LEFT:
    return CURSOR_LEFT;
  }
}


static inline int isValidKey(int ch) {
  return (ch == KEY_DOWN || ch == KEY_UP ||
          ch == KEY_LEFT || ch == KEY_RIGHT ||
          ch == KEY_ENTER || ch == ' ' ||
          ch == 'q');
}

static inline int canPlacePlayerPiece(GameBoard const *board) {
  return getSpace(board, board->cursorX, board->cursorY) == SPACE_FREE;
}

static void playerTurn(GameBoard *board) {
  board->cursorX = 0;
  board->cursorY = 0;
  drawAllPieces(board);
  drawCursor(0, 0);
  refresh();

  while(1) {
    int ch = getPlayerInput();
    if(ch == 'q') {
      cleanupInterface();
      exit(0);
    } else if(ch == KEY_ENTER || ch == ' ') {
      if(canPlacePlayerPiece(board)) {
        setSpace(board, board->cursorX, board->cursorY, SPACE_PLAYER);
        return;
      }
    } else {
      moveCursor(board, inputToDirection(ch));
      drawAllPieces(board);
      drawCursor(board->cursorX, board->cursorY);
      refresh();
    }
  }
}

static void printMessage(char const *message, int yCoordinate) {
  mvprintw(yCoordinate, 0, message);
}

#ifdef UNITTEST_MAIN

int main(void) {
  initializeInterface();
  srand(time(NULL));
  GameBoard test;
  GameBoardInit(&test, 3);
  drawGrid(3, 3);

  for(int i = 0; i < 5; i++) {
    cpuTurn(&test);
  }
  refresh();
  getch();
  cleanupInterface();
  return 0;
}

#endif  /* UNITTEST_MAIN */


/* Can't have to main()s, that would be crazy! */
#ifndef UNITTEST_MAIN

int main(int argc, char **argv) {
  /* Parse arguments */
  PROGNAME = *argv;
  if(argc > 2) {
    usage(1);
  }
  /* Default is three. It's a classic, after all */
  int boardSize = 3;
  if(argc == 2) {
    boardSize = getBoardSize(argv[1]);
    if(boardSize == 0) {
      usage(1);
    }
  }

  initializeInterface();

  /* Trim the board size to a usable value. */
  if(gridToBoardSize(boardSize) >= LINES || boardSize >= COLS) {
    int trimval = LINES < COLS ? LINES : COLS;
    boardSize = (trimval / 2) - 1;
  }

  /* Set the message location */
  int const messageLocation = gridToBoardSize(boardSize) + 1;

  srand(time(NULL));

  GameBoard board;
  GameBoardInit(&board, boardSize);
  drawGrid(boardSize, boardSize);
  refresh();

  while(1) {
    playerTurn(&board);
    if(testWin(&board) == SPACE_PLAYER) {
      printMessage("Player wins!\n", messageLocation);
      break;
    }
    cpuTurn(&board);
    if(testWin(&board) == SPACE_CPU) {
      printMessage("CPU wins!\n", messageLocation);
      break;
    }
    refresh();
  }
  getch();
  cleanupInterface();
  return 0;
}

#endif  /* !UNITTEST_MAIN */
