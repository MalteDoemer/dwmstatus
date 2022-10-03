#include "util.h"


result_t date(const char* format, char* buffer, size_t buffer_size);

result_t disk_free(const char* path, char* buffer, size_t buffer_size);

result_t ram_free(const char* unused, char* buffer, size_t buffer_size);
result_t ram_avail(const char* unused, char* buffer, size_t buffer_size);
result_t ram_perc(const char* unused, char* buffer, size_t buffer_size);
result_t ram_total(const char* unused, char* buffer, size_t buffer_size);
result_t ram_used(const char* unused, char* buffer, size_t buffer_size);

result_t cpu_perc(const char* unused, char* bufer, size_t buffer_size);

result_t temp(const char* device, char* buffer, size_t buffer_size);
result_t temp_icon(const char* device, char* buffer, size_t buffer_size);

result_t volume_alsa(const char* mixer, char* buffer, size_t buffer_size);
result_t volume_icon_alsa(const char* mixer, char* buffer, size_t buffer_size);

result_t powerprofile(const char* unused, char* buffer, size_t buffer_size);
result_t powerprofile_icon(const char* unused, char* buffer, size_t buffer_size);

result_t battery_perc(const char* battery, char* buffer, size_t buffer_size);
result_t battery_icon(const char* battery, char* buffer, size_t buffer_size);

result_t keyboard_layout(const char* unused, char* buffer, size_t buffer_size);

result_t wifi_perc(const char* interface, char* buffer, size_t buffer_size);
result_t wifi_bars(const char* interface, char* buffer, size_t buffer_size);
result_t wifi_essid(const char* interface, char* buffer, size_t buffer_size);