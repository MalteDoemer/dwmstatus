#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../util.h"
#include "../colors.h"

#define CONTENT_SIZE 32

typedef enum power_profile_t {
    PERFORMANCE,
    BALANCED,
    POWER_SAVER,
} power_profile_t;

static result_t get_powerprofile(power_profile_t* out_profile)
{
    FILE* proc = popen("powerprofilesctl get", "r");

    if (!proc) {
        warn("powerprofile: popen:");
        return RESULT_ERROR;
    }

    char buffer[CONTENT_SIZE];
    fread_trim(proc, buffer, CONTENT_SIZE);
    pclose(proc);

    if (!strcmp(buffer, "performance"))
        *out_profile = PERFORMANCE;
    else if (!strcmp(buffer, "balanced"))
        *out_profile = BALANCED;
    else if (!strcmp(buffer, "power-saver"))
        *out_profile = POWER_SAVER;
    else {
        warn("powerprofile: unable to parse output: '%s'", buffer);
        return RESULT_ERROR;
    }

    return RESULT_SUCCESS;
}

result_t powerprofile(const char* unused, char* buffer, size_t buffer_size)
{
    power_profile_t profile;
    result_t res = get_powerprofile(&profile);

    if (res == RESULT_ERROR)
        return RESULT_ERROR;

    const char* label;

    switch (profile) {
    case PERFORMANCE:
        label = "performance";
        break;
    case BALANCED:
        label = "balanced";
        break;
    case POWER_SAVER:
        label = "power-saver";
        break;
    default:
        assert(0);
    }

    int n = snprintf(buffer, buffer_size, "%s", label);
    return n < 0 ? RESULT_ERROR : RESULT_SUCCESS;
}

result_t powerprofile_icon(const char* unused, char* buffer, size_t buffer_size)
{
    power_profile_t profile;
    result_t res = get_powerprofile(&profile);

    if (res == RESULT_ERROR)
        return RESULT_ERROR;

    const char* label;

    switch (profile) {
    case PERFORMANCE:
        label = YELLOW "" NORMAL;
        break;
    case BALANCED:
        label = "";
        break;
    case POWER_SAVER:
        label = GREEN "" NORMAL;
        break;
    default:
        assert(0);
    }

    int n = snprintf(buffer, buffer_size, "%s", label);
    return n < 0 ? RESULT_ERROR : RESULT_SUCCESS;
}