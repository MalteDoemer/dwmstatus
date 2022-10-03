#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

static void verr(const char* fmt, va_list ap)
{
    vfprintf(stderr, fmt, ap);

    if (fmt[0] && fmt[strlen(fmt) - 1] == ':') {
        fputc(' ', stderr);
        perror(NULL);
    } else {
        fputc('\n', stderr);
    }
}

void warn(const char* fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    verr(fmt, ap);
    va_end(ap);
}

void die(const char* fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    verr(fmt, ap);
    va_end(ap);

    exit(1);
}

int pscanf(const char* path, const char* fmt, ...)
{
    FILE* file = fopen(path, "r");

    if (!file) {
        warn("fopen '%s':", path);
        return -1;
    }

    va_list ap;
    va_start(ap, fmt);
    int res = vfscanf(file, fmt, ap);
    va_end(ap);

    fclose(file);
    return res;
}

int fmt_power_of_ten(uintmax_t num, int base, char* buffer, size_t buffer_size)
{
    const size_t prefix_len = 9;
    const char* prefix_1000[9] = { "B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB" };
    const char* prefix_1024[9] = { "B", "KiB", "MiB", "GiB", "TiB", "PiB", "EiB", "ZiB", "YiB" };

    const char** prefix = base == 1000 ? prefix_1000 : prefix_1024;

    size_t i;
    double scaled = num;
    for (i = 0; i < prefix_len && scaled >= base; i++)
        scaled /= base;

    return snprintf(buffer, buffer_size, "%.1f %s", scaled, prefix[i]);
}

size_t fread_trim(FILE* file, char* buffer, size_t buffer_size)
{
    size_t len = fread(buffer, 1, buffer_size - 1, file);
    buffer[len] = '\0';

    char* end = buffer + len - 1;
    while (end > buffer && isspace(*end))
        end--;
    end[1] = '\0';

    return len;
}

str2int_result_t str2int(int* out, char* s, int base)
{
    if (!s || s[0] == '\0' || isspace(s[0]))
        return STR2INT_INCONVERTIBLE;

    errno = 0;

    char* end;
    long l = strtol(s, &end, base);

    if (l > INT_MAX || (errno == ERANGE && l == LONG_MAX))
        return STR2INT_OVERFLOW;
    if (l < INT_MIN || (errno == ERANGE && l == LONG_MIN))
        return STR2INT_UNDERFLOW;

    if (*end != '\0')
        return STR2INT_INCONVERTIBLE;

    *out = l;
    return STR2INT_SUCCESS;
}
