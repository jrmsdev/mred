#ifndef MRED_H
#define MRED_H

#include <stdio.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>


#define MRED_VERSION "0.3"

/* configurable data (?) */

#define MRED_TAB_STOP 8
#define MRED_QUIT_TIMES 2


#include "row_ops.h"


struct mred_state
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
struct mred_state ED;

void mred_init ();
void mred_free ();
void die (const char *s);

#endif /* !MRED_H */
