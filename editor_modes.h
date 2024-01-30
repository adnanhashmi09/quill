#ifndef EDITOR_MODES_H
#define EDITOR_MODES_H

typedef enum { NORMAL, INSERT } Mode;
void switch_to_normal_mode();
void switch_to_insert_mode();
char *stringify_mode();
Mode get_current_mode();
void print_mode_status(int row);

#endif // !EDITOR_MODES_H
