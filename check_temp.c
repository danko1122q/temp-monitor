#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <signal.h>
#include <sys/stat.h>

#define MAX_PATH 512
#define MAX_SENSORS 100
#define HWMON_PATH "/sys/class/hwmon"
#define THERMAL_PATH "/sys/class/thermal"
#define VERSION "2.0.0"

typedef struct {
    char name[128];
    char label[128];
    char path[MAX_PATH];
    int type; // 0=CPU, 1=GPU, 2=NVMe, 3=Chipset, 4=Other
    double temp;
    double max_temp;
    double min_temp;
    int active;
} TempSensor;

typedef struct {
    double avg_cpu_temp;
    double max_cpu_temp;
    double avg_gpu_temp;
    int cpu_count;
    int gpu_count;
    int total_sensors;
} SystemStats;

// Global variables
volatile sig_atomic_t keep_running = 1;
int use_celsius = 1;

// Signal handler untuk Ctrl+C
void sigint_handler(int sig) {
    (void)sig;
    keep_running = 0;
}

// Fungsi untuk membersihkan layar
void clear_screen() {
    printf("\033[2J\033[H");
    fflush(stdout);
}

// Fungsi untuk mendapatkan ukuran terminal
void get_terminal_size(int *rows, int *cols) {
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
        *rows = 24;
        *cols = 80;
    } else {
        *rows = w.ws_row;
        *cols = w.ws_col;
    }
}

// Fungsi untuk membaca file text dengan error handling yang lebih baik
int read_file(const char *path, char *buffer, size_t size) {
    FILE *file = fopen(path, "r");
    if (!file) return 0;
    
    size_t bytes_read = fread(buffer, 1, size - 1, file);
    buffer[bytes_read] = '\0';
    fclose(file);
    
    return bytes_read > 0;
}

// Fungsi untuk mendapatkan nama sensor
void get_sensor_name(const char *hwmon_path, char *name, size_t size) {
    char path[MAX_PATH];
    int written = snprintf(path, sizeof(path), "%s/name", hwmon_path);
    
    if (written < 0 || written >= (int)sizeof(path)) {
        strncpy(name, "Unknown", size - 1);
        name[size - 1] = '\0';
        return;
    }
    
    if (!read_file(path, name, size)) {
        strncpy(name, "Unknown", size - 1);
        name[size - 1] = '\0';
        return;
    }
    
    // Hapus newline dan whitespace
    name[strcspn(name, "\n\r")] = '\0';
}

// Fungsi untuk mendapatkan label sensor
void get_sensor_label(const char *base_path, const char *temp_file, char *label, size_t size) {
    char path[MAX_PATH];
    char temp_num[16];
    
    if (sscanf(temp_file, "temp%15[^_]", temp_num) != 1) {
        strncpy(label, "Unknown", size - 1);
        label[size - 1] = '\0';
        return;
    }
    
    int written = snprintf(path, sizeof(path), "%s/temp%s_label", base_path, temp_num);
    
    if (written < 0 || written >= (int)sizeof(path)) {
        snprintf(label, size, "Sensor %s", temp_num);
        return;
    }
    
    if (!read_file(path, label, size)) {
        snprintf(label, size, "Sensor %s", temp_num);
        return;
    }
    
    label[strcspn(label, "\n\r")] = '\0';
}

// Fungsi untuk konversi string ke lowercase
void str_tolower(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

// Fungsi untuk menentukan tipe sensor (lebih canggih)
int get_sensor_type(const char *name, const char *label) {
    char name_lower[128], label_lower[128];
    
    strncpy(name_lower, name, sizeof(name_lower) - 1);
    strncpy(label_lower, label, sizeof(label_lower) - 1);
    name_lower[sizeof(name_lower) - 1] = '\0';
    label_lower[sizeof(label_lower) - 1] = '\0';
    
    str_tolower(name_lower);
    str_tolower(label_lower);
    
    // Deteksi CPU
    if (strstr(name_lower, "coretemp") || strstr(name_lower, "k10temp") || 
        strstr(name_lower, "cpu") || strstr(label_lower, "core") ||
        strstr(label_lower, "cpu") || strstr(name_lower, "zenpower") ||
        strstr(label_lower, "package") || strstr(name_lower, "tctl") ||
        strstr(name_lower, "tccd")) {
        return 0;
    }
    
    // Deteksi GPU
    if (strstr(name_lower, "amdgpu") || strstr(name_lower, "nouveau") || 
        strstr(name_lower, "radeon") || strstr(label_lower, "gpu") ||
        strstr(label_lower, "edge") || strstr(name_lower, "nvidia")) {
        return 1;
    }
    
    // Deteksi NVMe
    if (strstr(name_lower, "nvme") || strstr(label_lower, "composite") ||
        strstr(label_lower, "sensor 1") || strstr(label_lower, "sensor 2")) {
        return 2;
    }
    
    // Deteksi Chipset
    if (strstr(name_lower, "acpi") || strstr(name_lower, "pch") ||
        strstr(label_lower, "motherboard") || strstr(name_lower, "nct") ||
        strstr(name_lower, "it87")) {
        return 3;
    }
    
    return 4; // Other
}

// Fungsi untuk scan semua sensor suhu dengan error handling
int scan_temperature_sensors(TempSensor *sensors) {
    DIR *dir;
    struct dirent *entry;
    int count = 0;
    
    dir = opendir(HWMON_PATH);
    if (!dir) {
        fprintf(stderr, "Error: Tidak dapat membuka %s\n", HWMON_PATH);
        return 0;
    }
    
    while ((entry = readdir(dir)) != NULL && count < MAX_SENSORS) {
        if (entry->d_name[0] == '.') continue;
        
        char hwmon_path[MAX_PATH];
        int written = snprintf(hwmon_path, sizeof(hwmon_path), "%s/%s", 
                               HWMON_PATH, entry->d_name);
        
        if (written < 0 || written >= (int)sizeof(hwmon_path)) continue;
        
        // Cek apakah direktori dapat diakses
        struct stat st;
        if (stat(hwmon_path, &st) != 0) continue;
        
        // Baca nama sensor
        char sensor_name[128];
        get_sensor_name(hwmon_path, sensor_name, sizeof(sensor_name));
        
        // Scan semua file temp*_input
        DIR *hwmon_dir = opendir(hwmon_path);
        if (!hwmon_dir) continue;
        
        struct dirent *temp_entry;
        while ((temp_entry = readdir(hwmon_dir)) != NULL && count < MAX_SENSORS) {
            if (strstr(temp_entry->d_name, "temp") && 
                strstr(temp_entry->d_name, "_input")) {
                
                char label[128];
                get_sensor_label(hwmon_path, temp_entry->d_name, label, sizeof(label));
                
                strncpy(sensors[count].name, sensor_name, sizeof(sensors[count].name) - 1);
                sensors[count].name[sizeof(sensors[count].name) - 1] = '\0';
                
                strncpy(sensors[count].label, label, sizeof(sensors[count].label) - 1);
                sensors[count].label[sizeof(sensors[count].label) - 1] = '\0';
                
                written = snprintf(sensors[count].path, sizeof(sensors[count].path), 
                                   "%s/%s", hwmon_path, temp_entry->d_name);
                
                if (written < 0 || written >= (int)sizeof(sensors[count].path)) continue;
                
                sensors[count].type = get_sensor_type(sensor_name, label);
                sensors[count].temp = 0.0;
                sensors[count].max_temp = -999.0;
                sensors[count].min_temp = 999.0;
                sensors[count].active = 1;
                count++;
            }
        }
        closedir(hwmon_dir);
    }
    closedir(dir);
    
    return count;
}

// Fungsi untuk membaca suhu dengan error handling
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

// Fungsi untuk konversi Celsius ke Fahrenheit
double celsius_to_fahrenheit(double celsius) {
    return (celsius * 9.0 / 5.0) + 32.0;
}

// Fungsi untuk menampilkan suhu dengan format yang dipilih
void print_temperature(double temp) {
    if (use_celsius) {
        printf("%6.1f°C", temp);
    } else {
        printf("%6.1f°F", celsius_to_fahrenheit(temp));
    }
}

// Fungsi untuk menampilkan bar suhu dengan lebih banyak detail
void print_temp_bar(double temp, int width) {
    int filled = 0;
    
    if (temp >= 0 && temp <= 100) {
        filled = (int)((temp / 100.0) * width);
    } else if (temp > 100) {
        filled = width;
    }
    
    // Tentukan warna berdasarkan suhu
    const char *color;
    const char *bar_char;
    
    if (temp < 40) {
        color = "\033[36m"; // Cyan - Dingin
        bar_char = "█";
    } else if (temp < 50) {
        color = "\033[32m"; // Hijau - Aman
        bar_char = "█";
    } else if (temp < 60) {
        color = "\033[92m"; // Hijau terang - Normal
        bar_char = "█";
    } else if (temp < 70) {
        color = "\033[33m"; // Kuning - Hangat
        bar_char = "▓";
    } else if (temp < 80) {
        color = "\033[93m"; // Kuning terang - Tinggi
        bar_char = "▓";
    } else if (temp < 90) {
        color = "\033[91m"; // Merah muda - Sangat tinggi
        bar_char = "▒";
    } else {
        color = "\033[31m\033[1m"; // Merah bold - Bahaya!
        bar_char = "▒";
    }
    
    printf("%s[", color);
    for (int i = 0; i < width; i++) {
        if (i < filled) {
            printf("%s", bar_char);
        } else {
            printf("░");
        }
    }
    printf("]\033[0m");
}

// Fungsi untuk mendapatkan waktu saat ini
void get_current_time(char *buffer, size_t size) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", tm_info);
}

// Fungsi untuk menghitung statistik sistem
void calculate_stats(TempSensor *sensors, int count, SystemStats *stats) {
    memset(stats, 0, sizeof(SystemStats));
    stats->total_sensors = count;
    
    for (int i = 0; i < count; i++) {
        if (sensors[i].temp < -500) continue;
        
        if (sensors[i].type == 0) { // CPU
            stats->avg_cpu_temp += sensors[i].temp;
            if (sensors[i].temp > stats->max_cpu_temp) {
                stats->max_cpu_temp = sensors[i].temp;
            }
            stats->cpu_count++;
        } else if (sensors[i].type == 1) { // GPU
            stats->avg_gpu_temp += sensors[i].temp;
            stats->gpu_count++;
        }
    }
    
    if (stats->cpu_count > 0) {
        stats->avg_cpu_temp /= stats->cpu_count;
    }
    if (stats->gpu_count > 0) {
        stats->avg_gpu_temp /= stats->gpu_count;
    }
}

// Fungsi untuk mendapatkan nama tipe sensor
const char* get_type_name(int type) {
    switch (type) {
        case 0: return "CPU";
        case 1: return "GPU";
        case 2: return "NVMe";
        case 3: return "Chipset";
        default: return "Other";
    }
}

// Fungsi utama untuk menampilkan suhu
void display_temperatures(TempSensor *sensors, int count, int show_stats) {
    char time_str[64];
    get_current_time(time_str, sizeof(time_str));
    
    int rows, cols;
    get_terminal_size(&rows, &cols);
    
    clear_screen();
    
    // Header dengan border dinamis
    int header_width = (cols > 80) ? 80 : cols - 2;
    
    printf("\033[1;36m╔");
    for (int i = 0; i < header_width - 2; i++) printf("═");
    printf("╗\033[0m\n");
    
    printf("\033[1;36m║\033[0m");
    int title_padding = (header_width - 50) / 2;
    for (int i = 0; i < title_padding; i++) printf(" ");
    printf("\033[1;33m★ ADVANCED HARDWARE TEMPERATURE MONITOR v%s ★\033[0m", VERSION);
    for (int i = 0; i < title_padding; i++) printf(" ");
    printf("\033[1;36m║\033[0m\n");
    
    printf("\033[1;36m║\033[0m  %s  |  MX Linux Compatible", time_str);
    int padding = header_width - 57;
    for (int i = 0; i < padding; i++) printf(" ");
    printf("\033[1;36m║\033[0m\n");
    
    printf("\033[1;36m╚");
    for (int i = 0; i < header_width - 2; i++) printf("═");
    printf("╝\033[0m\n\n");
    
    // Kelompokkan berdasarkan tipe
    const char *type_names[] = {"CPU", "GPU", "NVME", "CHIPSET", "OTHER"};
    const char *type_colors[] = {"\033[1;32m", "\033[1;35m", "\033[1;34m", "\033[1;33m", "\033[1;37m"};
    
    for (int type = 0; type < 5; type++) {
        int found = 0;
        
        // Hitung jumlah sensor tipe ini
        for (int i = 0; i < count; i++) {
            if (sensors[i].type == type && sensors[i].temp > -500) {
                found++;
            }
        }
        
        if (found == 0 && type != 0) continue; // Skip jika tidak ada, kecuali CPU
        
        printf("%s▓▓▓ %s TEMPERATURES (%d sensor%s) ▓▓▓\033[0m\n", 
               type_colors[type], type_names[type], found, found != 1 ? "s" : "");
        
        if (found == 0) {
            printf("  \033[90mTidak ada sensor %s terdeteksi\033[0m\n\n", type_names[type]);
            continue;
        }
        
        for (int i = 0; i < count; i++) {
            if (sensors[i].type == type) {
                double temp = read_temperature(sensors[i].path);
                
                if (temp > -500) {
                    sensors[i].temp = temp;
                    
                    // Update min/max
                    if (temp > sensors[i].max_temp) sensors[i].max_temp = temp;
                    if (temp < sensors[i].min_temp) sensors[i].min_temp = temp;
                    
                    printf("  %-30s  ", sensors[i].label);
                    print_temperature(temp);
                    printf("  ");
                    print_temp_bar(temp, 20);
                    
                    if (show_stats && sensors[i].max_temp > -500) {
                        printf(" [Min: ");
                        print_temperature(sensors[i].min_temp);
                        printf(" Max: ");
                        print_temperature(sensors[i].max_temp);
                        printf("]");
                    }
                    
                    printf("\n");
                }
            }
        }
        printf("\n");
    }
    
    // Tampilkan statistik
    if (show_stats) {
        SystemStats stats;
        calculate_stats(sensors, count, &stats);
        
        printf("\033[1;36m━━━━━━━━━━━━━━━━━━━━━━━ SYSTEM STATISTICS ━━━━━━━━━━━━━━━━━━━━━━━\033[0m\n");
        if (stats.cpu_count > 0) {
            printf("  CPU Average: ");
            print_temperature(stats.avg_cpu_temp);
            printf("  |  CPU Peak: ");
            print_temperature(stats.max_cpu_temp);
            printf("\n");
        }
        if (stats.gpu_count > 0) {
            printf("  GPU Average: ");
            print_temperature(stats.avg_gpu_temp);
            printf("\n");
        }
        printf("  Total Active Sensors: %d\n", stats.total_sensors);
        printf("\033[1;36m━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\033[0m\n");
    }
    
    printf("\n\033[1;36m══════════════════════════════════════════════════════════════════\033[0m\n");
    printf("Legend: \033[36m<40°C\033[0m \033[32m40-50°C\033[0m \033[92m50-60°C\033[0m \033[33m60-70°C\033[0m \033[93m70-80°C\033[0m \033[91m80-90°C\033[0m \033[31;1m>90°C\033[0m\n");
    printf("Controls: \033[1;31mCtrl+C\033[0m=Exit | \033[1;32mF/C\033[0m=Toggle °F/°C | \033[1;33mS\033[0m=Stats\n");
}

// Fungsi untuk menampilkan bantuan
void print_help(const char *prog_name) {
    printf("\n\033[1;33m╔══════════════════════════════════════════════════════════════╗\033[0m\n");
    printf("\033[1;33m║\033[0m  Advanced Hardware Temperature Monitor v%s              \033[1;33m║\033[0m\n", VERSION);
    printf("\033[1;33m╚══════════════════════════════════════════════════════════════╝\033[0m\n\n");
    
    printf("\033[1;32mPenggunaan:\033[0m\n");
    printf("  %s [options] [refresh_rate]\n\n", prog_name);
    
    printf("\033[1;32mOptions:\033[0m\n");
    printf("  -h, --help        Tampilkan bantuan ini\n");
    printf("  -v, --version     Tampilkan versi program\n");
    printf("  -f, --fahrenheit  Gunakan Fahrenheit (default: Celsius)\n");
    printf("  -s, --stats       Tampilkan statistik tambahan\n");
    printf("  -l, --list        List semua sensor yang terdeteksi\n\n");
    
    printf("\033[1;32mArguments:\033[0m\n");
    printf("  refresh_rate      Waktu refresh dalam detik (1-60, default: 2)\n\n");
    
    printf("\033[1;32mContoh:\033[0m\n");
    printf("  %s                # Jalankan dengan default (2 detik)\n", prog_name);
    printf("  %s 1              # Update setiap 1 detik\n", prog_name);
    printf("  %s -s 3           # Tampilkan stats, update setiap 3 detik\n", prog_name);
    printf("  %s -f 5           # Gunakan Fahrenheit, update setiap 5 detik\n\n", prog_name);
}

// Fungsi untuk list semua sensor
void list_sensors(TempSensor *sensors, int count) {
    printf("\n\033[1;36m╔══════════════════════════════════════════════════════════════╗\033[0m\n");
    printf("\033[1;36m║\033[0m  Detected Temperature Sensors: %d                         \033[1;36m║\033[0m\n", count);
    printf("\033[1;36m╚══════════════════════════════════════════════════════════════╝\033[0m\n\n");
    
    for (int i = 0; i < count; i++) {
        printf("\033[1;33m[%2d]\033[0m %-10s | %-30s | %s\n", 
               i + 1, get_type_name(sensors[i].type), sensors[i].label, sensors[i].path);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    TempSensor sensors[MAX_SENSORS];
    int sensor_count;
    int refresh_rate = 2;
    int show_stats = 0;
    int list_mode = 0;
    
    // Setup signal handler
    signal(SIGINT, sigint_handler);
    
    // Parse arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_help(argv[0]);
            return 0;
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            printf("Advanced Hardware Temperature Monitor v%s\n", VERSION);
            return 0;
        } else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--fahrenheit") == 0) {
            use_celsius = 0;
        } else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--stats") == 0) {
            show_stats = 1;
        } else if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--list") == 0) {
            list_mode = 1;
        } else {
            int rate = atoi(argv[i]);
            if (rate >= 1 && rate <= 60) {
                refresh_rate = rate;
            }
        }
    }
    
    printf("\033[1;33m⚡ Scanning temperature sensors...\033[0m\n");
    sensor_count = scan_temperature_sensors(sensors);
    
    if (sensor_count == 0) {
        fprintf(stderr, "\n\033[1;31m✗ Error: Tidak ada sensor suhu yang terdeteksi!\033[0m\n\n");
        fprintf(stderr, "\033[1;33m📋 Solusi yang bisa dicoba:\033[0m\n\n");
        fprintf(stderr, "1. \033[1;32mLoad modul kernel:\033[0m\n");
        fprintf(stderr, "   sudo modprobe coretemp        # Intel CPU\n");
        fprintf(stderr, "   sudo modprobe k10temp         # AMD CPU\n");
        fprintf(stderr, "   sudo modprobe zenpower        # AMD Ryzen (alternatif)\n\n");
        fprintf(stderr, "2. \033[1;32mInstall dan konfigurasi lm-sensors:\033[0m\n");
        fprintf(stderr, "   sudo apt install lm-sensors\n");
        fprintf(stderr, "   sudo sensors-detect\n");
        fprintf(stderr, "   sudo systemctl restart kmod\n\n");
        fprintf(stderr, "3. \033[1;32mVerifikasi sensor:\033[0m\n");
        fprintf(stderr, "   ls -la /sys/class/hwmon/\n");
        fprintf(stderr, "   sensors\n\n");
        fprintf(stderr, "4. \033[1;32mLihat README.md untuk panduan lengkap\033[0m\n\n");
        return 1;
    }
    
    printf("\033[1;32m✓ Ditemukan %d sensor suhu!\033[0m\n", sensor_count);
    sleep(1);
    
    // Mode list
    if (list_mode) {
        list_sensors(sensors, sensor_count);
        return 0;
    }
    
    // Loop realtime
    printf("\033[1;36m⟳ Starting realtime monitoring (refresh: %ds)...\033[0m\n", refresh_rate);
    sleep(1);
    
    while (keep_running) {
        display_temperatures(sensors, sensor_count, show_stats);
        sleep(refresh_rate);
    }
    
    // Cleanup
    clear_screen();
    printf("\n\033[1;33m👋 Temperature monitoring stopped. Goodbye!\033[0m\n\n");
    
    return 0;
}
