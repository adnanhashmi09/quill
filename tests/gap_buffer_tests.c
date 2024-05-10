#include "../include/gap_buffer.h"
#include "../lib/include/tiny_test.h"

void test_new_gap_buffer_min_size() {
  GapBuffer buf = new_gap_buffer(5);
  assert_condition(buf.capacity == MIN_BUF_SIZE,
                   "Gap buffer size initialised to less than the minimum "
                   "allowed buffer size");
  free_buffer(&buf);
}

void test_new_gap_buffer_valid_size() {

  GapBuffer buf = new_gap_buffer(1024 * 4);
  assert_condition(buf.capacity == (1024 * 3),
                   "Gap buffer size initialised incorrectly. "
                   "allowed buffer size");
  free_buffer(&buf);
}

void test_move_cursor_left_at_zero() {
  GapBuffer buf = new_gap_buffer(5);
  bool has_moved = move_cursor_left(&buf);
  assert_condition(has_moved == false,
                   "move_cursor_left didn't return false when cursor was at 0");
  free_buffer(&buf);
}

void test_move_cursor_right_when_buffer_is_empty() {
  GapBuffer buf = new_gap_buffer(5);
  bool has_moved = move_cursor_right(&buf);
  assert_condition(
      has_moved == false,
      "move_cursor_right didn't return false when buffer was empty. The cursor "
      "shouldn't move right when there are no characters in the buffer.");
  free_buffer(&buf);
}

void test_insert_character_in_empty_buffer() {
  GapBuffer buf = new_gap_buffer(5);
  insert_character(&buf, 'a');
  insert_character(&buf, 'b');
  insert_character(&buf, 'c');
  insert_character(&buf, 'd');

  assert_condition(buf.size == 4,
                   "Buffer size is not equal to numbe of characters");
  assert_condition(buf.capacity == MIN_BUF_SIZE,
                   "Buffer capacity changed unexpectedly");
  assert_condition(buf.cursor == 4,
                   "Buffer cursor not at the right position, ie, it should be "
                   "one space ahead of the last character entered");
  free_buffer(&buf);
}
