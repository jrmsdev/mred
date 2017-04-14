#include "inc/debug.h"
#include "inc/mred.h"

int DEBUG_ENABLED = 0;

void
debug_init (void)
{
	DEBUG_ENABLED = 1;
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

void
debugln (const char *fmt, ...)
{
	if (DEBUG_ENABLED)
	{
		va_list ap;
		va_start (ap, fmt);
		vfprintf(stderr, fmt, ap);
		fprintf(stderr, "\n");
		va_end (ap);
	}
}
