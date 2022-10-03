#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../map.h"
#include "../util.h"

#define CONTENT_SIZE 32

static hashmap* map = NULL;

static result_t resolve_hwmon_path(const char* device_name, char* buffer, size_t buffer_size)
{
    const char* prefix = "/sys/class/hwmon";

    DIR* dir = opendir(prefix);

    if (!dir) {
        warn("temp: '%s':", prefix);
        return RESULT_ERROR;
    }

    struct dirent* entry;

    while ((entry = readdir(dir)) != NULL) {

        // create a full path to the name file
        snprintf(buffer, buffer_size, "%s/%s/name", prefix, entry->d_name);

        FILE* file = fopen(buffer, "r");

        // if the directory doesn't contain a name file just skip it
        if (!file)
            continue;

        char content[CONTENT_SIZE];
        fread_trim(file, content, CONTENT_SIZE);
        fclose(file);

        if (!strcmp(device_name, content)) {
            snprintf(buffer, buffer_size, "%s/%s/temp1_input", prefix, entry->d_name);
            return RESULT_SUCCESS;
        }
    }

    // the device wasn't found
    warn("temp: device '%s' not found", device_name);
    return RESULT_ERROR;
}

static const char* hwmon_path(const char* device_name)
{
    if (!map)
        map = hashmap_create();

    uintptr_t cached;

    if (hashmap_get(map, (void*)device_name, strlen(device_name), &cached)) {
        return (const char*)cached;
    } else {
        char* buffer = malloc(PATH_MAX);
        result_t res = resolve_hwmon_path(device_name, buffer, PATH_MAX);

        if (res == RESULT_ERROR) {
            free(buffer);
            return NULL;
        }

        hashmap_set(map, (void*)device_name, strlen(device_name), (uintptr_t)buffer);
        return buffer;
    }
}

static result_t get_temperature(const char* device, int* out_temprature)
{
    const char* path = hwmon_path(device);

    if (!path)
        return RESULT_ERROR;

    int res = pscanf(path, "%d", out_temprature);

    if (res != 1)
        return RESULT_ERROR;
    
    /* temperature is most likely in millidegrees celsius */
    *out_temprature /= 1000;

    return RESULT_SUCCESS;
}

result_t temp(const char* device, char* buffer, size_t buffer_size)
{
    int temperature;
    result_t res = get_temperature(device, &temperature);

    if (res == RESULT_ERROR)
        return RESULT_ERROR;

    int n = snprintf(buffer, buffer_size, "%d", temperature);
    return n < 0 ? RESULT_ERROR : RESULT_SUCCESS;
}

result_t temp_icon(const char* device, char* buffer, size_t buffer_size)
{
    int temperature;
    result_t res = get_temperature(device, &temperature);

    if (res == RESULT_ERROR)
        return RESULT_ERROR;

    const char* icon;

    // Note: these values are chosen for CPU temprature
    if (temperature < 45) {
        icon = "";
    } else if (temperature < 55) {
        icon = "";
    } else if (temperature < 65) {
        icon = "";
    } else if (temperature < 75) {
        icon = "";
    } else {
        icon = "";
    }

    int n = snprintf(buffer, buffer_size, "%s", icon);
    return n < 0 ? RESULT_ERROR : RESULT_SUCCESS;
}