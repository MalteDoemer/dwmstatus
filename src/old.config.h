
#include "colors.h"

/* this signal is used by dwm when the bar is clicked */
#define SIG_CLICK 9

/* a special char to tell dwm when a new block begins */
#define SEP "\x1f"

#define SIG_VOLUME 10
#define SIG_BRIGHTNESS 11
#define SIG_POWER_PROFILE 12
#define SIG_KEYBOARD 13
#define SIG_NETWORK 14

/* text to show if no value is available */
static const char not_available[] = "n/a";

/* the status line format string */
static const char status_line[] = " %s |"SEP
                                  "  %s%% |"SEP
                                  "  %s%% |"SEP
                                  " %s %s°C |"SEP
                                  " %s %s%% |"SEP
                                  " %s"SEP
                                  " %s %s%% |"SEP
                                  "  %s |"SEP
                                  "  %s |"SEP
                                  " %s";

static const block_t blocks[] = {
    /* function             argument      interval   signal */
    {  disk_free,           "/",          30,        0 },
    {  ram_perc,            NULL,         1,         0 },
    {  cpu_perc,            NULL,         1,         0 },
    {  temp_icon,           "coretemp",   5,         0 },
    {  temp,                "coretemp",   5,         0 },
    {  volume_icon_alsa,    "Master",     30,        SIG_VOLUME },
    {  volume_alsa,         "Master",     30,        SIG_VOLUME },
    {  powerprofile_icon,   NULL,         30,        SIG_POWER_PROFILE },
    {  battery_icon,        "BAT0",       30,        0 },
    {  battery_perc,        "BAT0",       30,        0 },
    {  keyboard_layout,     NULL,         30,        SIG_KEYBOARD },
    {  wifi_essid,          "wlo1",       30,        SIG_NETWORK },
    {  date,                "%F %T",      1,         0 },
};