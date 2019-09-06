#include "interface.h"

static int const BOX_WIDTH = 1;
static int const BOX_HEIGHT = 1;

/* static char const VLINE = '|'; */
/* static char const HLINE = '-'; */
/* static char const CROSSLINE = '+'; */

static void drawColumn(int xCoordinate, int length, char joinChar, char horizontalChar);
static void drawRow(int yCoordinate, int length, char joinChar, char verticalChar);


void initializeInterface(void) {
  initscr();
  curs_set(0);
  noecho();
  raw();
  keypad(stdscr, 1);
  start_color();
}

void cleanupInterface(void) {
  endwin();
}

void drawGrid(int xBoxes, int yBoxes) {
  /* All boxes except for one are two characters: the blank one in the
     middle and the line either to the left or above; the last box is
     three characters. */
  int width = xBoxes * 2 + 1;
  int height = yBoxes * 2 + 1;

  for(int x = 0; x < width; x += BOX_WIDTH + 1) {
    drawColumn(x, height, CROSSLINE, VLINE);
  }
  for(int y = 0; y < height; y += BOX_HEIGHT + 1) {
    drawRow(y, height, CROSSLINE, HLINE);
  }
}

static void drawColumn(int xCoordinate, int length, char joinChar, char verticalChar) {
  for(int i = 1; i < length; i += 2) {
    mvaddch(i, xCoordinate, verticalChar);
  }

  for(int i = 0; i < length; i += 2) {
    mvaddch(i, xCoordinate, joinChar);
  }
}

static void drawRow(int yCoordinate, int length, char joinChar, char horizontalChar) {
  for(int i = 1; i < length; i += 2) {
    mvaddch(yCoordinate, i, horizontalChar);
  }

  for(int i = 0; i < length; i += 2) {
    mvaddch(yCoordinate, i, joinChar);
  }
}

void drawPiece(int x, int y, enum DrawingChars ch) {
  /* Convert from box index to coordinate */
  int xCoordinate = x * 2 + 1;
  int yCoordinate = y * 2 + 1;

  mvaddch(yCoordinate, xCoordinate, ch);
}

void drawCursor(int x, int y) {
  attron(A_BOLD);
  drawPiece(x, y, PLAYERPIECE);
  attroff(A_BOLD);
}

void drawAllPieces(GameBoard const *board) {
  for(int y = 0; y < board->dimension; y++) {
    for(int x = 0; x < board->dimension; x++) {
      enum DrawingChars space;
      switch(getSpace(board, x, y)) {
      default:
        /* Can't happen */
        assert(0);
      case SPACE_FREE:
        space = BLANKSPACE;
        break;
      case SPACE_CPU:
        space = CPUPIECE;
        break;
      case SPACE_PLAYER:
        space = PLAYERPIECE;
        break;
      }
      drawPiece(x, y, space);
    }
  }
}

/* This draws the entire board, and should only be done at the very
   beginning to avoid redrawing the grid unnecessarily */
void drawBoard(GameBoard const *board) {
  drawGrid(board->dimension, board->dimension);
  drawAllPieces(board);
}

/* Useful utility function */
int gridToBoardSize(int gridSize) {
  return gridSize * 2 + 1;
}

#ifdef UNITTEST_INTERFACE
int main(void)
{
  /* Check the main interface code */
  initializeInterface();
  /* Gotta get a clear screen for this */
  clear();
  refresh();
  drawGrid(10, 10);

  /* Let's draw an X just for fun */
  for(int i = 0; i < 10; i++) {
    drawPiece(i, i, PLAYERPIECE);
  }
  for(int i = 0; i < 10; i++) {
    drawPiece(9 - i, i, CPUPIECE);
  }
  refresh();
  getch();
  clear();

  /* Test integration with board code. This time it's dynamically
     allocated because I feel like it and my middle name is Malloc. */
  GameBoard *board = malloc(sizeof(GameBoard));
  if(board == NULL) {
    goto unittest_exit;
  }
  GameBoardInit(board, 3);
  for(int y = 0; y < board->dimension; y++) {
    for(int x = 0; x < board->dimension; x++) {
      setSpace(board, x, y, (x + y) % 2 ? SPACE_PLAYER : SPACE_CPU);
    }
  }
  drawAllPieces(board);
  drawGrid(3, 3);
  refresh();
  getch();

  GameBoardDestroy(board);
  free(board);

  /* Make sure gridToBoardSize() works */
  assert(gridToBoardSize(1) == 3);
  assert(gridToBoardSize(3) == 7);
  assert(gridToBoardSize(10) == 21);
 unittest_exit:
  cleanupInterface();
  return 0;

 unittest_fail:
  cleanupInterface();
  return 1;
}
#endif  /* UNITTEST_INTERFACE */
