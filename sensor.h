/**
 * @file sensor.h
 * @brief Temp Monitor - Sensor detection and management
 * 
 * Header file containing sensor data structures, constants,
 * and function prototypes for temperature sensor detection
 * and reading from Linux hwmon/thermal subsystems.
 * 
 * @version 0.0.2
 * @date 2024-12-05
 * 
 * MIT License
 * Copyright (c) 2024 Danko
 */

#ifndef SENSOR_H
#define SENSOR_H

/* Maximum path length for sensor files */
#define MAX_PATH 512
/* Maximum number of sensors to track */
#define MAX_SENSORS 200

/* Maximum length for sensor names and labels */
#define MAX_NAME_LEN 128

/* Maximum number of fans to track */
#define MAX_FANS 50

/* Linux sysfs paths for sensor data */
#define HWMON_PATH "/sys/class/hwmon"
#define THERMAL_PATH "/sys/class/thermal"
#define DRM_PATH "/sys/class/drm"

/**
 * @brief Sensor type enumeration
 * 
 * Categorizes sensors by their hardware type for
 * grouping and display purposes.
 */
typedef enum {
    SENSOR_CPU = 0,     /* CPU cores and package */
    SENSOR_GPU,         /* Graphics cards */
    SENSOR_NVME,        /* NVMe SSDs */
    SENSOR_CHIPSET,     /* Motherboard/PCH */
    SENSOR_MEMORY,      /* RAM modules */
    SENSOR_VRM,         /* Voltage regulators */
    SENSOR_DISK,        /* HDDs/SATA SSDs */
    SENSOR_OTHER        /* Unclassified sensors */
} SensorType;

/**
 * @brief Sensor status enumeration
 * 
 * Indicates the health/alert status of a sensor
 * based on its current temperature.
 */
typedef enum {
    STATUS_OK = 0,      /* Temperature normal */
    STATUS_WARN,        /* Temperature elevated */
    STATUS_CRITICAL,    /* Temperature critical */
    STATUS_ERROR        /* Sensor read error */
} SensorStatus;

/**
 * @brief Temperature sensor data structure
 * 
 * Stores all information about a single temperature sensor
 * including its identification, current readings, statistics,
 * and associated fan data.
 */
typedef struct {
    char name[MAX_NAME_LEN];        /* Driver/chip name */
    char label[MAX_NAME_LEN];       /* Human-readable label */
    char path[MAX_PATH];            /* Sysfs path to temp file */
    char fan_path[MAX_PATH];        /* Sysfs path to fan file */
    char device_model[MAX_NAME_LEN]; /* Device model info */
    SensorType type;                /* Sensor category */
    SensorStatus status;            /* Current status */
    
    double temp_current;    /* Current temperature (Celsius) */
    double temp_max;        /* Maximum recorded temperature */
    double temp_min;        /* Minimum recorded temperature */
    double temp_critical;   /* Critical threshold temperature */
    double temp_avg;        /* Running average temperature */
    
    long read_count;        /* Number of readings taken */
    int active;             /* 1 if sensor is working */
    int alarm_state;        /* 1 if alarm triggered */
    
    int has_fan;            /* 1 if sensor has associated fan */
    int fan_speed_rpm;      /* Current fan speed in RPM */
    int fan_speed_percent;  /* Fan speed as percentage */
    int fan_max_rpm;        /* Maximum fan RPM */
} TempSensor;

/**
 * @brief Standalone fan sensor data structure
 * 
 * For fans not directly associated with a temperature sensor.
 */
typedef struct {
    char name[MAX_NAME_LEN];    /* Fan name */
    char label[MAX_NAME_LEN];   /* Human-readable label */
    char path[MAX_PATH];        /* Sysfs path */
    int speed_rpm;              /* Current speed in RPM */
    int speed_percent;          /* Speed as percentage */
    int max_rpm;                /* Maximum RPM */
    int min_rpm;                /* Minimum RPM */
    int active;                 /* 1 if fan is detected */
} FanSensor;

/**
 * @brief System-wide statistics structure
 * 
 * Aggregated statistics across all sensors for
 * summary display and health monitoring.
 */
typedef struct {
    double avg_cpu_temp;        /* Average CPU temperature */
    double max_cpu_temp;        /* Maximum CPU temperature */
    double min_cpu_temp;        /* Minimum CPU temperature */
    double avg_gpu_temp;        /* Average GPU temperature */
    double max_gpu_temp;        /* Maximum GPU temperature */
    double avg_nvme_temp;       /* Average NVMe temperature */
    
    int cpu_count;              /* Number of CPU sensors */
    int gpu_count;              /* Number of GPU sensors */
    int nvme_count;             /* Number of NVMe sensors */
    int chipset_count;          /* Number of chipset sensors */
    int total_active_sensors;   /* Total active sensors */
    int total_fans;             /* Total fans detected */
    
    int warnings;               /* Number of warnings */
    int criticals;              /* Number of critical alerts */
} SystemStats;

int scan_temperature_sensors(TempSensor *sensors);
int scan_hwmon_sensors(TempSensor *sensors, int *count);
int scan_thermal_sensors(TempSensor *sensors, int *count);
int scan_gpu_sensors(TempSensor *sensors, int *count);
int scan_fan_sensors(TempSensor *sensors, int count);

double read_temperature(const char *path);
int read_fan_speed(const char *path);
int read_fan_max(const char *hwmon_path, const char *fan_num);
void update_sensor_data(TempSensor *sensor);
void update_fan_data(TempSensor *sensor);
void calculate_system_stats(TempSensor *sensors, int count, SystemStats *stats);

SensorType detect_sensor_type(const char *name, const char *label, const char *path);
SensorStatus get_sensor_status(double temp, double critical);
const char* get_type_name(SensorType type);
const char* get_type_icon(SensorType type);
const char* get_status_color(SensorStatus status);

int get_cpu_info(char *model, char *vendor, int *cores, int *threads);
int get_gpu_info(char *model, char *vendor, int index);

#endif
