#ifndef MRED_H
#define MRED_H

#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#define MRED_VERSION "0.3"

/* configurable data (?) */
#define MRED_TAB_STOP 8
#define MRED_QUIT_TIMES 2
#ifdef __ALLOW_NOTTY
#  define MRED_ALLOW_NOTTY 1
#else
#  define MRED_ALLOW_NOTTY 0
#endif


/* data */

struct mred_syntax {
	char *filetype;
	char **filematch;
	char **keywords;
	char *singleline_comment_start;
	char *multiline_comment_start;
	char *multiline_comment_end;
	int flags;
};

typedef struct {
	int idx;
	int size;
	int rsize;
	char *chars;
	char *render;
	unsigned char *hl;
	int hl_open_comment;
} edrow;

struct mred_config
{
	int stdin;
	int stdout;
	struct termios orig_termios;
	int screenrows;
	int screencols;
	int cx, cy; /* cursor position */
	int rx; /* render cursor position */
	int numrows;
	edrow *row;
	int rowoff;
	int coloff;
	char *filename;
	char statusmsg[80];
	time_t statusmsg_time;
	int dirty;
	struct mred_syntax *syntax;
};
struct mred_config ED;

enum mred_key
{
        BACKSPACE = 127,
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

enum mred_highlight {
	HL_NORMAL = 0,
	HL_COMMENT,
	HL_MLCOMMENT,
	HL_KEYWORD1,
	HL_KEYWORD2,
	HL_STRING,
	HL_NUMBER,
	HL_MATCH
};


/* append buffer */

struct abuf
{
	char *b;
	int len;
};
#define ABUF_INIT {NULL, 0}


/* mred.c */

void mred_init ();
void mred_free ();
void die (const char *s);


/* terminal.c */

void enable_raw_mode ();
void disable_raw_mode ();
int mred_read_key ();
int get_window_size (int *rows, int *cols);
int get_cursor_position (int *rows, int *cols);


/* row_ops.c */

void mred_insert_row (int at, char *s, size_t len);
void mred_update_row (edrow *row);
int mred_row_cx_to_rx (edrow *row, int cx);
void mred_row_insert_char (edrow *row, int at, int c);
void mred_row_del_char (edrow *row, int at);
void mred_del_row (int at);
void mred_free_row (edrow *row);
void mred_row_append_string (edrow *row, char *s, size_t len);
int mred_row_rx_to_cx (edrow *row, int rx);


/* syntax_hl.c */

void mred_update_syntax (edrow *row);
int mred_syntax_to_color (int hl);
void mred_select_syntax_hl ();

#endif /* !MRED_H */
