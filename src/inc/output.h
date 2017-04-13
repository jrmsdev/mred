#ifndef MRED_OUTPUT_H
#define MRED_OUTPUT_H

void mred_draw_rows (struct abuf *ab);
void mred_refresh_screen ();
void mred_scroll ();
void mred_draw_status_bar (struct abuf *ab);
void mred_set_status_message (const char *fmt, ...);
void mred_draw_message_bar (struct abuf *ab);

#endif /* !MRED_OUTPUT_H */
