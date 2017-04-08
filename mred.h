#ifndef __MRED_H
#define __MRED_H

#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

#define MRED_VERSION "0.0"

/* data */
typedef struct {
	int size;
	char *chars;
} edrow;

struct mred_config
{
	struct termios orig_termios;
	int screenrows;
	int screencols;
	int cx, cy; /* cursor position */
	int numrows;
	edrow *row;
};
struct mred_config ED;

/* append buffer */
struct abuf
{
	char *b;
	int len;
};

#define ABUF_INIT {NULL, 0}

/* main.c */
void die (const char *s);
void mred_init ();

/* terminal.c */
void enable_raw_mode ();
void disable_raw_mode ();
int mred_read_key ();
int get_window_size (int *rows, int *cols);
int get_cursor_position (int *rows, int *cols);

/* input.c */
#define CTRL_KEY(k) ((k) & 0x1f)
enum mred_key
{
	ARROW_LEFT = 1000,
	ARROW_RIGHT,
	ARROW_UP,
	ARROW_DOWN,
	DEL_KEY,
	HOME_KEY,
	END_KEY,
	PAGE_UP,
	PAGE_DOWN
};
void mred_process_keypress ();
void mred_move_cursor (int key);

/* output.c */
void mred_draw_rows (struct abuf *ab);
void mred_refresh_screen ();

/* buffer.c */
void ab_append (struct abuf *ab, const char *s, int len);
void ab_free (struct abuf *ab);

/* file_io.c */
void mred_open(char *filename);

/* row_ops.c */
void mred_append_row(char *s, size_t len);

#endif /* !__MRED_H */
