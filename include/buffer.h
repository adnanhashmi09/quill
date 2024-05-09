#ifndef BUFFER_H
#define BUFFER_H

#include <stdio.h>

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

void save_buffer_to_file(Buffer *buffer, char *file_name);

#endif // !BUFFER_H
