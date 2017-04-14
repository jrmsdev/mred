#include "inc/debug.h"
#include "inc/mred.h"

int DEBUG_ENABLED = 0;

void
debug_init (void)
{
	DEBUG_ENABLED = 1;
	atexit (debug_end);
}


void
debug_end (void)
{
}


void
debug (const char *fmt, ...)
{
	if (DEBUG_ENABLED)
	{
		va_list ap;
		va_start (ap, fmt);
		vfprintf(stderr, fmt, ap);
		va_end (ap);
	}
}
