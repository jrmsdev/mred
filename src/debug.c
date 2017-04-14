#include "inc/debug.h"
#include "inc/mred.h"

void
debug_init (void)
{
	atexit (debug_end);
}


void
debug_end (void)
{
}


void
debug (const char *fmt, ...)
{
	va_list ap;
	va_start (ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end (ap);
}
