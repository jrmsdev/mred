#include "../src/terminal.c"
#include "../src/mred.c"
#include "../src/syntax_hl.c"
#include "../src/row_ops.c"
#include <err.h>

void
t_insert_row ()
{
	mred_insert_row (0, "", 0);

	if (ED.row == NULL)
		errx (1, "ED.row == NULL");
	if (ED.numrows != 1)
		errx (2, "ED.numrows != 1");

}


int
main (void)
{
	enable_raw_mode();
	mred_init ();
	t_insert_row ();
	return (0);
}
