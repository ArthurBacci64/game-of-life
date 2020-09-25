/*** INCLUDES ***/

#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*** DEFINES ***/

#define CTRL_KEY(k) ((k)&0x1f) // CTRL+k

/*** GRID ***/

struct Grid
{
  int cols;
  int rows;
  int realcols;
  unsigned char *data;
};

struct Grid grid = {0, 0, 0, NULL};

// Allocates the grid size
void setGrid(int cols, int rows, struct Grid *g)
{
  g->cols = cols / 2; // A block ocupates two characters of width
  g->rows = rows;
  g->realcols = cols;
  free(g->data);
  g->data = (unsigned char *)calloc(cols * rows, sizeof(unsigned char));
}

// Shows the grid and free the grid
void showGrid(int cx, int cy)
{
  for (int i = 0; i < grid.rows; i++)
  {
    for (int j = 0; j < grid.cols; j++)
    {
      if (grid.data[i * grid.cols + j])
        attrset(A_REVERSE);

      if (i == cy && j == cx)
        addstr("##");
      else
        addstr("  ");

      if (grid.data[i * grid.cols + j])
        attroff(A_REVERSE);
    }
    if ((i < (grid.rows - 1)) && ((grid.realcols - (grid.cols * 2)) != 0))
      addstr("\n");
  }
}

unsigned int neighboursOf(int x, int y)
{
  unsigned int n = 0;

  if (x > 0)
  {
    if (y > 0)
      n += grid.data[(y - 1) * grid.cols + (x - 1)];
    n += grid.data[y * grid.cols + (x - 1)];
    if (y < grid.rows - 1)
      n += grid.data[(y + 1) * grid.cols + (x - 1)];
  }

  if (y > 0)
    n += grid.data[(y - 1) * grid.cols + x];
  // That is the middle, I am the middle
  if (y < grid.rows - 1)
    n += grid.data[(y + 1) * grid.cols + x];

  if (x < grid.cols - 1)
  {
    if (y > 0)
      n += grid.data[(y - 1) * grid.cols + (x + 1)];
    n += grid.data[y * grid.cols + (x + 1)];
    if (y < grid.rows - 1)
      n += grid.data[(y + 1) * grid.cols + (x + 1)];
  }

  return n;
}

void updateGrid(int cols, int rows)
{
  struct Grid next = {0, 0, 0, NULL};
  setGrid(cols, rows, &next);

  for (int i = 0; i < grid.rows; i++)
  {
    for (int j = 0; j < grid.cols; j++)
    {
      unsigned char neighbours = neighboursOf(j, i);
      if (grid.data[i * grid.cols + j])
        next.data[i * grid.cols + j] = (neighbours == 2 || neighbours == 3);
      else
        next.data[i * grid.cols + j] = (neighbours == 3);
    }
  }
  grid = next;
}

/*** MAIN FUNCTIONS ***/

char stop = 0;
int cx = 0, cy = 0;
char playing = 0;

int cols, rows;

int CONFIG_PLAYSPEED = 10;

void processKeypress(int k)
{
  if (playing > 0)
  {
    switch (k)
    {
    case CTRL_KEY('c'):
      stop = 1;
      break;
    case 'p':
      playing = 0;
      break;
    case '+':
    case '=':
      if (CONFIG_PLAYSPEED < 1000)
        CONFIG_PLAYSPEED++;
      break;
    case '-':
      if (CONFIG_PLAYSPEED > 0)
        CONFIG_PLAYSPEED--;
      break;
    }
  }
  else
  {
    switch (k)
    {
    case CTRL_KEY('c'):
      stop = 1;
      break;
    case 'p':
      playing = 1;
      break;
    case 's':
      playing = 2;
      break;

    case 'k':
    case CTRL_KEY('p'):
    case KEY_UP:
      cy -= (cy > 0);
      break;
    case 'j':
    case CTRL_KEY('n'):
    case KEY_DOWN:
      cy += (cy < grid.rows - 1);
      break;
    case 'h':
    case CTRL_KEY('b'):
    case KEY_LEFT:
      cx -= (cx > 0);
      break;
    case 'l':
    case CTRL_KEY('f'):
    case KEY_RIGHT:
      cx += (cx < grid.cols - 1);
      break;

    case ' ':
      grid.data[cy * grid.cols + cx] = !grid.data[cy * grid.cols + cx];
      break;
    case 'c':
      setGrid(cols, rows, &grid);
      break;
    }
  }
}

int main()
{
  initscr();
  raw();
  noecho();
  nodelay(stdscr, TRUE);
  curs_set(0);
  keypad(stdscr, TRUE);

  getmaxyx(stdscr, rows, cols);
  setGrid(cols, rows, &grid);

  while (!stop)
  {
    move(0, 0);
    if (playing)
      showGrid(-1, -1);
    else
      showGrid(cx, cy);
    refresh();

    if (playing > 0)
    {
      updateGrid(cols, rows);
      // 1 == playing, 2 == only one step
      if (playing == 2)
        playing = 0;

      napms(1000 / CONFIG_PLAYSPEED);
    }

    int k;
    if ((k = getch()) == ERR)
      continue;
    processKeypress(k);
  }
  endwin();
  return 0;
}
