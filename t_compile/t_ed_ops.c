#include "../src/terminal.c"
#include "../src/mred.c"
#include "../src/row_ops.c"
#include "../src/syntax_hl.c"
#include "../src/ed_ops.c"
#include <err.h>

void
t_insert_char (void)
{
	mred_insert_char ('a');
	if (ED.row[ED.cy].chars[0] != 'a')
		errx (1, "ED.row[ED.cy].chars[0] != 'a'");
	if (ED.cy != 0)
		errx (2, "ED.cy != 0");
	if (ED.cx != 1)
		errx (3, "ED.cx != 1");
}


void
t_del_char (void)
{
	mred_del_char ();
	if (ED.row[ED.cy].chars[0] != '\0')
		errx (4, "ED.row[ED.cy].chars[0] != '\\0'");
	if (ED.cy != 0)
		errx (5, "ED.cy != 0");
	if (ED.cx != 0)
		errx (6, "ED.cx != 0");
}


void
t_insert_newline (void)
{
	mred_insert_newline ();
	if (ED.cy != 1)
		errx (7, "ED.cy != 1");
	if (ED.cx != 0)
		errx (8, "ED.cx != 0");

	mred_insert_char ('b');
	if (ED.cx != 1)
		errx (9, "ED.cx != 1");

	mred_insert_newline ();
	if (ED.cy != 2)
		errx (10, "ED.cy != 2");
	if (ED.cx != 0)
		errx (11, "ED.cx != 0");
}


int
main (void)
{
	t_insert_char ();
	t_del_char ();
	t_insert_newline ();
	return (0);
}
