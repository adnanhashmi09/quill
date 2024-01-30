#include<curses.h>
#include "editor_modes.h"

Mode mode = NORMAL;

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

Mode get_current_mode(){
  return mode;
}

void print_mode_status(int row) {
  mvprintw(row - 1, 0, "-- %s --", stringify_mode());
}
