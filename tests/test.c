#include "../lib/include/tiny_test.h"
#include "./gap_buffer_tests.h"
int main() {
  addTest("Test new_gap_buffer() when invalid size is given",
          &test_new_gap_buffer_min_size);
  addTest("Test new_gap_buffer() when valid size is given",
          &test_new_gap_buffer_valid_size);
  addTest("Test cursor_move_right() when buffer is empty",
          &test_move_cursor_right_when_buffer_is_empty);
  addTest("Test cursor_move_left() when cursor is at 0",
          &test_move_cursor_left_at_zero);
  addTest("Test insert_character() function",
          &test_insert_character_in_empty_buffer);
  runTests();
}
