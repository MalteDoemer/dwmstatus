#include <stdio.h>
#include <time.h>

#include "../util.h"

result_t date(const char* arg, char* buffer, size_t buffer_size)
{
    time_t t = time(NULL);

    // strftime copies the null terminator from the format string
    size_t res = strftime(buffer, buffer_size, arg, localtime(&t));

    if (res == 0) {
        warn("date: overflow");
        return RESULT_ERROR;
    }

    return RESULT_SUCCESS;
}
