
/* uncomment the next line if colors should be used*/
#define NO_COLORS

#include "colors.h"

/* the signal number with wich dwm uses for click events */
#define SIG_CLICK 9

/* signals used to update the status bar */
#define SIG_VOLUME 10
#define SIG_BRIGHTNESS 11
#define SIG_POWER_PROFILE 12
#define SIG_KEYBOARD 13
#define SIG_NETWORK 14

/* the char to tell dwm a new block started*/
const char block_seperator = '\x1f';

/* text to show if no value is available */
static const char not_available[] = "n/a";

static const char toggle_vol[] = "amixer_wrapper set Master toggle";

static const block_t blocks[] = {
    /* format               function            argument      on click          interval   signal */
    {  " %s |",            disk_free,          "/",          NULL,             30,        0 },
    {  "  %s%% |",         ram_perc,           NULL,         NULL,             1,         0 },
    {  "  %s%% |",         cpu_perc,           NULL,         NULL,             1,         0 },
    {  " %s",               temp_icon,          "coretemp",   NULL,             5,         0 },
    {  " %s °C |",          temp,               "coretemp",   NULL,             5,         0 },
    {  " %s",               volume_icon_alsa,   "Master",     toggle_vol,       30,        SIG_VOLUME },
    {  " %s%% |",           volume_alsa,        "Master",     toggle_vol,       30,        SIG_VOLUME },
    {  " %s",               powerprofile_icon,  NULL,         "powerprofile",   30,        SIG_POWER_PROFILE },
    {  " %s ",              battery_icon,       "BAT0",       "powerprofile",   30,        0 },
    {  " %s%% |",           battery_perc,       "BAT0",       "powerprofile",   30,        0 },
    {  "  %s |",          keyboard_layout,    NULL,          NULL,             30,        SIG_KEYBOARD },
    {  "  %s |",          wifi_essid,         "wlo1",        "wifimenu",       30,        SIG_NETWORK },
    {  "%s |",              date,               "%F %T",      NULL,             1,         0 },
    {  "  ",               NULL,               NULL,         "powermenu",      0,         0 },
};