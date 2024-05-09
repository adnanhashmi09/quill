#include <curses.h>

#include "../../include/buffer.h"
#include "../../include/editor_modes.h"
#include "../../include/keys.h"

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
