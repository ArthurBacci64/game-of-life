/*** INCLUDES ***/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <termios.h>


/*** RAW MODE ***/

struct termios terminalConfig;

void disableRawMode()
{
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &terminalConfig);
}

void enableRawMode()
{
  tcgetattr(STDIN_FILENO, &terminalConfig);
  atexit(disableRawMode);

  struct termios tc = terminalConfig;

  tc.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN | ICRNL);
  tc.c_iflag &= ~(IXON);
  tc.c_oflag &= ~(OPOST);

  tc.c_iflag &= ~(BRKINT | INPCK | ISTRIP);
  tc.c_cflag |= CS8;

  tc.c_cc[VMIN] = 0;
  tc.c_cc[VTIME] = 1; // 1 = 1/10s = 100ms
  
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &tc);
}


/*** BUFFER ***/

struct Buffer
{
  int len; // The buffer's length
  unsigned char *data; // The buffer memory location
};

// The global buffer
struct Buffer buf = { 0, NULL };

// To append data to the buffer
void appendBuffer(unsigned char *data, int len)
{
  buf.data = (unsigned char *)realloc(buf.data, buf.len + len);
  memcpy(&buf.data[buf.len], data, len);
  buf.len += len;
}

// Show the buffer
void showBuffer()
{
  write(STDOUT_FILENO, buf.data, buf.len);
  free(buf.data);
  buf.data = NULL;
  buf.len = 0;
}


/*** GRID ***/

struct Grid
{
  int cols;
  int rows;
  unsigned char *data;
};

struct Grid grid = { 0, 0, NULL };

// Allocates the grid size
void setGrid(int cols, int rows, struct Grid *g)
{
  g->cols = cols;
  g->rows = rows;
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
      if (i == cy && j == cx)
	appendBuffer("\x1b[4m", 4);
	
      if (grid.data[i * grid.cols + j])
	appendBuffer("██", 6);
      else
	appendBuffer("  ", 2);

      if (i == cy && j == cx)
	appendBuffer("\x1b[m", 3);
    }
    appendBuffer("\r\n", 2 * (i < grid.rows - 1));
  }
}

unsigned int neighboursOf(int x, int y)
{
  unsigned int n = 0;
  
  if (x > 0)
  {
    if (y > 0)
      n += grid.data[(y-1) * grid.cols + (x-1)];
    n += grid.data[y * grid.cols + (x-1)];
    if (y < grid.rows - 1)
      n += grid.data[(y+1) * grid.cols + (x-1)];
  }
  
  if (y > 0)
    n += grid.data[(y-1) * grid.cols + x];
  // That is the middle, I am the middle
  if (y < grid.rows - 1)
    n += grid.data[(y+1) * grid.cols + x];

  if (x < grid.cols - 1)
  {
    if (y > 0)
      n += grid.data[(y-1) * grid.cols + (x+1)];
    n += grid.data[y * grid.cols + (x+1)];
    if (y < grid.rows - 1)
      n += grid.data[(y+1) * grid.cols + (x+1)];
  }

  return n;
}


/*** WINDOW SIZE ***/

int getWindowSize(int *cols, int *rows)
{
  write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12);
  write(STDOUT_FILENO, "\x1b[6n", 4);

  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);

  char buf[80];
  int at = 0;

  read(STDIN_FILENO, &buf[at], 1);
  if (buf[0] != '\x1b')
    return 1;
  read(STDIN_FILENO, &buf[++at], 1);
  if (buf[1] != '[')
    return -1;

  read(STDIN_FILENO, &buf[++at], 1);
  while (buf[at] != 'R')
    read(STDIN_FILENO, &buf[++at], 1);

  buf[at] = '\0';

  sscanf(&buf[2], "%d;%d", rows, cols);
    
  return 0;
}


/*** MAIN ***/

int main()
{

  enableRawMode();

  int cols, rows;
  getWindowSize(&cols, &rows);
  cols /= 2; // A block ocupates two characters of width
  
  setGrid(cols, rows, &grid);

  int cx = 0, cy = 0;
  char playing = 0;
  char _continue = 1;
  
  while (_continue)
  {
    appendBuffer("\x1b[2J", 4);
    appendBuffer("\x1b[H", 3);
    appendBuffer("\x1b[?25l", 6);
    if (playing) showGrid(-1, -1);
    else         showGrid(cx, cy);
    unsigned char buf[80];
    int buflen = snprintf(buf, 80, "\x1b[%d;%dH", cy + 1, cx + 1);
    appendBuffer(buf, buflen);
    
    showBuffer();


    struct Grid next = { 0, 0, NULL };
    setGrid(cols, rows, &next);

    if (playing)
    {
      for (int i = 0; i < rows; i++)
      {
	for (int j = 0; j < cols; j++)
	{
	  unsigned char neighbours = neighboursOf(j, i);
	  if (grid.data[i * cols + j])
	  {
	    // It is alive
	    next.data[i * cols + j] = (neighbours == 2 || neighbours == 3);
	  }
	  else
	  {
	    // It is dead
	    next.data[i * cols + j] = (neighbours == 3);
	  }
	}
      }
      grid = next;
    }
    
    
    unsigned char c;
    if (!read(STDIN_FILENO, &c, 1)) continue;

    
    switch (c)
      {
	case 'q':
	  _continue = 0;
	  break;
	case ' ':
	  playing = !playing;
	  break;
  
	case 'w':
	  cy -= (cy > 0);
	  break;
	case 's':
	  cy += (cy < rows - 1);
	  break;
	case 'a':
	  cx -= (cx > 0);
	  break;
	case 'd':
	  cx += (cx < cols - 1);
	  break;
	case 'e':
	  grid.data[cy * grid.cols + cx] = !grid.data[cy * grid.cols + cx];
	  break;
      }
  }
  
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);
  write(STDOUT_FILENO, "\x1b[?25h", 6);
  
  return 0;
}
