#include<curses.h>

#include "buffer.h"
#include "editor_modes.h"
#include "keys.h"

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
  Row *curr_row;
  switch (ch) {
  case 'j':
    if (*y >= buffer->row_cnt - 1)
      break;

    if (*x > buffer->rows[++buffer->row_index].size - 1) {
      curr_row = &buffer->rows[buffer->row_index];
      *x = curr_row->size - 1;
      char last_char = curr_row->contents[curr_row->size - 1];

      if (last_char == '\n')
        *x = *x > 0 ? *x - 1 : 0;
    }
    buffer->curr_pos = *x;
    move(*y + 1, *x);
    break;
  case 'k':
    if (*y <= 0)
      break;

    if (*x + 1 > buffer->rows[--buffer->row_index].size - 1) {
      if (buffer->rows[buffer->row_index].size <= 1) {
        *x = 0;
      } else {
        *x = buffer->rows[buffer->row_index].size - 2;
      }
    }
    buffer->curr_pos = *x;
    move(*y - 1, *x);
    break;
  case 'h':
    if (*x <= 0 && *y <= 0)
      break;

    if (*x <= 0) {
      curr_row = &buffer->rows[--buffer->row_index];
      *x = curr_row->size - 1;
      *x = *x > 0 ? *x - 1 : 0;
      buffer->curr_pos = *x;
      move(*y - 1, *x);
      break;
    }

    move(*y, *x - 1);
    buffer->curr_pos -= 1;
    break;
  case 'l':
    curr_row = &buffer->rows[buffer->row_index];
    if (*x >= curr_row->size - 1 && buffer->row_index == buffer->row_cnt - 1)
      break;
    else if (*x+1 >= curr_row->size - 1 &&
             buffer->row_index < buffer->row_cnt - 1) {
      *x = 0;
      *y += 1;
      buffer->row_index += 1;
      buffer->curr_pos = 0;
      move(*y, *x);
      break;
    }
    move(*y, *x + 1);
    buffer->curr_pos += 1;
    break;
  case ctrl('s'):
    save_buffer_to_file(buffer, "out.txt");
    break;
  }

  getyx(stdscr, *y, *x);
}
