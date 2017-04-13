#ifndef MRED_FILE_IO_H
#define MRED_FILE_IO_H

void mred_open (char *filename);
char * mred_rows_to_string (int *buflen);
void mred_save ();

#endif /* !MRED_FILE_IO_H */
