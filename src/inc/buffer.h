#ifndef MRED_BUFFER_H
#define MRED_BUFFER_H

void ab_append (struct abuf *ab, const char *s, int len);
void ab_free (struct abuf *ab);

#endif /* !MRED_BUFFER_H */
