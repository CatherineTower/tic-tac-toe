#include "board.h"

static inline int boardCoordinate(GameBoard const *board, int x, int y);

int GameBoardInit(GameBoard *board, int dimension) {
  assert(dimension > 0);

  board->dimension = dimension;

  board->spaces = malloc(sizeof(int) * dimension * dimension);
  assert(board->spaces != NULL);
  memset(board->spaces, SPACE_FREE, sizeof(int) * dimension * dimension);

  board->cursorX = 0;
  board->cursorY = 0;

  return 0;
}

void GameBoardDestroy(GameBoard *board) {
  if(board != NULL) {
    free(board->spaces);
  }
}

void setSpace(GameBoard *board, int x, int y, SpaceDesignation designation) {
  board->spaces[boardCoordinate(board, x, y)] = designation;
}

/* This is kind of a useless function, isn't it? */
SpaceDesignation getSpace(GameBoard const *board, int x, int y) {
  return board->spaces[boardCoordinate(board, x, y)];
}

/* I don't want to have to do the calculation every time */
static inline int boardCoordinate(GameBoard const *board, int x, int y) {
  assert(board != NULL && board->spaces != NULL);
  assert(x <= board->dimension && y <= board->dimension);

  return (y * board->dimension) + x;
}

SpaceDesignation testWin(GameBoard const *board) {
  /* test rows */
  for(int y = 0; y < board->dimension; y++) {
    /* spacesCount[0] is the number of spaces the player's got for a
     particular run. spacesCount[1] is the number of spaces the CPU's
     got. */
  int spacesCount[2] = {};
    for(int x = 0; x < board->dimension; x++) {
      switch(board->spaces[boardCoordinate(board, x, y)]) {
      default:
        continue;
      case SPACE_PLAYER:
        spacesCount[0]++;
        break;
      case SPACE_CPU:
        spacesCount[1]++;
        break;
      }
    }
    if(spacesCount[0] == board->dimension) {
      return SPACE_PLAYER;
    }
    if(spacesCount[1] == board->dimension) {
      return SPACE_CPU;
    }
  }

  /* Test columns */
  for(int x = 0; x < board->dimension; x++) {
    int spacesCount[2] = {};
    for(int y = 0; y < board->dimension; y++) {
      switch(board->spaces[boardCoordinate(board, x, y)]) {
      default:
        continue;
      case SPACE_PLAYER:
        spacesCount[0]++;
        break;
      case SPACE_CPU:
        spacesCount[1]++;
        break;
      }
    }
    if(spacesCount[0] == board->dimension) {
      return SPACE_PLAYER;
    }
    if(spacesCount[1] == board->dimension) {
      return SPACE_CPU;
    }
  }

  /* Test diagonals */
  int spacesCount[2] = {};
  for(int i = 0; i < board->dimension; i++) {
    switch(board->spaces[boardCoordinate(board, i, i)]) {
    default:
      continue;
    case SPACE_PLAYER:
      spacesCount[0]++;
      break;
    case SPACE_CPU:
      spacesCount[1]++;
      break;
    }
  }
  if(spacesCount[0] == board->dimension) {
    return SPACE_PLAYER;
  }
  if(spacesCount[1] == board->dimension) {
    return SPACE_CPU;
  }

  spacesCount[0] = 0;
  spacesCount[1] = 0;
  for(int i = 0; i < board->dimension; i++) {
    switch(board->spaces[boardCoordinate(board, board->dimension - (i + 1), i)]) {
    default:
      continue;
    case SPACE_PLAYER:
      spacesCount[0]++;
      break;
    case SPACE_CPU:
      spacesCount[1]++;
      break;
    }
  }
  if(spacesCount[0] == board->dimension) {
    return SPACE_PLAYER;
  }
  if(spacesCount[1] == board->dimension) {
    return SPACE_CPU;
  }
  return SPACE_FREE;
}

void setCursor(GameBoard *board, int x, int y) {
  if(x >= board->dimension || y >= board->dimension ||
     x < 0 || y < 0) {
    return;
  }

  board->cursorX = x;
  board->cursorY = y;
}

void moveCursor(GameBoard *board, CursorDirection direction) {
  switch(direction) {
  default:
    /* Shouldn't happen; crash immediately */
    assert(0);
  case CURSOR_UP:
    board->cursorY--;
    break;
  case CURSOR_RIGHT:
    board->cursorX++;
    break;
  case CURSOR_DOWN:
    board->cursorY++;
    break;
  case CURSOR_LEFT:
    board->cursorX--;
    break;
  }

  /* Account for overflow */
  if(board->cursorX < 0) {
    board->cursorX += board->dimension;
  }
  if(board->cursorY < 0) {
    board->cursorY += board->dimension;
  }

  board->cursorX %= board->dimension;
  board->cursorY %= board->dimension;
}

#ifdef UNITTEST_BOARD
int main(void) {

  /* Make sure initialization works as expected */
  GameBoard test;
  GameBoardInit(&test, 3);
  assert(test.dimension == 3);
  assert(testWin(&test) == SPACE_FREE);
  assert(test.cursorX == 0);
  assert(test.cursorY == 0);

  /* Quick sanity check to make sure this works as expected */
  assert(boardCoordinate(&test, 1, 1) == 4);
  assert(boardCoordinate(&test, 2, 2) == 8);

  /* Can we set spaces? Is this a winning board? Yes and no in that
     order, hopefully */
  setSpace(&test, 0, 0, SPACE_PLAYER);
  setSpace(&test, 1, 1, SPACE_CPU);
  setSpace(&test, 2, 2, SPACE_PLAYER);
  assert(testWin(&test) == SPACE_FREE);
  /* Did the spaces set correctly? */
  assert(test.spaces[0] == SPACE_PLAYER);
  assert(test.spaces[4] == SPACE_CPU);
  assert(test.spaces[8] == SPACE_PLAYER);

  /* Set some spaces for a winning board */
  setSpace(&test, 0, 0, SPACE_PLAYER);
  setSpace(&test, 1, 0, SPACE_PLAYER);
  setSpace(&test, 2, 0, SPACE_PLAYER);
  assert(testWin(&test) == SPACE_PLAYER);

  /* Destroy the evidence. This struct can be reused, however. */
  GameBoardDestroy(&test);

  /* Four-square tic-tac-toe */
  GameBoardInit(&test, 4);
  setSpace(&test, 0, 0, SPACE_CPU);
  setSpace(&test, 1, 0, SPACE_CPU);
  setSpace(&test, 2, 0, SPACE_CPU);
  assert(testWin(&test) == SPACE_FREE);
  setSpace(&test, 3, 0, SPACE_CPU);
  assert(testWin(&test) == SPACE_CPU);

  /* Test the cursor location */
  setCursor(&test, 1, 1);
  assert(test.cursorX == 1);
  assert(test.cursorY == 1);

  /* If the coordinate given is outside the dimension, nothing should
     happen. */
  setCursor(&test, 25, 6);
  assert(test.cursorX == 1);
  assert(test.cursorY == 1);

  /* Should now be at (1, 2) */
  moveCursor(&test, CURSOR_DOWN);

  /* (1, 3) */
  moveCursor(&test, CURSOR_DOWN);

  /* (1, 0) overflow */
  moveCursor(&test, CURSOR_DOWN);

  /* (1, 3) overflow in the other direction */
  moveCursor(&test, CURSOR_UP);

  return 0;
}
#endif  /* UNITTEST_BOARD */
