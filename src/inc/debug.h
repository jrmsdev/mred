#ifndef MRED_DEBUG_H
#define MRED_DEBUG_H

#include <stdarg.h>

void debug_init (void);
void debug_end (void);
void debug (const char *fmt, ...);

#endif /* !MRED_DEBUG_H */
