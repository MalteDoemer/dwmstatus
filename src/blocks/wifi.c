#include <assert.h>
#include <ifaddrs.h>
#include <limits.h>
#include <linux/wireless.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../util.h"

#define CONTENT_SIZE 512

typedef enum wifi_state_t {
    WIFI_UP,
    WIFI_DOWN,
} wifi_state_t;

static result_t get_wifi_state(const char* interface, wifi_state_t* state)
{
    char path[PATH_MAX];
    int res = snprintf(path, PATH_MAX, "/sys/class/net/%s/operstate", interface);

    if (res < 0)
        return RESULT_ERROR;

    FILE* file = fopen(path, "r");

    if (!file) {
        warn("wifi: '%s':", path);
        return RESULT_ERROR;
    }

    char status[5];
    fread_trim(file, status, 5);
    fclose(file);

    if (!strcmp(status, "up"))
        *state = WIFI_UP;
    else if (!strcmp(status, "down"))
        *state = WIFI_DOWN;
    else {
        warn("wifi: unable to parse output: '%s'", status);
        return RESULT_ERROR;
    }

    return RESULT_SUCCESS;
}

static result_t get_wifi_perc(const char* interface, int* percent)
{
    const char* path = "/proc/net/wireless";
    FILE* file = fopen(path, "r");

    if (!file) {
        warn("wifi: '%s':", path);
        return RESULT_ERROR;
    }

    char line_buffer[CONTENT_SIZE];
    for (int i = 0; i < 3; i++) {
        if (!fgets(line_buffer, CONTENT_SIZE, file)) {
            fclose(file);
            warn("wifi: unable to parse file: '%s'", path);
            return RESULT_ERROR;
        }
    }

    fclose(file);

    const char* scan_start = strstr(line_buffer, interface);

    if (!scan_start) {
        printf("%s\n", line_buffer);
        warn("wifi: unable to parse file: '%s'", path);
        return RESULT_ERROR;
    }

    assert(strlen(scan_start) >= strlen(interface) + 1);

    scan_start = scan_start + strlen(interface) + 1;

    int current;
    int res = sscanf(scan_start, "%*d %d", &current);

    if (res != 1) {
        warn("wifi: unable to parse file: '%s'", path);
        return RESULT_ERROR;
    }

    // /proc/net/wireless reports values between 0 - 70

    *percent = (int)((float)current / 70.0f * 100.0f);

    return RESULT_SUCCESS;
}

result_t wifi_perc(const char* interface, char* buffer, size_t buffer_size)
{

    wifi_state_t state;
    int percent;

    result_t res1 = get_wifi_state(interface, &state);

    if (res1 == RESULT_ERROR || state == WIFI_DOWN)
        return RESULT_ERROR;

    result_t res2 = get_wifi_perc(interface, &percent);

    if (res2 == RESULT_ERROR)
        return RESULT_ERROR;

    int n = snprintf(buffer, buffer_size, "%d", percent);
    return n < 0 ? RESULT_ERROR : RESULT_SUCCESS;
}

result_t wifi_bars(const char* interface, char* buffer, size_t buffer_size)
{

    wifi_state_t state;
    int percent;

    result_t res1 = get_wifi_state(interface, &state);

    if (res1 == RESULT_ERROR || state == WIFI_DOWN)
        return RESULT_ERROR;

    result_t res2 = get_wifi_perc(interface, &percent);

    if (res2 == RESULT_ERROR)
        return RESULT_ERROR;

    const char* bars;

    if (percent < 25) {
        bars = "▂";
    } else if (percent < 50) {
        bars = "▂▄";
    } else if (percent < 75) {
        bars = "▂▄▆";
    } else {
        bars = "▂▄▆█";
    }

    int n = snprintf(buffer, buffer_size, "%s", bars);
    return n < 0 ? RESULT_ERROR : RESULT_SUCCESS;
}

result_t wifi_essid(const char* interface, char* buffer, size_t buffer_size)
{
    struct iwreq wreq;
    char essid[IW_ESSID_MAX_SIZE + 1];
    int res;

    memset(&wreq, 0, sizeof(struct iwreq));
    wreq.u.essid.length = IW_ESSID_MAX_SIZE + 1;

    res = snprintf(wreq.ifr_name, sizeof(wreq.ifr_name), "%s", interface);

    if (res < 0)
        return RESULT_ERROR;

    int socketfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (socketfd < 0) {
        warn("socket:");
        return RESULT_ERROR;
    }

    wreq.u.essid.pointer = essid;

    res = ioctl(socketfd, SIOCGIWESSID, &wreq);
    close(socketfd);

    if (res < 0) {
        warn("ioctl:");
        return RESULT_ERROR;
    }

    if (!strcmp(essid, ""))
        return RESULT_ERROR;

    res = snprintf(buffer, buffer_size, "%s", essid);
    return res < 0 ? RESULT_ERROR : RESULT_SUCCESS;
}