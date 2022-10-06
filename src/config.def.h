#include "colors.h"

/* the signal number with wich dwm uses for click events */
#define SIG_CLICK 9

/* the char to tell dwm a new block started*/
const char block_seperator = '\x1f';

/* text to show if no value is available */
static const char not_available[] = "n/a";

static const block_t blocks[] = {
    /* format           function        argument      on click    interval   signal */
    {  "%s",            date,           "%F %T",      NULL,       1,         0 },
};