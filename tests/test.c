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
  addTest("insert_character() should grow buffer when inserted beyond limit",
          &test_buffer_should_grow_when_inserted_characters_beyond_capacity);
  addTest("backspace() should shrink buffer when one fourth of the capacity is "
          "being used",
          &test_backspace_for_shrinking_buffer);
  addTest(
      "backspace() ran multiple times should shrink appropriately if empty "
      "buffer is still significantly larger."
      "being used",
      &test_multiple_backspace_on_buffer_significantly_larger_than_required_size);
  addTest("Test delete_character() when at the end of the buffer",
          &test_delete_character_on_gap_end);
  addTest("Test delete_character() on overspaced buffer",
          &test_delete_character_on_overspaced_buffer);
  runTests();
}
