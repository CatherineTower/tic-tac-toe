#ifndef BOARD_H
#define BOARD_H

#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef enum SpaceDesignation {
  SPACE_FREE,
  SPACE_PLAYER,
  SPACE_CPU,
} SpaceDesignation;

typedef enum CursorDirection {
  CURSOR_UP,
  CURSOR_RIGHT,
  CURSOR_DOWN,
  CURSOR_LEFT,
} CursorDirection;

typedef struct GameBoard {
  int dimension;
  SpaceDesignation *spaces;
  int cursorX;
  int cursorY;
} GameBoard;

int GameBoardInit(GameBoard *board, int dimension);
void GameBoardDestroy(GameBoard *board);

void setSpace(GameBoard *board, int x, int y, SpaceDesignation designation);
SpaceDesignation getSpace(GameBoard const *board, int x, int y);

SpaceDesignation testWin(GameBoard const *board);
void setCursor(GameBoard *board, int x, int y);
void moveCursor(GameBoard *board, CursorDirection direction);

#endif  /* BOARD_H */
