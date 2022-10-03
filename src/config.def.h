
/* uncomment the next line if colors should be used*/
#define NO_COLORS

#include "colors.h"

/* text to show if no value is available */
static const char not_available[] = "n/a";

/* the status line format string */
static const char status_line[] = "%s";

static const block_t blocks[] = {
    /* function - argument - interval - signal */
    {  date,      "%F %T",   1,         0},
};