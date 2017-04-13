#ifndef MRED_H
#define MRED_H

#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

#include <stdio.h>
#include <termios.h>
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


#include "row_ops.h"


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

void mred_init ();
void mred_free ();
void die (const char *s);

#endif /* !MRED_H */
