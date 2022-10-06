
#include "colors.h"

#define SIG_VOLUME 10
#define SIG_BRIGHTNESS 11
#define SIG_POWER_PROFILE 12
#define SIG_KEYBOARD 13
#define SIG_NETWORK 14

/* text to show if no value is available */
static const char not_available[] = "n/a";

/* the status line format string */
static const char status_line[] =  " %s |  %s%% |  %s%% | %s %s°C | %s %s%% | %s %s %s%% |  %s |  %s | %s";

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