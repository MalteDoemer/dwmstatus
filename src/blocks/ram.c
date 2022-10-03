#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../util.h"

/* relevant info returned by `cat /proc/meninfo` */
typedef struct {
    uintmax_t total;
    uintmax_t free;
    uintmax_t avail;
    uintmax_t buffers;
    uintmax_t cached;
} meminfo_t;

static result_t meminfo(meminfo_t* info)
{
    int res = pscanf(
        "/proc/meminfo",
        "MemTotal: %ju kB\n"
        "MemFree: %ju kB\n"
        "MemAvailable: %ju kB\n"
        "Buffers: %ju kB\n"
        "Cached: %ju kB\n",
        &info->total, &info->free, &info->avail, &info->buffers, &info->cached
    );

    if (res != 5)
        return RESULT_ERROR;

    // everything is reported in kB
    info->total *= 1024;
    info->free *= 1024;
    info->avail *= 1024;
    info->buffers *= 1024;
    info->cached *= 1024;

    return RESULT_SUCCESS;
}

result_t ram_free(const char* unused, char* buffer, size_t buffer_size)
{
    meminfo_t info;
    result_t res = meminfo(&info);

    if (res == RESULT_ERROR)
        return RESULT_ERROR;

    int n = fmt_power_of_ten(info.free, 1024, buffer, buffer_size);
    return n < 0 ? RESULT_ERROR : RESULT_SUCCESS;
}

result_t ram_avail(const char* unused, char* buffer, size_t buffer_size)
{
    meminfo_t info;
    result_t res = meminfo(&info);

    if (res == RESULT_ERROR)
        return RESULT_ERROR;

    int n = fmt_power_of_ten(info.avail, 1024, buffer, buffer_size);
    return n < 0 ? RESULT_ERROR : RESULT_SUCCESS;
}

int ram_perc(const char* unused, char* buffer, size_t buffer_size)
{
    meminfo_t info;
    result_t res = meminfo(&info);

    if (res == RESULT_ERROR)
        return RESULT_ERROR;

    uintmax_t used = info.total - info.free - info.buffers - info.cached;

    int n = snprintf(buffer, buffer_size, "%02ld", 100 * used / info.total);
    return n < 0 ? RESULT_ERROR : RESULT_SUCCESS;
}

int ram_total(const char* unused, char* buffer, size_t buffer_size)
{
    meminfo_t info;
    result_t res = meminfo(&info);

    if (res == RESULT_ERROR)
        return RESULT_ERROR;

    int n = fmt_power_of_ten(info.total, 1024, buffer, buffer_size);
    return n < 0 ? RESULT_ERROR : RESULT_SUCCESS;
}

int ram_used(const char* unused, char* buffer, size_t buffer_size)
{
    meminfo_t info;
    result_t res = meminfo(&info);

    if (res == RESULT_ERROR)
        return RESULT_ERROR;

    uintmax_t used = info.total - info.free - info.buffers - info.cached;

    int n = fmt_power_of_ten(used, 1024, buffer, buffer_size);
    return n < 0 ? RESULT_ERROR : RESULT_SUCCESS;
}
