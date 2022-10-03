#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <limits.h>

#include "../util.h"

#define TOTAL(x) (x.user + x.nice + x.system + x.idle + x.iowait + x.irq + x.softirq)

#define LOAD(x) (x.user + x.nice + x.system + x.irq + x.softirq)

/* relevant info returned by `cat /proc/stat` */
typedef struct {
    uintmax_t user;
    uintmax_t nice;
    uintmax_t system;
    uintmax_t idle;
    uintmax_t iowait;
    uintmax_t irq;
    uintmax_t softirq;

} cpu_stat_t;

int cpu_stat(cpu_stat_t* cpu_stat)
{
    int res = pscanf(
        "/proc/stat", "%*s %ju %ju %ju %ju %ju %ju %ju", &cpu_stat->user, &cpu_stat->nice, &cpu_stat->system,
        &cpu_stat->idle, &cpu_stat->iowait, &cpu_stat->irq, &cpu_stat->softirq
    );

    if (res != 7)
        return -1;

    return 0;
}

int cpu_perc(const char* unused, char* bufer, size_t buffer_size)
{
    static cpu_stat_t a = { 0 };
    cpu_stat_t b;

    memcpy(&b, &a, sizeof(cpu_stat_t));

    int res = cpu_stat(&a);

    if (res < 0)
        return -1;

    if (b.user == 0) // this should only be zero the first time this function is called
        return -1;

    long double total = (long double)TOTAL(a) - (long double)TOTAL(b);

    if (total == 0)
        return -1;

    long double load = (long double)LOAD(a) - (long double)LOAD(b);

    return snprintf(bufer, buffer_size, "%02d", (int)(100 * load / total));
}