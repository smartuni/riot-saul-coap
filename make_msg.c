/**
 * via http://man7.org/linux/man-pages/man3/printf.3.html
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

char *
make_msg(const char *fmt, ...)
{
    int size = 0;
    char *p = NULL;
    va_list ap;

    /* Determine required size */

    va_start(ap, fmt);
    size = vsnprintf(p, size, fmt, ap);
    va_end(ap);

    if (size < 0)
	return NULL;

    size++;             /* For '\0' */
    p = malloc(size);
    if (p == NULL)
	return NULL;

    va_start(ap, fmt);
    size = vsnprintf(p, size, fmt, ap);
    va_end(ap);

    if (size < 0) {
	free(p);
	return NULL;
    }

    return p;
}
