#include "../include/gap_buffer.h"
#include "../lib/include/tiny_test.h"
#include <malloc.h>
#include <stddef.h>
#include <stdint.h>

void test_new_gap_buffer_min_size() {
  GapBuffer buf = new_gap_buffer(5);
  assert_condition(buf.capacity == MIN_BUF_SIZE,
                   "Gap buffer size initialised to less than the minimum "
                   "allowed buffer size");
  free_buffer(&buf);
}

void test_new_gap_buffer_valid_size() {

  GapBuffer buf = new_gap_buffer(1024 * 4);
  assert_condition(buf.capacity == (1024 * 4),
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

void test_buffer_should_grow_when_inserted_characters_beyond_capacity() {
  GapBuffer buf = new_gap_buffer(MIN_BUF_SIZE);
  for (size_t i = 0; i < MIN_BUF_SIZE; i++) {
    insert_character(&buf, 'a');
  }

  assert_condition(buf.size == MIN_BUF_SIZE,
                   "Buf size is not equal to the number of characters entered, "
                   "ie, = MIN_BUF_SIZE");
  assert_condition(
      buf.capacity == MIN_BUF_SIZE,
      "Buf capacity should grow on the next insert, not the insert "
      "which fills the buffer");
  assert_condition(
      buf.cursor == buf.gap_end,
      "cursor and gap_end should be same as there is no gap left to insert.");

  bool did_grow = insert_character(&buf, 'b');
  assert_condition(
      did_grow == true,
      "insert_character() returned false, which means the buffer didn't grow");
  assert_condition(
      buf.capacity == (2 * MIN_BUF_SIZE),
      "Buf capacity is not equal to the twice the capacity, it didn't "
      "grow properly ");
  assert_condition(
      buf.size == MIN_BUF_SIZE + 1,
      "Buf size didn't increase by one when character was inserted");
  assert_condition(
      buf.gap_end == (2 * MIN_BUF_SIZE),
      "gap end not set properly. In this case it should be at the end");
  free_buffer(&buf);
}

void test_backspace_for_shrinking_buffer() {
  GapBuffer buf = new_gap_buffer(4 * 1024);
  for (size_t i = 0; i < 10; i++) {
    insert_character(&buf, 'a');
  }
  size_t prev_cursor = buf.cursor;
  size_t prev_size = buf.size;
  size_t prev_gap_end = buf.gap_end;
  size_t prev_capacity = buf.capacity;
  backspace(&buf);
  assert_condition(
      buf.cursor == prev_cursor - 1,
      "cursor didn't move to one space back when using backspace.");
  assert_condition(buf.size == prev_size - 1,
                   "size of buffer didn't reduce by 1");
  assert_condition(buf.gap_end == buf.capacity,
                   "gap_end should point to new_capacity - "
                   "number_of_chars_after_the_cursor ");
  assert_condition(buf.capacity == prev_capacity / 2,
                   "buffer capacity didn't get half of its original size.");
  assert_condition((malloc_usable_size(buf.buffer) >= buf.capacity &&
                    malloc_usable_size(buf.buffer) < prev_capacity),
                   "Buffer actual capacity didn't decrease");
  free_buffer(&buf);
}

void test_multiple_backspace_on_buffer_significantly_larger_than_required_size() {

  GapBuffer buf = new_gap_buffer(4 * MIN_BUF_SIZE);

  for (size_t i = 0; i < 10; i++) {
    insert_character(&buf, 'a');
  }
  size_t prev_cursor = buf.cursor;
  size_t prev_size = buf.size;
  size_t prev_gap_end = buf.gap_end;
  size_t prev_capacity = buf.capacity;
  backspace(&buf);
  backspace(&buf);

  assert_condition(
      buf.cursor == prev_cursor - 2,
      "cursor didn't move to two space back when using backspace twice.");
  assert_condition(buf.size == prev_size - 2,
                   "size of buffer didn't reduce by 2");
  assert_condition(buf.cursor == prev_cursor - 2, "curosr didn't go back by 2");
  assert_condition(buf.gap_end == buf.capacity,
                   "gap_end should point to new_capacity - "
                   "number_of_chars_after_the_cursor ");
  assert_condition(buf.capacity == prev_capacity / 4,
                   "buffer capacity didn't get 1/4th of its original size.");
  assert_condition((malloc_usable_size(buf.buffer) >= buf.capacity &&
                    malloc_usable_size(buf.buffer) < prev_capacity),
                   "Buffer actual capacity didn't decrease");

  backspace(&buf);
  backspace(&buf);
  assert_condition(buf.capacity == MIN_BUF_SIZE,
                   "Didn't reduce buf size to min buf_size when empty space "
                   "was significant.");

  free_buffer(&buf);
}

void test_delete_character_on_gap_end() {
  GapBuffer buf = new_gap_buffer(MIN_BUF_SIZE);
  for (size_t i = 0; i < MIN_BUF_SIZE; i++) {
    insert_character(&buf, 'a');
  }
  bool is_deleted = delete_character(&buf);
  assert_condition(is_deleted == false,
                   "Deleted the character even when the buf_end is at size");
  free_buffer(&buf);
}

void test_delete_character_on_overspaced_buffer() {
  GapBuffer buf = new_gap_buffer(3 * MIN_BUF_SIZE);
  for (size_t i = 0; i < MIN_BUF_SIZE / 2; i++) {
    insert_character(&buf, 'a');
  }
  size_t prev_cursor = buf.cursor;
  size_t left_iterations = 5;
  size_t prev_size = buf.size;

  for (size_t i = 0; i < left_iterations; i++) {
    move_cursor_left(&buf);
  }

  assert_condition(buf.cursor == prev_cursor - left_iterations,
                   "cursor didn't move left to the correct spot");

  for (size_t i = 0; i < left_iterations; i++) {
    delete_character(&buf);
  }

  assert_condition(buf.size == prev_size - left_iterations,
                   "size didn't update properly after multiple deletions");

  assert_condition(buf.capacity == (3 * MIN_BUF_SIZE) / 2,
                   "Capcity of buffer didn't shrink");

  assert_condition(
      buf.cursor == buf.size,
      "Cursor should be at position `size` after deleting all characters "
      "after the cursor");
  free_buffer(&buf);
}
