#include "../src/buffer.c"
#include <err.h>

void
t_abuf_init (struct abuf *ab)
{
	if (ab->b != NULL)
		errx (1, "ab.b != NULL");
	if (ab->len != 0)
		errx (2, "ab.len != 0");
}


void
t_ab_append (struct abuf *ab)
{
	ab_append (ab, "abc", 3);
	if (strncmp (ab->b, "abc", 3) != 0)
		errx (3, "ab->b != abc");
	if (ab->len != 3)
		errx (4, "ab->len != 3");
}


void
t_ab_free (struct abuf *ab)
{
	ab_free (ab);
	if (ab->b != NULL)
		errx (5, "ab->b != NULL");
	if (ab->len != 0)
		errx (6, "ab->len != 0");
}


int
main (void)
{
	struct abuf ab = ABUF_INIT;
	t_abuf_init (&ab);
	t_ab_append (&ab);
	t_ab_free (&ab);
	return (0);
}
