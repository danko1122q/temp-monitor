#include "sensor.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

static void get_sensor_name(const char *hwmon_path, char *name, size_t size) {
    char path[MAX_PATH];
    snprintf(path, sizeof(path), "%s/name", hwmon_path);
    
    if (!read_file(path, name, size)) {
        strncpy(name, "Unknown", size - 1);
        name[size - 1] = '\0';
        return;
    }
    
    str_trim(name);
}

static void get_sensor_label(const char *base_path, const char *temp_file, char *label, size_t size) {
    char path[MAX_PATH];
    char temp_num[16];
    
    if (sscanf(temp_file, "temp%15[^_]", temp_num) != 1) {
        strncpy(label, "Unknown", size - 1);
        return;
    }
    
    snprintf(path, sizeof(path), "%s/temp%s_label", base_path, temp_num);
    
    if (!read_file(path, label, size)) {
        snprintf(label, size, "Sensor %s", temp_num);
        return;
    }
    
    str_trim(label);
}

static double get_critical_temp(const char *base_path, const char *temp_file) {
    char path[MAX_PATH];
    char temp_num[16];
    char buffer[32];
    
    if (sscanf(temp_file, "temp%15[^_]", temp_num) != 1) {
        return 100.0;
    }
    
    snprintf(path, sizeof(path), "%s/temp%s_crit", base_path, temp_num);
    
    if (!read_file(path, buffer, sizeof(buffer))) {
        snprintf(path, sizeof(path), "%s/temp%s_max", base_path, temp_num);
        if (!read_file(path, buffer, sizeof(buffer))) {
            return 100.0;
        }
    }
    
    return parse_double(buffer, 100000.0) / 1000.0;
}

SensorType detect_sensor_type(const char *name, const char *label, const char *path) {
    char name_lower[MAX_NAME_LEN];
    char label_lower[MAX_NAME_LEN];
    
    (void)path;
    
    strncpy(name_lower, name, sizeof(name_lower) - 1);
    name_lower[sizeof(name_lower) - 1] = '\0';
    strncpy(label_lower, label, sizeof(label_lower) - 1);
    label_lower[sizeof(label_lower) - 1] = '\0';
    str_tolower(name_lower);
    str_tolower(label_lower);
    
    if (str_contains(name_lower, "coretemp") || 
        str_contains(name_lower, "k10temp") ||
        str_contains(name_lower, "zenpower") ||
        str_contains(name_lower, "cpu") ||
        str_contains(name_lower, "tctl") ||
        str_contains(name_lower, "tccd") ||
        str_contains(label_lower, "core") ||
        str_contains(label_lower, "package") ||
        str_contains(label_lower, "tdie") ||
        str_contains(label_lower, "cpu")) {
        return SENSOR_CPU;
    }
    
    if (str_contains(name_lower, "amdgpu") || 
        str_contains(name_lower, "nouveau") ||
        str_contains(name_lower, "radeon") ||
        str_contains(name_lower, "nvidia") ||
        str_contains(name_lower, "i915") ||
        str_contains(label_lower, "gpu") ||
        str_contains(label_lower, "edge") ||
        str_contains(label_lower, "junction") ||
        str_contains(label_lower, "mem") ) {
        return SENSOR_GPU;
    }
    
    if (str_contains(name_lower, "nvme") || 
        str_contains(label_lower, "composite") ||
        str_contains(label_lower, "sensor 1") ||
        str_contains(label_lower, "sensor 2")) {
        return SENSOR_NVME;
    }
    
    if (str_contains(name_lower, "dimm") || 
        str_contains(label_lower, "memory") ||
        str_contains(label_lower, "ram") ||
        str_contains(label_lower, "sodimm")) {
        return SENSOR_MEMORY;
    }
    
    if (str_contains(label_lower, "vrm") || 
        str_contains(label_lower, "vcore") ||
        str_contains(label_lower, "vsoc") ||
        str_contains(label_lower, "soc")) {
        return SENSOR_VRM;
    }
    
    if (str_contains(name_lower, "acpitz") || 
        str_contains(name_lower, "pch") ||
        str_contains(name_lower, "nct") ||
        str_contains(name_lower, "it87") ||
        str_contains(name_lower, "asus") ||
        str_contains(name_lower, "thinkpad") ||
        str_contains(label_lower, "motherboard") ||
        str_contains(label_lower, "chipset") ||
        str_contains(label_lower, "systin") ||
        str_contains(label_lower, "peci")) {
        return SENSOR_CHIPSET;
    }
    
    if (str_contains(name_lower, "drivetemp") || 
        str_contains(name_lower, "sata") ||
        str_contains(name_lower, "scsi") ||
        str_contains(label_lower, "disk") ||
        str_contains(label_lower, "hdd") ||
        str_contains(label_lower, "ssd")) {
        return SENSOR_DISK;
    }
    
    return SENSOR_OTHER;
}

double read_temperature(const char *path) {
    char buffer[32];
    if (!read_file(path, buffer, sizeof(buffer))) {
        return -999.0;
    }
    
    long temp_milli = strtol(buffer, NULL, 10);
    if (temp_milli == 0 && buffer[0] != '0') {
        return -999.0;
    }
    
    return (double)temp_milli / 1000.0;
}

int read_fan_speed(const char *path) {
    char buffer[32];
    if (!read_file(path, buffer, sizeof(buffer))) {
        return -1;
    }
    
    return parse_int(buffer, -1);
}

int read_fan_max(const char *hwmon_path, const char *fan_num) {
    char path[MAX_PATH];
    char buffer[32];
    
    snprintf(path, sizeof(path), "%s/fan%s_max", hwmon_path, fan_num);
    if (read_file(path, buffer, sizeof(buffer))) {
        return parse_int(buffer, 5000);
    }
    
    snprintf(path, sizeof(path), "%s/pwm%s_max", hwmon_path, fan_num);
    if (read_file(path, buffer, sizeof(buffer))) {
        return 255;
    }
    
    return 5000;
}

SensorStatus get_sensor_status(double temp, double critical) {
    if (temp < 0) return STATUS_ERROR;
    if (temp >= critical) return STATUS_CRITICAL;
    if (temp >= critical * 0.85) return STATUS_WARN;
    return STATUS_OK;
}

void update_sensor_data(TempSensor *sensor) {
    double temp = read_temperature(sensor->path);
    
    if (temp < -500) {
        sensor->active = 0;
        return;
    }
    
    sensor->temp_current = temp;
    sensor->active = 1;
    sensor->read_count++;
    
    if (sensor->read_count == 1) {
        sensor->temp_max = temp;
        sensor->temp_min = temp;
        sensor->temp_avg = temp;
    } else {
        if (temp > sensor->temp_max) sensor->temp_max = temp;
        if (temp < sensor->temp_min) sensor->temp_min = temp;
        sensor->temp_avg = (sensor->temp_avg * (sensor->read_count - 1) + temp) / sensor->read_count;
    }
    
    sensor->status = get_sensor_status(temp, sensor->temp_critical);
    
    if (sensor->has_fan && sensor->fan_path[0] != '\0') {
        update_fan_data(sensor);
    }
}

void update_fan_data(TempSensor *sensor) {
    if (!sensor->has_fan || sensor->fan_path[0] == '\0') {
        return;
    }
    
    sensor->fan_speed_rpm = read_fan_speed(sensor->fan_path);
    
    if (sensor->fan_speed_rpm > 0 && sensor->fan_max_rpm > 0) {
        sensor->fan_speed_percent = (sensor->fan_speed_rpm * 100) / sensor->fan_max_rpm;
        if (sensor->fan_speed_percent > 100) sensor->fan_speed_percent = 100;
    } else {
        sensor->fan_speed_percent = 0;
    }
}

static int scan_fans_for_hwmon(const char *hwmon_path, TempSensor *sensors, int sensor_count) {
    DIR *dir = opendir(hwmon_path);
    if (!dir) return 0;
    
    struct dirent *entry;
    int fans_found = 0;
    
    while ((entry = readdir(dir)) != NULL) {
        if (str_contains(entry->d_name, "fan") && 
            str_endswith(entry->d_name, "_input")) {
            
            char fan_num[8];
            if (sscanf(entry->d_name, "fan%7[^_]", fan_num) != 1) {
                continue;
            }
            
            char fan_path[MAX_PATH];
            snprintf(fan_path, sizeof(fan_path), "%s/%s", hwmon_path, entry->d_name);
            
            int fan_rpm = read_fan_speed(fan_path);
            if (fan_rpm < 0) continue;
            
            int fan_max = read_fan_max(hwmon_path, fan_num);
            
            for (int i = 0; i < sensor_count; i++) {
                if (str_contains(sensors[i].path, hwmon_path) && !sensors[i].has_fan) {
                    sensors[i].has_fan = 1;
                    strncpy(sensors[i].fan_path, fan_path, sizeof(sensors[i].fan_path) - 1);
                    sensors[i].fan_path[sizeof(sensors[i].fan_path) - 1] = '\0';
                    sensors[i].fan_max_rpm = fan_max;
                    sensors[i].fan_speed_rpm = fan_rpm;
                    if (fan_max > 0) {
                        sensors[i].fan_speed_percent = (fan_rpm * 100) / fan_max;
                    }
                    fans_found++;
                    break;
                }
            }
        }
    }
    
    closedir(dir);
    return fans_found;
}

int scan_fan_sensors(TempSensor *sensors, int count) {
    DIR *dir = opendir(HWMON_PATH);
    if (!dir) return 0;
    
    struct dirent *entry;
    int total_fans = 0;
    
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') continue;
        
        char hwmon_path[MAX_PATH];
        snprintf(hwmon_path, sizeof(hwmon_path), "%s/%s", HWMON_PATH, entry->d_name);
        
        if (!dir_exists(hwmon_path)) continue;
        
        total_fans += scan_fans_for_hwmon(hwmon_path, sensors, count);
    }
    
    closedir(dir);
    return total_fans;
}

int scan_hwmon_sensors(TempSensor *sensors, int *count) {
    DIR *dir = opendir(HWMON_PATH);
    if (!dir) return 0;
    
    struct dirent *entry;
    int found = 0;
    
    while ((entry = readdir(dir)) != NULL && *count < MAX_SENSORS) {
        if (entry->d_name[0] == '.') continue;
        
        char hwmon_path[MAX_PATH];
        snprintf(hwmon_path, sizeof(hwmon_path), "%s/%s", HWMON_PATH, entry->d_name);
        
        if (!dir_exists(hwmon_path)) continue;
        
        char sensor_name[MAX_NAME_LEN];
        get_sensor_name(hwmon_path, sensor_name, sizeof(sensor_name));
        
        DIR *hwmon_dir = opendir(hwmon_path);
        if (!hwmon_dir) continue;
        
        struct dirent *temp_entry;
        while ((temp_entry = readdir(hwmon_dir)) != NULL && *count < MAX_SENSORS) {
            if (str_contains(temp_entry->d_name, "temp") && 
                str_endswith(temp_entry->d_name, "_input")) {
                
                TempSensor *s = &sensors[*count];
                memset(s, 0, sizeof(TempSensor));
                
                get_sensor_label(hwmon_path, temp_entry->d_name, s->label, sizeof(s->label));
                strncpy(s->name, sensor_name, sizeof(s->name) - 1);
                s->name[sizeof(s->name) - 1] = '\0';
                snprintf(s->path, sizeof(s->path), "%s/%s", hwmon_path, temp_entry->d_name);
                
                s->type = detect_sensor_type(sensor_name, s->label, s->path);
                s->temp_critical = get_critical_temp(hwmon_path, temp_entry->d_name);
                s->temp_max = -999.0;
                s->temp_min = 999.0;
                s->read_count = 0;
                s->active = 1;
                s->has_fan = 0;
                s->fan_path[0] = '\0';
                
                (*count)++;
                found++;
            }
        }
        closedir(hwmon_dir);
    }
    closedir(dir);
    
    return found;
}

int scan_thermal_sensors(TempSensor *sensors, int *count) {
    DIR *dir = opendir(THERMAL_PATH);
    if (!dir) return 0;
    
    struct dirent *entry;
    int found = 0;
    
    while ((entry = readdir(dir)) != NULL && *count < MAX_SENSORS) {
        if (!str_startswith(entry->d_name, "thermal_zone")) continue;
        
        char zone_path[MAX_PATH];
        snprintf(zone_path, sizeof(zone_path), "%s/%s", THERMAL_PATH, entry->d_name);
        
        char temp_path[MAX_PATH];
        snprintf(temp_path, sizeof(temp_path), "%s/temp", zone_path);
        
        if (!file_exists(temp_path)) continue;
        
        TempSensor *s = &sensors[*count];
        memset(s, 0, sizeof(TempSensor));
        
        char type_buf[MAX_NAME_LEN];
        char type_path[MAX_PATH];
        snprintf(type_path, sizeof(type_path), "%s/type", zone_path);
        
        if (read_file(type_path, type_buf, sizeof(type_buf))) {
            str_trim(type_buf);
            strncpy(s->label, type_buf, sizeof(s->label) - 1);
        } else {
            snprintf(s->label, sizeof(s->label), "Thermal Zone %s", entry->d_name + 12);
        }
        
        strncpy(s->name, "thermal", sizeof(s->name) - 1);
        strncpy(s->path, temp_path, sizeof(s->path) - 1);
        s->type = SENSOR_CHIPSET;
        s->temp_critical = 100.0;
        s->temp_max = -999.0;
        s->temp_min = 999.0;
        s->read_count = 0;
        s->active = 1;
        s->has_fan = 0;
        
        (*count)++;
        found++;
    }
    
    closedir(dir);
    return found;
}

int scan_temperature_sensors(TempSensor *sensors) {
    int count = 0;
    
    scan_hwmon_sensors(sensors, &count);
    
    if (count == 0) {
        scan_thermal_sensors(sensors, &count);
    }
    
    if (count > 0) {
        scan_fan_sensors(sensors, count);
    }
    
    return count;
}

void calculate_system_stats(TempSensor *sensors, int count, SystemStats *stats) {
    memset(stats, 0, sizeof(SystemStats));
    
    stats->min_cpu_temp = 999.0;
    
    for (int i = 0; i < count; i++) {
        if (!sensors[i].active || sensors[i].temp_current < -500) continue;
        
        stats->total_active_sensors++;
        
        if (sensors[i].has_fan && sensors[i].fan_speed_rpm > 0) {
            stats->total_fans++;
        }
        
        if (sensors[i].status == STATUS_WARN) stats->warnings++;
        if (sensors[i].status == STATUS_CRITICAL) stats->criticals++;
        
        switch (sensors[i].type) {
            case SENSOR_CPU:
                stats->avg_cpu_temp += sensors[i].temp_current;
                if (sensors[i].temp_current > stats->max_cpu_temp)
                    stats->max_cpu_temp = sensors[i].temp_current;
                if (sensors[i].temp_current < stats->min_cpu_temp)
                    stats->min_cpu_temp = sensors[i].temp_current;
                stats->cpu_count++;
                break;
                
            case SENSOR_GPU:
                stats->avg_gpu_temp += sensors[i].temp_current;
                if (sensors[i].temp_current > stats->max_gpu_temp)
                    stats->max_gpu_temp = sensors[i].temp_current;
                stats->gpu_count++;
                break;
                
            case SENSOR_NVME:
                stats->avg_nvme_temp += sensors[i].temp_current;
                stats->nvme_count++;
                break;
                
            case SENSOR_CHIPSET:
                stats->chipset_count++;
                break;
                
            default:
                break;
        }
    }
    
    if (stats->cpu_count > 0) stats->avg_cpu_temp /= stats->cpu_count;
    if (stats->gpu_count > 0) stats->avg_gpu_temp /= stats->gpu_count;
    if (stats->nvme_count > 0) stats->avg_nvme_temp /= stats->nvme_count;
    if (stats->min_cpu_temp > 900) stats->min_cpu_temp = 0;
}

const char* get_type_name(SensorType type) {
    switch (type) {
        case SENSOR_CPU: return "CPU";
        case SENSOR_GPU: return "GPU";
        case SENSOR_NVME: return "NVMe";
        case SENSOR_CHIPSET: return "Chipset";
        case SENSOR_MEMORY: return "Memory";
        case SENSOR_VRM: return "VRM";
        case SENSOR_DISK: return "Disk";
        default: return "Other";
    }
}

const char* get_type_icon(SensorType type) {
    switch (type) {
        case SENSOR_CPU: return "[CPU]";
        case SENSOR_GPU: return "[GPU]";
        case SENSOR_NVME: return "[SSD]";
        case SENSOR_CHIPSET: return "[MB]";
        case SENSOR_MEMORY: return "[RAM]";
        case SENSOR_VRM: return "[VRM]";
        case SENSOR_DISK: return "[HDD]";
        default: return "[???]";
    }
}

const char* get_status_color(SensorStatus status) {
    switch (status) {
        case STATUS_OK: return COLOR_GREEN;
        case STATUS_WARN: return COLOR_YELLOW;
        case STATUS_CRITICAL: return COLOR_RED;
        case STATUS_ERROR: return COLOR_BRIGHT_BLACK;
        default: return COLOR_RESET;
    }
}
