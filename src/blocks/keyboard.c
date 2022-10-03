#include <stdio.h>
#include <string.h>

#include "../util.h"

#define CONTENT_LENGTH 16

result_t keyboard_layout(const char* unused, char* buffer, size_t buffer_size)
{
    const char* command = "setxkbmap -query | awk '/layout/{ print $2 }'";

    FILE* proc = popen(command, "r");

    if (!proc) {
        warn("keyboard_layout:");
        return RESULT_ERROR;
    }

    char layout[CONTENT_LENGTH];
    fread_trim(proc, layout, CONTENT_LENGTH);
    pclose(proc);

    int n = snprintf(buffer, buffer_size, "%s", layout);
    return n < 0 ? RESULT_ERROR : RESULT_SUCCESS;
}