
/* uncomment the next line if colors should be used*/
#define NO_COLORS

#include "colors.h"

/* this signal is used by dwm when the bar is clicked */
#define SIG_CLICK 9

/* text to show if no value is available */
static const char not_available[] = "n/a";

static const block_t blocks[] = {
    /*format            function             argument      interval   signal */
    { " {} |",         disk_free,           "/",          30,        0 },
    { " {}% |",        ram_perc,            NULL,         1,         0 },
    { " {}% |",        cpu_perc,            NULL,         1,         0 },
    { "{}",             date,                "%F %T",      1,         0 },
};