#include <stdio.h>
#include "buffer.h"
void save_buffer_to_file(Buffer *buffer, char *file_name) {
  FILE *file = fopen(file_name, "w");
  for (size_t i = 0; i < buffer->row_cnt; i++) {
    fwrite(buffer->rows[i].contents, buffer->rows[i].size, 1, file);
  }
  fclose(file);
}

