#include "../../include/gap_buffer.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int get_num_of_chars_before_cursor(GapBuffer *gap_buffer) {
  return (int)gap_buffer->cursor;
}

int get_num_of_chars_after_cursor(GapBuffer *gap_buffer) {
  return (int)(gap_buffer->capacity - gap_buffer->gap_end);
}

int get_amount_of_gap_buffer_used(GapBuffer *gap_buffer) {
  return (get_num_of_chars_before_cursor(gap_buffer) +
          get_num_of_chars_after_cursor(gap_buffer));
}

int cap_double_buffer_capacity(size_t capacity) {
  return (capacity > SIZE_MAX / 2) ? SIZE_MAX : 2 * capacity;
}

void move_memory_back(GapBuffer *buf, char *new_buf, size_t new_size) {
  int chars_to_copy = get_num_of_chars_after_cursor(buf);
  memmove(new_buf + new_size - chars_to_copy, buf->buffer + buf->gap_end,
          chars_to_copy);
}

bool grow_buffer(GapBuffer *gap_buffer, size_t new_capacity) {
  if (gap_buffer->capacity >= new_capacity)
    return false;

  new_capacity = MAX(MIN_BUF_SIZE, new_capacity);
  char *new_buffer = realloc(gap_buffer->buffer, new_capacity);
  if (new_buffer == NULL)
    return false;

  move_memory_back(gap_buffer, new_buffer, new_capacity);

  gap_buffer->buffer = new_buffer;
  gap_buffer->gap_end =
      new_capacity - get_num_of_chars_after_cursor(gap_buffer);
  gap_buffer->capacity = new_capacity;

  return true;
}

void shrink_buffer(GapBuffer *gap_buffer, size_t new_capacity) {
  if (new_capacity < get_amount_of_gap_buffer_used(gap_buffer))
    return;

  new_capacity = MAX(new_capacity, MIN_BUF_SIZE);
  move_memory_back(gap_buffer, gap_buffer->buffer, new_capacity);
  char *new_buffer = realloc(gap_buffer->buffer, new_capacity);

  if (new_buffer == NULL)
    return;

  gap_buffer->buffer = new_buffer;
  gap_buffer->gap_end =
      new_capacity - get_num_of_chars_after_cursor(gap_buffer);
  gap_buffer->capacity = new_capacity;
}

GapBuffer new_gap_buffer(size_t init_size) {
  GapBuffer gap_buffer = {};
  gap_buffer.cursor = 0;
  gap_buffer.size = 0;

  char *buffer = malloc(MAX(init_size, MIN_BUF_SIZE));
  if (!buffer) {
    gap_buffer.capacity = 0;
    gap_buffer.gap_end = 0;
    gap_buffer.buffer = NULL;
  }
  gap_buffer.capacity = MAX(init_size, MIN_BUF_SIZE);
  gap_buffer.gap_end = init_size;
  gap_buffer.buffer = buffer;

  return gap_buffer;
}

bool move_cursor_left(GapBuffer *gap_buffer) {
  // 1234 [|___] 6789
  // 123 [|___] 46789

  if (gap_buffer->cursor == 0)
    return false;

  gap_buffer->buffer[--gap_buffer->gap_end] =
      gap_buffer->buffer[--gap_buffer->cursor];
  return true;
}

bool move_cursor_right(GapBuffer *gap_buffer) {
  // 1234 [|___] 6789
  // 12346 [|___] 789
  if (gap_buffer->cursor == gap_buffer->size)
    return false;

  gap_buffer->buffer[gap_buffer->cursor++] =
      gap_buffer->buffer[gap_buffer->gap_end++];
  return true;
}

void free_buffer(GapBuffer *gap_buffer) {
  if (!gap_buffer->buffer)
    return;

  free(gap_buffer->buffer);
  gap_buffer->size = 0;
  gap_buffer->capacity = 0;
  gap_buffer->gap_end = 0;
  gap_buffer->cursor = 0;
}

bool backspace(GapBuffer *gap_buffer) {
  if (gap_buffer->cursor < 0) {
    return false;
  }
  gap_buffer->cursor--;
  if (get_amount_of_gap_buffer_used(gap_buffer) < gap_buffer->capacity / 4) {
    shrink_buffer(gap_buffer, gap_buffer->capacity / 2);
  }
  gap_buffer->size -= 1;
  return true;
}

bool delete_character(GapBuffer *gap_buffer) {
  if (gap_buffer->cursor == gap_buffer->size) {
    return false;
  }
  gap_buffer->gap_end++;
  if (get_amount_of_gap_buffer_used(gap_buffer) < gap_buffer->capacity / 4) {
    shrink_buffer(gap_buffer, gap_buffer->capacity / 2);
  }
  gap_buffer->size -= 1;
  return true;
}

bool insert_character(GapBuffer *gap_buffer, char c) {
  if (gap_buffer->gap_end == gap_buffer->cursor) {
    size_t new_buf_size = cap_double_buffer_capacity(gap_buffer->capacity);
    bool grow_success = grow_buffer(gap_buffer, new_buf_size);
    if (!grow_success)
      return false;
  }

  gap_buffer->buffer[gap_buffer->cursor++] = c;
  gap_buffer->size += 1;
  return true;
}
