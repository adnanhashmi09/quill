#ifndef GAP_BUFFER_H
#define GAP_BUFFER_H

#include <stdio.h>
#include <stdbool.h>

#define MIN_BUF_SIZE 1024
#define MAX(a, b) ((a > b) ? a : b)

typedef struct {
  char *buffer;
  size_t cursor;
  size_t gap_end;
  size_t size;
  size_t capacity;
} GapBuffer;

GapBuffer new_gap_buffer(size_t init_size);
bool move_cursor_left(GapBuffer *gap_buffer);
bool move_cursor_right(GapBuffer *gap_buffer);
bool backspace(GapBuffer *gap_buffer);
bool delete_character(GapBuffer *gap_buffer);
bool insert_character(GapBuffer *gap_buffer, char c);
void free_buffer(GapBuffer *gap_buffer);

#endif // !GAP_BUFFER_H
