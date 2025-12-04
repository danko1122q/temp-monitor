#include "display.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <time.h>

void clear_screen(void) {
    printf("\033[2J\033[H");
    fflush(stdout);
}

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

void hide_cursor(void) {
    printf("\033[?25l");
    fflush(stdout);
}

void show_cursor(void) {
    printf("\033[?25h");
    fflush(stdout);
}

double celsius_to_fahrenheit(double celsius) {
    return (celsius * 9.0 / 5.0) + 32.0;
}

void print_temperature(double temp, int use_celsius) {
    if (temp < -500) {
        printf("  N/A   ");
        return;
    }
    
    if (use_celsius) {
        printf("%6.1f°C", temp);
    } else {
        printf("%6.1f°F", celsius_to_fahrenheit(temp));
    }
}

void get_current_time(char *buffer, size_t size) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", tm_info);
}

void print_separator(int width, char style) {
    const char *chars;
    switch (style) {
        case '=': chars = "═"; break;
        case '-': chars = "─"; break;
        case '~': chars = "━"; break;
        default:  chars = "─"; break;
    }
    
    for (int i = 0; i < width; i++) {
        printf("%s", chars);
    }
}

void print_header(const char *version) {
    int rows, cols;
    get_terminal_size(&rows, &cols);
    
    int width = (cols > 100) ? 100 : cols - 4;
    
    printf("\n");
    printf(COLOR_BRIGHT_CYAN "╔");
    print_separator(width - 2, '=');
    printf("╗\n" COLOR_RESET);
    
    printf(COLOR_BRIGHT_CYAN "║" COLOR_RESET);
    int title_len = 45 + strlen(version);
    int padding = (width - title_len - 2) / 2;
    
    for (int i = 0; i < padding; i++) printf(" ");
    printf(COLOR_BOLD COLOR_BRIGHT_YELLOW "🔥 ULTIMATE HARDWARE TEMPERATURE MONITOR");
    printf(COLOR_BRIGHT_WHITE " v%s" COLOR_RESET, version);
    for (int i = 0; i < padding; i++) printf(" ");
    if ((width - title_len - 2) % 2) printf(" ");
    printf(COLOR_BRIGHT_CYAN "║\n" COLOR_RESET);
    
    char time_str[64];
    get_current_time(time_str, sizeof(time_str));
    
    printf(COLOR_BRIGHT_CYAN "║" COLOR_RESET);
    printf("  " COLOR_CYAN "⏰ %s" COLOR_RESET "  │  " 
           COLOR_GREEN "✓ Real-time Monitoring" COLOR_RESET, time_str);
    int pad = width - 58;
    for (int i = 0; i < pad; i++) printf(" ");
    printf(COLOR_BRIGHT_CYAN "║\n" COLOR_RESET);
    
    printf(COLOR_BRIGHT_CYAN "╚");
    print_separator(width - 2, '=');
    printf("╝\n" COLOR_RESET);
    printf("\n");
}

void print_footer(DisplayConfig *config) {
    int rows, cols;
    get_terminal_size(&rows, &cols);
    int width = (cols > 100) ? 100 : cols - 4;
    
    printf("\n");
    printf(COLOR_BRIGHT_CYAN);
    print_separator(width, '=');
    printf(COLOR_RESET "\n");
    
    printf(COLOR_BRIGHT_CYAN "Temperature Ranges: " COLOR_RESET);
    printf(COLOR_CYAN "<40°C " COLOR_RESET);
    printf(COLOR_GREEN "40-50°C " COLOR_RESET);
    printf(COLOR_BRIGHT_GREEN "50-60°C " COLOR_RESET);
    printf(COLOR_YELLOW "60-70°C " COLOR_RESET);
    printf(COLOR_BRIGHT_YELLOW "70-80°C " COLOR_RESET);
    printf(COLOR_BRIGHT_RED "80-90°C " COLOR_RESET);
    printf(COLOR_RED COLOR_BOLD ">90°C\n" COLOR_RESET);
    
    printf(COLOR_BRIGHT_WHITE "Controls: " COLOR_RESET);
    printf(COLOR_RED "Ctrl+C" COLOR_RESET "=Exit │ ");
    printf(COLOR_GREEN "F/C" COLOR_RESET "=Toggle Unit │ ");
    printf(COLOR_YELLOW "S" COLOR_RESET "=Stats │ ");
    printf("Refresh: " COLOR_CYAN "%ds\n" COLOR_RESET, config->refresh_rate);
}

void print_temp_bar(double temp, int width, int use_gradient) {
    if (temp < -500) {
        printf(COLOR_BRIGHT_BLACK "[");
        for (int i = 0; i < width; i++) printf("░");
        printf("]" COLOR_RESET);
        return;
    }
    
    int filled = (temp >= 0 && temp <= 100) ? (int)((temp / 100.0) * width) : 
                 (temp > 100 ? width : 0);
    
    const char *color;
    const char *bar_char;
    
    if (temp < 40) {
        color = COLOR_CYAN;
        bar_char = "█";
    } else if (temp < 50) {
        color = COLOR_GREEN;
        bar_char = "█";
    } else if (temp < 60) {
        color = COLOR_BRIGHT_GREEN;
        bar_char = "█";
    } else if (temp < 70) {
        color = COLOR_YELLOW;
        bar_char = "▓";
    } else if (temp < 80) {
        color = COLOR_BRIGHT_YELLOW;
        bar_char = "▓";
    } else if (temp < 90) {
        color = COLOR_BRIGHT_RED;
        bar_char = "▒";
    } else {
        color = COLOR_RED COLOR_BOLD;
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
    printf("]" COLOR_RESET);
}

void display_sensor_group(TempSensor *sensors, int count, SensorType type, DisplayConfig *config) {
    int found = 0;
    
    for (int i = 0; i < count; i++) {
        if (sensors[i].type == type && sensors[i].active) {
            found++;
        }
    }
    
    if (found == 0) return;
    
    const char *type_name = get_type_name(type);
    const char *icon = get_type_icon(type);
    
    printf("\n");
    printf(COLOR_BOLD COLOR_BRIGHT_CYAN "┌─ %s %s SENSORS ", icon, type_name);
    printf(COLOR_BRIGHT_BLACK "(%d detected) ", found);
    print_separator(40, '─');
    printf(COLOR_RESET "\n");
    
    for (int i = 0; i < count; i++) {
        if (sensors[i].type != type || !sensors[i].active) continue;
        
        update_sensor_data(&sensors[i]);
        
        const char *status_color = get_status_color(sensors[i].status);
        
        printf(COLOR_BRIGHT_WHITE "│ " COLOR_RESET);
        printf("%-32s ", sensors[i].label);
        
        printf("%s", status_color);
        print_temperature(sensors[i].temp_current, config->use_celsius);
        printf(COLOR_RESET " ");
        
        print_temp_bar(sensors[i].temp_current, 25, 1);
        
        if (config->show_stats) {
            printf(" " COLOR_BRIGHT_BLACK "[");
            print_temperature(sensors[i].temp_min, config->use_celsius);
            printf("▸");
            print_temperature(sensors[i].temp_max, config->use_celsius);
            printf("]" COLOR_RESET);
        }
        
        if (sensors[i].status == STATUS_CRITICAL) {
            printf(" " COLOR_RED "⚠ CRITICAL!" COLOR_RESET);
        } else if (sensors[i].status == STATUS_WARN) {
            printf(" " COLOR_YELLOW "⚠ High" COLOR_RESET);
        }
        
        printf("\n");
    }
}

void display_all_sensors(TempSensor *sensors, int count, DisplayConfig *config) {
    SensorType types[] = {SENSOR_CPU, SENSOR_GPU, SENSOR_NVME, 
                          SENSOR_CHIPSET, SENSOR_MEMORY, SENSOR_VRM, 
                          SENSOR_DISK, SENSOR_OTHER};
    
    for (int i = 0; i < 8; i++) {
        display_sensor_group(sensors, count, types[i], config);
    }
}

void display_statistics(SystemStats *stats, DisplayConfig *config) {
    printf("\n");
    printf(COLOR_BRIGHT_CYAN "┌─ 📊 SYSTEM STATISTICS ");
    print_separator(60, '─');
    printf(COLOR_RESET "\n");
    
    if (stats->cpu_count > 0) {
        printf(COLOR_BRIGHT_WHITE "│ " COLOR_RESET);
        printf(COLOR_GREEN "CPU Statistics:" COLOR_RESET);
        printf("  Average: ");
        print_temperature(stats->avg_cpu_temp, config->use_celsius);
        printf("  │  Peak: ");
        print_temperature(stats->max_cpu_temp, config->use_celsius);
        printf("  │  Min: ");
        print_temperature(stats->min_cpu_temp, config->use_celsius);
        printf("\n");
    }
    
    if (stats->gpu_count > 0) {
        printf(COLOR_BRIGHT_WHITE "│ " COLOR_RESET);
        printf(COLOR_MAGENTA "GPU Statistics:" COLOR_RESET);
        printf("  Average: ");
        print_temperature(stats->avg_gpu_temp, config->use_celsius);
        printf("  │  Peak: ");
        print_temperature(stats->max_gpu_temp, config->use_celsius);
        printf("\n");
    }
    
    if (stats->nvme_count > 0) {
        printf(COLOR_BRIGHT_WHITE "│ " COLOR_RESET);
        printf(COLOR_BLUE "NVMe Statistics:" COLOR_RESET);
        printf(" Average: ");
        print_temperature(stats->avg_nvme_temp, config->use_celsius);
        printf("\n");
    }
    
    printf(COLOR_BRIGHT_WHITE "│ " COLOR_RESET);
    printf(COLOR_CYAN "System Status:" COLOR_RESET);
    printf("   Active Sensors: " COLOR_BRIGHT_GREEN "%d" COLOR_RESET, stats->total_active_sensors);
    
    if (stats->warnings > 0) {
        printf("  │  " COLOR_YELLOW "⚠ Warnings: %d" COLOR_RESET, stats->warnings);
    }
    
    if (stats->criticals > 0) {
        printf("  │  " COLOR_RED "🔥 Critical: %d" COLOR_RESET, stats->criticals);
    }
    
    printf("\n");
    
    printf(COLOR_BRIGHT_CYAN "└");
    print_separator(84, '─');
    printf(COLOR_RESET "\n");
}

void display_sensor_list(TempSensor *sensors, int count) {
    printf("\n");
    printf(COLOR_BRIGHT_CYAN "╔══════════════════════════════════════════════════════════════════════════╗\n");
    printf("║" COLOR_RESET " " COLOR_BOLD "DETECTED TEMPERATURE SENSORS" COLOR_RESET);
    printf(" " COLOR_BRIGHT_BLACK "(%d total)" COLOR_RESET, count);
    printf("                                  " COLOR_BRIGHT_CYAN "║\n");
    printf("╠══════════════════════════════════════════════════════════════════════════╣\n" COLOR_RESET);
    
    for (int i = 0; i < count; i++) {
        printf(COLOR_BRIGHT_CYAN "║" COLOR_RESET);
        printf(" " COLOR_YELLOW "[%3d]" COLOR_RESET, i + 1);
        printf(" %-10s │ %-35s │ ", get_type_name(sensors[i].type), sensors[i].label);
        printf(COLOR_BRIGHT_BLACK "%s" COLOR_RESET, sensors[i].name);
        
        int padding = 76 - 52 - strlen(sensors[i].name);
        for (int j = 0; j < padding; j++) printf(" ");
        printf(COLOR_BRIGHT_CYAN "║\n" COLOR_RESET);
    }
    
    printf(COLOR_BRIGHT_CYAN "╚══════════════════════════════════════════════════════════════════════════╝\n" COLOR_RESET);
    printf("\n");
}
