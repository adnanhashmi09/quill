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

void switch_to_normal_mode();
void switch_to_insert_mode();
char *stringify_mode();

void init_curses();
void cleanup_curses();
void print_mode_status(int row);
void handle_insert_mode(int ch, char *buf, size_t *buf_s, int *y, int *x);
void handle_normal_mode(int ch, char *buf, size_t buf_s, int *y, int *x);

int main() {
  int row, col;
  ESCDELAY = 25;

  init_curses();

  getmaxyx(stdscr, row, col);
  raw();
  keypad(stdscr, TRUE);
  noecho();

  char *buf = malloc(sizeof(char) * 1024);
  size_t buf_s = 0;

  int ch = 0;
  int x, y;
  getyx(stdscr, y, x);

  while (ch != ctrl('q') && QUIT != 1) {
    print_mode_status(row);
    move(y, x);
    ch = getch();

    if (mode == NORMAL) {
      handle_normal_mode(ch, buf, buf_s, &y, &x);
    } else if (mode == INSERT) {
      handle_insert_mode(ch, buf, &buf_s, &y, &x);
    }
  }

  cleanup_curses();
  return 0;
}

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
    case INSERT:
      return "INSERT";
  }
  return "NORMAL";
}

void init_curses() {
  initscr();
}

void cleanup_curses() {
  refresh();
  endwin();
}

void print_mode_status(int row) {
  mvprintw(row - 1, 0, "-- %s --", stringify_mode());
}

void handle_insert_mode(int ch, char *buf, size_t *buf_s, int *y, int *x) {
  if (ch == ESCAPE) {
    switch_to_normal_mode();
    return;
  }

  if (ch == BACKSPACE) {
    getyx(stdscr, *y, *x);
    move(*y, *x - 1);
    delch();
    buf[(*buf_s)--] = ' ';
  } else {
    addch(ch);
    buf[(*buf_s)++] = ch;
  }

  getyx(stdscr, *y, *x);
}

void handle_normal_mode(int ch, char *buf, size_t buf_s, int *y, int *x) {
  if (ch == INSERT_KEY) {
    switch_to_insert_mode();
    return;
  }

  switch (ch) {
    case 'j':
      getyx(stdscr, *y, *x);
      move(*y + 1, *x);
      break;
    case 'k':
      getyx(stdscr, *y, *x);
      move(*y - 1, *x);
      break;
    case 'h':
      getyx(stdscr, *y, *x);
      move(*y, *x - 1);
      break;
    case 'l':
      getyx(stdscr, *y, *x);
      move(*y, *x + 1);
      break;
  }

  if (ch == ctrl('s')) {
    FILE *file = fopen("out.txt", "w");
    fwrite(buf, buf_s, 1, file);
    fclose(file);
  }

  getyx(stdscr, *y, *x);
}
