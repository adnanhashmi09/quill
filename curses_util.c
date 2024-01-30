#include<curses.h>

void init_curses() {
  ESCDELAY = 25;
  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();
  // scrollok(stdscr, TRUE);
}

void cleanup_curses() {
  refresh();
  endwin();
}


