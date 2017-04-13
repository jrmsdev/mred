#ifndef MRED_TERMINAL_H
#define MRED_TERMINAL_H

void enable_raw_mode ();
void disable_raw_mode ();
int mred_read_key ();
int get_window_size (int *rows, int *cols);
int get_cursor_position (int *rows, int *cols);

#endif /* !MRED_TERMINAL_H */
