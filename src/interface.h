#ifndef INTERFACE_H
#define INTERFACE_H

#include <ncurses.h>
#include <assert.h>

#include "board.h"

enum DrawingChars {
  VLINE = '|',
  HLINE = '-',
  CROSSLINE = '+',
  PLAYERPIECE = 'X',
  CPUPIECE = 'O',
  BLANKSPACE = ' ',
};

/* Colors and attributes */
extern int PLAYERPIECE_COLOR;
extern int CPUPIECE_COLOR;
extern int GRID_COLOR;

void initializeInterface(void);
void cleanupInterface(void);

void drawGrid(int xBoxes, int yBoxes);
void drawPiece(int x, int y, enum DrawingChars ch);
void drawCursor(int x, int y);
void drawAllPieces(GameBoard const *board);

int gridToBoardSize(int gridSize);

#endif
