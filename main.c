#include <curses.h>
#include <stdio.h>
#include <stdlib.h>

#define ctrl(x) ((x) & 0x1f)
#define ESCAPE 27
#define BACKSPACE 263
#define INSERT_KEY 105
#define ENTER 10

typedef enum { NORMAL, INSERT } Mode;

typedef struct {
  char *contents;
  size_t index;
  size_t size;
} Row;

typedef struct {
  char *buf;
  Row rows[1024];
  size_t row_index;
  size_t curr_pos;
  size_t row_cnt;
} Buffer;

Mode mode = NORMAL;
int QUIT = 0;

void switch_to_normal_mode();
void switch_to_insert_mode();
char *stringify_mode();

void init_curses();
void cleanup_curses();
void print_mode_status(int row);
void handle_insert_mode(int ch, Buffer *buffer, int *y, int *x);
void handle_normal_mode(int ch, Buffer *buffer, int *y, int *x);
void save_buffer_to_file(Buffer *buffer, char *file_name);

int main() {
  int row, col;
  ESCDELAY = 25;

  init_curses();

  getmaxyx(stdscr, row, col);
  raw();
  keypad(stdscr, TRUE);
  noecho();

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

    for (size_t i = 0; i < buffer.row_cnt; i++) {
      mvprintw(i, 0, buffer.rows[i].contents);
    }

    move(y, x);
    ch = getch();

    if (mode == NORMAL) {
      handle_normal_mode(ch, &buffer, &y, &x);
    } else if (mode == INSERT) {
      handle_insert_mode(ch, &buffer, &y, &x);
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
  // scrollok(stdscr, TRUE);
}

void cleanup_curses() {
  refresh();
  endwin();
}

void print_mode_status(int row) {
  mvprintw(row - 1, 0, "-- %s --", stringify_mode());
}

void handle_insert_mode(int ch, Buffer *buffer, int *y, int *x) {

  Row *curr;
  FILE *file;
  switch (ch) {
  case ESCAPE:
    switch_to_normal_mode();
    break;

  case BACKSPACE:
    getyx(stdscr, *y, *x);
    if (buffer->curr_pos == 0 && buffer->row_cnt > 1) {
      curr = &buffer->rows[--buffer->row_index];
      buffer->row_cnt -= 1;
      curr->contents[curr->size--] = ' ';
      buffer->curr_pos = curr->size;
      move(buffer->row_index, curr->size);
    } else if (buffer->row_cnt >= 1 && buffer->curr_pos > 0) {
      curr = &buffer->rows[buffer->row_index];
      curr->contents[--buffer->curr_pos] = ' ';
      curr->size = buffer->curr_pos;
      move(*y, buffer->curr_pos);
    }
    break;

  case ENTER:
    curr = &buffer->rows[buffer->row_index];
    curr->contents[buffer->curr_pos++] = '\n';
    curr->size = buffer->curr_pos;

    buffer->row_index++;
    buffer->curr_pos = 0;
    buffer->row_cnt += 1;
    move(buffer->row_index, buffer->curr_pos);
    break;

  case ctrl('s'):
    save_buffer_to_file(buffer, "out.txt");
    break;

  default:
    curr = &buffer->rows[buffer->row_index];
    curr->contents[buffer->curr_pos++] = ch;
    curr->size = buffer->curr_pos;
    move(*y, buffer->curr_pos);
    break;
  }

  getyx(stdscr, *y, *x);
}

void save_buffer_to_file(Buffer *buffer, char *file_name) {
  FILE *file = fopen(file_name, "w");
  for (size_t i = 0; i < buffer->row_cnt; i++) {
    fwrite(buffer->rows[i].contents, buffer->rows[i].size, 1, file);
  }
  fclose(file);
}

void handle_normal_mode(int ch, Buffer *buffer, int *y, int *x) {
  if (ch == INSERT_KEY) {
    switch_to_insert_mode();
    return;
  }

  int row, col;
  getmaxyx(stdscr, row, col);
  getyx(stdscr, *y, *x);

  /*! TODO: Handle x position while moving with j and k.
   *
   * @todo Cure my dementia.
   *  Currently I am moving to where \n character is. Editing from there will
   * overwrite newline So we need to push the last character to the end
   */

  switch (ch) {
  case 'j':
    if (*y >= buffer->row_cnt - 1)
      break;

    if (*x > buffer->rows[++buffer->row_index].size) {
      Row *curr_row = &buffer->rows[buffer->row_index];
      *x = curr_row->size;
      char last_char = curr_row->contents[curr_row->size];

      if (last_char == '\n')
        *x -= 1;
    }
    buffer->curr_pos = *x;
    move(*y + 1, *x);
    break;
  case 'k':
    if (*y <= 0)
      break;

    if (*x > buffer->rows[--buffer->row_index].size) {
      *x = buffer->rows[buffer->row_index].size - 1;
    }
    buffer->curr_pos = *x;
    move(*y - 1, *x);
    break;
  case 'h':
    if (*x <= 0)
      break;

    move(*y, *x - 1);
    buffer->curr_pos -= 1;
    break;
  case 'l':
    move(*y, *x + 1);
    buffer->curr_pos += 1;
    break;
  case ctrl('s'):
    save_buffer_to_file(buffer, "out.txt");
    break;
  }

  getyx(stdscr, *y, *x);
}
