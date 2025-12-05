#ifndef SENSOR_H
#define SENSOR_H

#define MAX_PATH 512
#define MAX_SENSORS 200
#define MAX_NAME_LEN 128
#define HWMON_PATH "/sys/class/hwmon"
#define THERMAL_PATH "/sys/class/thermal"
#define DRM_PATH "/sys/class/drm"

typedef enum {
    SENSOR_CPU = 0,
    SENSOR_GPU,
    SENSOR_NVME,
    SENSOR_CHIPSET,
    SENSOR_MEMORY,
    SENSOR_VRM,
    SENSOR_DISK,
    SENSOR_OTHER
} SensorType;

typedef enum {
    STATUS_OK = 0,
    STATUS_WARN,
    STATUS_CRITICAL,
    STATUS_ERROR
} SensorStatus;

typedef struct {
    char name[MAX_NAME_LEN];
    char label[MAX_NAME_LEN];
    char path[MAX_PATH];
    char device_model[MAX_NAME_LEN];
    SensorType type;
    SensorStatus status;
    
    double temp_current;
    double temp_max;
    double temp_min;
    double temp_critical;
    double temp_avg;
    
    long read_count;
    int active;
    int alarm_state;
    
    // Fan data (jika ada)
    int has_fan;
    int fan_speed_rpm;
    int fan_speed_percent;
} TempSensor;

typedef struct {
    double avg_cpu_temp;
    double max_cpu_temp;
    double min_cpu_temp;
    double avg_gpu_temp;
    double max_gpu_temp;
    double avg_nvme_temp;
    
    int cpu_count;
    int gpu_count;
    int nvme_count;
    int chipset_count;
    int total_active_sensors;
    
    int warnings;
    int criticals;
} SystemStats;

// Function prototypes
int scan_temperature_sensors(TempSensor *sensors);
int scan_hwmon_sensors(TempSensor *sensors, int *count);
int scan_thermal_sensors(TempSensor *sensors, int *count);
int scan_gpu_sensors(TempSensor *sensors, int *count);

double read_temperature(const char *path);
int read_fan_speed(const char *path);
void update_sensor_data(TempSensor *sensor);
void calculate_system_stats(TempSensor *sensors, int count, SystemStats *stats);

SensorType detect_sensor_type(const char *name, const char *label, const char *path);
SensorStatus get_sensor_status(double temp, double critical);
const char* get_type_name(SensorType type);
const char* get_type_icon(SensorType type);
const char* get_status_color(SensorStatus status);

int get_cpu_info(char *model, char *vendor, int *cores, int *threads);
int get_gpu_info(char *model, char *vendor, int index);

#endif // SENSOR_H
