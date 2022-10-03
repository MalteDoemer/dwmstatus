#include <stdio.h>
#include <string.h>

#include "../util.h"

#define CONTENT_SIZE 32
#define COMMAND_SIZE 128

typedef enum { ALSA_ON, ALSA_OFF } alsa_status_t;

static result_t get_alsa_status(const char* mixer, alsa_status_t* status_out)
{
    const char* command_fmt = "amixer get %s | tail -n1  | awk -F '[][]' '{ print $4  }'";
    char command[COMMAND_SIZE];

    int n = snprintf(command, COMMAND_SIZE, command_fmt, mixer);

    if (n < 0)
        return RESULT_ERROR;

    FILE* proc = popen(command, "r");

    if (!proc) {
        warn("volume_alsa: popen:");
        return RESULT_ERROR;
    }

    char content[CONTENT_SIZE];
    fread_trim(proc, content, CONTENT_SIZE);
    pclose(proc);

    if (!strcmp(content, "on"))
        *status_out = ALSA_ON;
    else if (!strcmp(content, "off"))
        *status_out = ALSA_OFF;
    else {
        warn("volume_alsa: unable to parse status command: '%s'", content);
        return RESULT_ERROR;
    }

    return RESULT_SUCCESS;
}

static result_t get_alsa_volume(const char* mixer, int* volume_out)
{
    const char* command_fmt = "amixer get %s | tail -n1  | awk -F '[][%%]' '{ print $2  }'";
    char command[COMMAND_SIZE];

    int n = snprintf(command, COMMAND_SIZE, command_fmt, mixer);

    if (n < 0)
        return RESULT_ERROR;

    FILE* proc = popen(command, "r");

    if (!proc) {
        warn("volume_alsa: popen:");
        return RESULT_ERROR;
    }

    char content[CONTENT_SIZE];
    fread_trim(proc, content, CONTENT_SIZE);
    pclose(proc);

    str2int_result_t conv_res = str2int(volume_out, content, 10);

    if (conv_res != STR2INT_SUCCESS) {
        warn("volume_alsa: unable to parse volume: '%s'", content);
        return RESULT_ERROR;
    }

    return RESULT_SUCCESS;
}

result_t volume_alsa(const char* mixer, char* buffer, size_t buffer_size)
{
    int volume;
    result_t res = get_alsa_volume(mixer, &volume);

    if (res == RESULT_ERROR)
        return RESULT_ERROR;

    int n = snprintf(buffer, buffer_size, "%d", volume);
    return n < 0 ? RESULT_ERROR : RESULT_SUCCESS;
}

result_t volume_icon_alsa(const char* mixer, char* buffer, size_t buffer_size)
{
    int volume;
    alsa_status_t status;
    result_t res1 = get_alsa_volume(mixer, &volume);
    result_t res2 = get_alsa_status(mixer, &status);

    if (res1 == RESULT_ERROR || res2 == RESULT_ERROR)
        return RESULT_ERROR;


    const char* icon;

    if (status == ALSA_OFF) {
        icon = "";
    } else {

        if (volume == 0) {
            icon = "";
        } else if (volume < 33) {
            icon = "";
        } else {
            icon = "";
        }

    }

    int n = snprintf(buffer, buffer_size, "%s", icon);
    return n < 0 ? RESULT_ERROR : RESULT_SUCCESS;
}