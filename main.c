#include <curses.h>
#include <stdio.h>
#include <stdlib.h>

#include "buffer.h"
#include "normal_mode.h"
#include "editor_modes.h"
#include "insert_mode.h"
#include "curses_util.h"
#include "keys.h"


int QUIT = 0;


int main() {
  int row, col;

  init_curses();

  getmaxyx(stdscr, row, col);

  Buffer buffer = {0};
  for (size_t i = 0; i < 1024; i++) {
    buffer.rows[i].contents = calloc(1024, sizeof(char));
  }

  buffer.row_cnt = 1;

  int ch = 0;
  int x, y;
  getyx(stdscr, y, x);

  while (ch != ctrl('q') && QUIT != 1) {
    print_mode_status(row);

    mvprintw(row - 2, 0, "row_cnt -> %d ", buffer.row_cnt);
    mvprintw(row - 3, 0, "x -> %d, y -> %d ", x, y);
    mvprintw(row - 4, 0, "row_size -> %d ", buffer.rows[buffer.row_index].size);
    mvprintw(row - 5, 0, "row_index -> %d ", buffer.row_index);

    for (size_t i = 0; i < buffer.row_cnt; i++) {
      mvprintw(i, 0, buffer.rows[i].contents);
    }

    move(y, x);
    ch = getch();

    if (get_current_mode() == NORMAL) {
      handle_normal_mode(ch, &buffer, &y, &x);
    } else if (get_current_mode() == INSERT) {
      handle_insert_mode(ch, &buffer, &y, &x);
    }
  }

  cleanup_curses();
  return 0;
}


