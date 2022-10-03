#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "../util.h"

static result_t get_battery_percent(const char* battery, int* percent) 
{
    char path[PATH_MAX];
    int res = snprintf(path, PATH_MAX, "/sys/class/power_supply/%s/capacity", battery);

    if (res < 0)
        return RESULT_ERROR;

    int n = pscanf(path, "%d", percent);

    if (n != 1)
        return RESULT_ERROR;

    return RESULT_SUCCESS;
}

result_t battery_perc(const char* battery, char* buffer, size_t buffer_size) 
{
    int percent;
    result_t res = get_battery_percent(battery, &percent);

    if (res == RESULT_ERROR)
        return RESULT_ERROR;

    int n = snprintf(buffer, buffer_size, "%d", percent);
    return n < 0 ? RESULT_ERROR : RESULT_SUCCESS;
}

result_t battery_icon(const char* battery, char* buffer, size_t buffer_size) 
{
    int percent;
    result_t res = get_battery_percent(battery, &percent);

    if (res == RESULT_ERROR)
        return RESULT_ERROR;

    const char* icon;

    if (percent < 5)
        icon = "";
    else if (percent < 25)
        icon = "";
    else if (percent < 50)
        icon = "";
    else if (percent < 75)
        icon = "";
    else 
        icon = "";

    int n = snprintf(buffer, buffer_size, "%s", icon);
    return n < 0 ? RESULT_ERROR : RESULT_SUCCESS;
}