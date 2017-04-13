#ifndef MRED_ROW_OPS_H
#define MRED_ROW_OPS_H

void mred_insert_row (int at, char *s, size_t len);
void mred_update_row (edrow *row);
int mred_row_cx_to_rx (edrow *row, int cx);
void mred_row_insert_char (edrow *row, int at, int c);
void mred_row_del_char (edrow *row, int at);
void mred_del_row (int at);
void mred_free_row (edrow *row);
void mred_row_append_string (edrow *row, char *s, size_t len);
int mred_row_rx_to_cx (edrow *row, int rx);

#endif /* !MRED_ROW_OPS_H */
