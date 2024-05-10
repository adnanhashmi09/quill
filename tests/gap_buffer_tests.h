#ifndef GAP_BUFFER_TEST_H
#define GAP_BUFFER_TEST_H

void test_new_gap_buffer_min_size();
void test_new_gap_buffer_valid_size();
void test_move_cursor_left_at_zero();
void test_move_cursor_right_when_buffer_is_empty();
void test_insert_character_in_empty_buffer();
void test_buffer_should_grow_when_inserted_characters_beyond_capacity();
void test_backspace_for_shrinking_buffer();
void test_multiple_backspace_on_buffer_significantly_larger_than_required_size();
void test_delete_character_on_gap_end();
void test_delete_character_on_overspaced_buffer();

#endif // !GAP_BUFFER_TEST_H
