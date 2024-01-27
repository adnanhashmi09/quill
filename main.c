#include <curses.h>
#include <stdio.h>
#include <stdlib.h>

#define ctrl(x) ((x) & 0x1f)
#define ESCAPE 27
#define BACKSPACE 263
#define INSERT_KEY 105

typedef enum { NORMAL, INSERT } Mode;

Mode mode = NORMAL;
int QUIT = 0;

void switch_to_normal_mode() {
  if (mode == INSERT) {
    mode = NORMAL;
    return;
  }

  mode = NORMAL;
}

void switch_to_insert_mode() {
  if (mode == NORMAL) {
    mode = INSERT;
    return;
  }
}
char *stringify_mode() {
  switch (mode) {
  case NORMAL:
    return "NORMAL";
    break;
  case INSERT:
    return "INSERT";
    break;
  }

  return "NORMAL";
}

int main() {
  int row, col;
  // might break how arrow keys work.
  // https://stackoverflow.com/questions/34101477/remove-the-delay-on-getch-function
  ESCDELAY = 25;

  initscr();

  getmaxyx(stdscr, row, col);
  raw();
  keypad(stdscr, TRUE);
  noecho();

  mvprintw(row - 1, 0, "-- %s --", stringify_mode());
  move(0, 0);

  char *buf = malloc(sizeof(char) * 1024);
  size_t buf_s = 0;

  int ch = 0;
  int x, y;
  getyx(stdscr, y, x);

  while (ch != ctrl('q') && QUIT != 1) {
    mvprintw(row - 1, 0, "-- %s --", stringify_mode());
    move(y, x);
    ch = getch();

    if (mode == NORMAL) {
      if (ch == INSERT_KEY) {
        switch_to_insert_mode();
        continue;
      }
    } else if (mode == INSERT) {
      if (ch == ESCAPE) {
        switch_to_normal_mode();
        continue;
      }
    }

    if (mode == INSERT) {
      if (ch == BACKSPACE) {
        getyx(stdscr, y, x);
        move(y, x - 1);
        delch();
        buf[buf_s--] = ' ';
      } else {
        addch(ch);
        buf[buf_s++] = ch;
      }
      getyx(stdscr, y, x);

    } else if (mode == NORMAL) {

      switch (ch) {
      case 'j':
        getyx(stdscr, y, x);
        move(y + 1, x);
        break;
      case 'k':
        getyx(stdscr, y, x);
        move(y - 1, x);
        break;
      case 'h':
        getyx(stdscr, y, x);
        move(y, x - 1);
        break;
      case 'l':
        getyx(stdscr, y, x);
        move(y, x + 1);
        break;
      }

      if(ch == ctrl('s')){
        FILE *file = fopen("out.txt", "w");
        fwrite(buf, buf_s, 1, file);
        fclose(file);
      }
      getyx(stdscr, y, x);
    }
  }

  refresh();
  endwin();

  return 0;
}
