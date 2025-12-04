#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <unistd.h>

int read_file(const char *path, char *buffer, size_t size) {
    FILE *file = fopen(path, "r");
    if (!file) return 0;
    
    size_t bytes_read = fread(buffer, 1, size - 1, file);
    buffer[bytes_read] = '\0';
    fclose(file);
    
    return bytes_read > 0;
}

int write_file(const char *path, const char *data) {
    FILE *file = fopen(path, "w");
    if (!file) return 0;
    
    size_t len = strlen(data);
    size_t written = fwrite(data, 1, len, file);
    fclose(file);
    
    return written == len;
}

int file_exists(const char *path) {
    return access(path, F_OK) == 0;
}

int dir_exists(const char *path) {
    struct stat st;
    if (stat(path, &st) != 0) return 0;
    return S_ISDIR(st.st_mode);
}

void str_tolower(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void str_toupper(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

void str_trim(char *str) {
    if (!str) return;
    
    // Trim leading
    char *start = str;
    while (isspace((unsigned char)*start)) start++;
    
    if (*start == 0) {
        *str = 0;
        return;
    }
    
    // Trim trailing
    char *end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) end--;
    
    *(end + 1) = '\0';
    
    // Move trimmed string to beginning
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }
}

int str_startswith(const char *str, const char *prefix) {
    size_t str_len = strlen(str);
    size_t prefix_len = strlen(prefix);
    
    if (prefix_len > str_len) return 0;
    return strncmp(str, prefix, prefix_len) == 0;
}

int str_endswith(const char *str, const char *suffix) {
    size_t str_len = strlen(str);
    size_t suffix_len = strlen(suffix);
    
    if (suffix_len > str_len) return 0;
    return strcmp(str + str_len - suffix_len, suffix) == 0;
}

int str_contains(const char *str, const char *substr) {
    return strstr(str, substr) != NULL;
}

void path_join(char *dest, size_t size, const char *path1, const char *path2) {
    size_t len1 = strlen(path1);
    
    if (len1 > 0 && path1[len1-1] == '/') {
        snprintf(dest, size, "%s%s", path1, path2);
    } else {
        snprintf(dest, size, "%s/%s", path1, path2);
    }
}

int parse_int(const char *str, int default_val) {
    if (!str) return default_val;
    
    char *endptr;
    long val = strtol(str, &endptr, 10);
    
    if (endptr == str || *endptr != '\0') {
        return default_val;
    }
    
    return (int)val;
}

double parse_double(const char *str, double default_val) {
    if (!str) return default_val;
    
    char *endptr;
    double val = strtod(str, &endptr);
    
    if (endptr == str) {
        return default_val;
    }
    
    return val;
}

void format_number(double value, char *buffer, size_t size, int decimals) {
    char format[16];
    snprintf(format, sizeof(format), "%%.%df", decimals);
    snprintf(buffer, size, format, value);
}

void format_bytes(long bytes, char *buffer, size_t size) {
    const char *units[] = {"B", "KB", "MB", "GB", "TB"};
    int unit = 0;
    double value = (double)bytes;
    
    while (value >= 1024.0 && unit < 4) {
        value /= 1024.0;
        unit++;
    }
    
    snprintf(buffer, size, "%.2f %s", value, units[unit]);
}

int is_root(void) {
    return geteuid() == 0;
}

long get_system_uptime(void) {
    char buffer[128];
    if (!read_file("/proc/uptime", buffer, sizeof(buffer))) {
        return 0;
    }
    
    double uptime;
    if (sscanf(buffer, "%lf", &uptime) != 1) {
        return 0;
    }
    
    return (long)uptime;
}

int get_cpu_count(void) {
    long count = sysconf(_SC_NPROCESSORS_ONLN);
    return (count > 0) ? (int)count : 1;
}

long get_total_memory(void) {
    long pages = sysconf(_SC_PHYS_PAGES);
    long page_size = sysconf(_SC_PAGE_SIZE);
    
    if (pages <= 0 || page_size <= 0) return 0;
    
    return pages * page_size;
}
