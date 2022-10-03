#include <stdio.h>
#include <sys/statvfs.h>

#include "../util.h"

result_t disk_free(const char* path, char* buffer, size_t buffer_size)
{
    struct statvfs fs;

    if (statvfs(path, &fs) < 0) {
        warn("disk: '%s':", path);
        return RESULT_ERROR;
    }

    uintmax_t free = fs.f_frsize * fs.f_bavail;

    int n = fmt_power_of_ten(free, 1024, buffer, buffer_size);
    return n < 0 ? RESULT_ERROR : RESULT_SUCCESS;
}