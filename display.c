/**
 * @file display.c
 * @brief Display and UI rendering functions
 * 
 * This file contains all functions related to terminal display,
 * including screen management, temperature bars, sensor output,
 * and the alternate screen buffer implementation for clean display.
 * 
 * @version 0.0.2
 * @date 2024-12-05
 * 
 * MIT License
 * Copyright (c) 2024 Danko
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 */

#include "display.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <time.h>

/**
 * @brief Clears the terminal screen
 * 
 * Uses ANSI escape codes to:
 * - \033[H    : Move cursor to home position (top-left)
 * - \033[2J   : Clear entire screen
 * - \033[3J   : Clear scroll buffer (prevents ghost artifacts)
 */
void clear_screen(void) {
    printf("\033[H\033[2J\033[3J");
    fflush(stdout);
}

/**
 * @brief Enters the alternate screen buffer
 * 
 * Switches terminal to alternate screen buffer (like vim/htop).
 * This prevents any output from appearing in the terminal's
 * scroll history, keeping the terminal clean after exit.
 * 
 * Uses escape code \033[?1049h to save cursor and switch buffer.
 */
void enter_alternate_screen(void) {
    printf("\033[?1049h");
    printf("\033[H\033[2J\033[3J");
    fflush(stdout);
}

/**
 * @brief Exits the alternate screen buffer
 * 
 * Switches back to the main terminal screen buffer.
 * Restores the previous terminal content that was visible
 * before entering alternate screen.
 * 
 * Uses escape code \033[?1049l to restore cursor and switch back.
 */
void exit_alternate_screen(void) {
    printf("\033[?1049l");
    fflush(stdout);
}

/**
 * @brief Gets the current terminal dimensions
 * 
 * Uses ioctl to query terminal size. Falls back to 24x80
 * if the query fails (e.g., when output is piped).
 * 
 * @param rows Pointer to store number of rows
 * @param cols Pointer to store number of columns
 */
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

/**
 * @brief Hides the terminal cursor
 * 
 * Uses ANSI escape code to make cursor invisible.
 * Should be called when entering monitoring mode.
 */
void hide_cursor(void) {
    printf("\033[?25l");
    fflush(stdout);
}

/**
 * @brief Shows the terminal cursor
 * 
 * Restores cursor visibility. Must be called before
 * program exit to restore normal terminal behavior.
 */
void show_cursor(void) {
    printf("\033[?25h");
    fflush(stdout);
}

/**
 * @brief Converts Celsius to Fahrenheit
 * 
 * @param celsius Temperature in Celsius
 * @return Temperature in Fahrenheit
 */
double celsius_to_fahrenheit(double celsius) {
    return (celsius * 9.0 / 5.0) + 32.0;
}

/**
 * @brief Prints a formatted temperature value
 * 
 * Displays temperature with appropriate unit (C or F).
 * Shows "N/A" for invalid readings (< -500).
 * 
 * @param temp Temperature value in Celsius
 * @param use_celsius 1 for Celsius, 0 for Fahrenheit
 */
void print_temperature(double temp, int use_celsius) {
    if (temp < -500) {
        printf("  N/A   ");
        return;
    }
    
    if (use_celsius) {
        printf("%6.1fC", temp);
    } else {
        printf("%6.1fF", celsius_to_fahrenheit(temp));
    }
}

/**
 * @brief Gets the current date and time as formatted string
 * 
 * Format: YYYY-MM-DD HH:MM:SS
 * 
 * @param buffer Output buffer for the time string
 * @param size Size of the output buffer
 */
void get_current_time(char *buffer, size_t size) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", tm_info);
}

/**
 * @brief Prints a horizontal separator line
 * 
 * @param width Number of characters to print
 * @param style Style variant (1=equals, 2=dash, 3=hash)
 */
void print_separator(int width, int style) {
    const char *chars;
    switch (style) {
        case 1: chars = "="; break;
        case 2: chars = "-"; break;
        case 3: chars = "#"; break;
        default: chars = "-"; break;
    }
    
    for (int i = 0; i < width; i++) {
        printf("%s", chars);
    }
}

/**
 * @brief Prints the application header
 * 
 * Displays the title bar with version number and current time.
 * Adapts width based on terminal size.
 * 
 * @param version Version string to display
 */
void print_header(const char *version) {
    int rows, cols;
    get_terminal_size(&rows, &cols);
    
    int width = (cols > 100) ? 100 : cols - 4;
    if (width < 60) width = 60;
    
    printf("\n");
    printf(COLOR_BRIGHT_CYAN "+");
    print_separator(width - 2, 1);
    printf("+\n" COLOR_RESET);
    
    printf(COLOR_BRIGHT_CYAN "|" COLOR_RESET);
    int title_len = 16 + (int)strlen(version);
    int padding = (width - title_len - 2) / 2;
    
    for (int i = 0; i < padding; i++) printf(" ");
    printf(COLOR_BOLD COLOR_BRIGHT_WHITE "Temp Monitor");
    printf(COLOR_BRIGHT_BLACK " v%s" COLOR_RESET, version);
    for (int i = 0; i < padding; i++) printf(" ");
    if ((width - title_len - 2) % 2) printf(" ");
    printf(COLOR_BRIGHT_CYAN "|\n" COLOR_RESET);
    
    char time_str[64];
    get_current_time(time_str, sizeof(time_str));
    
    printf(COLOR_BRIGHT_CYAN "|" COLOR_RESET);
    printf("  " COLOR_CYAN "%s" COLOR_RESET "  |  " 
           COLOR_GREEN "[*] Real-time Monitoring" COLOR_RESET, time_str);
    int pad = width - 55;
    for (int i = 0; i < pad; i++) printf(" ");
    printf(COLOR_BRIGHT_CYAN "|\n" COLOR_RESET);
    
    printf(COLOR_BRIGHT_CYAN "+");
    print_separator(width - 2, 1);
    printf("+\n" COLOR_RESET);
    printf("\n");
}

/**
 * @brief Prints the footer with legend and controls
 * 
 * Shows temperature color ranges, keyboard controls,
 * and current refresh rate setting.
 * 
 * @param config Display configuration
 */
void print_footer(DisplayConfig *config) {
    int rows, cols;
    get_terminal_size(&rows, &cols);
    int width = (cols > 100) ? 100 : cols - 4;
    if (width < 60) width = 60;
    
    printf("\n");
    printf(COLOR_BRIGHT_CYAN);
    print_separator(width, 1);
    printf(COLOR_RESET "\n");
    
    printf(COLOR_BRIGHT_CYAN "Temperature Ranges: " COLOR_RESET);
    printf(COLOR_CYAN "<40C " COLOR_RESET);
    printf(COLOR_GREEN "40-50C " COLOR_RESET);
    printf(COLOR_BRIGHT_GREEN "50-60C " COLOR_RESET);
    printf(COLOR_YELLOW "60-70C " COLOR_RESET);
    printf(COLOR_BRIGHT_YELLOW "70-80C " COLOR_RESET);
    printf(COLOR_BRIGHT_RED "80-90C " COLOR_RESET);
    printf(COLOR_RED COLOR_BOLD ">90C\n" COLOR_RESET);
    
    printf(COLOR_BRIGHT_WHITE "Controls: " COLOR_RESET);
    printf(COLOR_RED "Ctrl+C" COLOR_RESET "=Exit | ");
    if (config->show_fans) {
        printf(COLOR_MAGENTA "Fan monitoring enabled" COLOR_RESET " | ");
    }
    printf("Refresh: " COLOR_CYAN "%ds\n" COLOR_RESET, config->refresh_rate);
}

/**
 * @brief Prints a color-coded temperature progress bar
 * 
 * Displays a visual bar representing temperature level.
 * Color changes based on temperature thresholds:
 * - Cyan: < 40C (Cool)
 * - Green: 40-60C (Normal)
 * - Yellow: 60-80C (Warm)
 * - Red: > 80C (Hot/Critical)
 * 
 * @param temp Current temperature in Celsius
 * @param width Width of the bar in characters
 * @param use_gradient Whether to use gradient colors (unused)
 */
void print_temp_bar(double temp, int width, int use_gradient) {
    (void)use_gradient;
    
    if (temp < -500) {
        printf(COLOR_BRIGHT_BLACK "[");
        for (int i = 0; i < width; i++) printf(".");
        printf("]" COLOR_RESET);
        return;
    }
    
    int filled = (temp >= 0 && temp <= 100) ? (int)((temp / 100.0) * width) : 
                 (temp > 100 ? width : 0);
    
    const char *color;
    const char *bar_char;
    
    if (temp < 40) {
        color = COLOR_CYAN;
        bar_char = "#";
    } else if (temp < 50) {
        color = COLOR_GREEN;
        bar_char = "#";
    } else if (temp < 60) {
        color = COLOR_BRIGHT_GREEN;
        bar_char = "#";
    } else if (temp < 70) {
        color = COLOR_YELLOW;
        bar_char = "=";
    } else if (temp < 80) {
        color = COLOR_BRIGHT_YELLOW;
        bar_char = "=";
    } else if (temp < 90) {
        color = COLOR_BRIGHT_RED;
        bar_char = "*";
    } else {
        color = COLOR_RED COLOR_BOLD;
        bar_char = "!";
    }
    
    printf("%s[", color);
    for (int i = 0; i < width; i++) {
        if (i < filled) {
            printf("%s", bar_char);
        } else {
            printf(".");
        }
    }
    printf("]" COLOR_RESET);
}

/**
 * @brief Prints fan speed information
 * 
 * Displays RPM and percentage with color coding.
 * Color indicates fan activity level.
 * 
 * @param rpm Fan speed in RPM
 * @param percent Fan speed as percentage of maximum
 */
void print_fan_speed(int rpm, int percent) {
    if (rpm < 0) {
        printf(COLOR_BRIGHT_BLACK "  N/A  " COLOR_RESET);
        return;
    }
    
    const char *color;
    if (percent < 30) {
        color = COLOR_GREEN;
    } else if (percent < 60) {
        color = COLOR_YELLOW;
    } else if (percent < 80) {
        color = COLOR_BRIGHT_YELLOW;
    } else {
        color = COLOR_RED;
    }
    
    printf("%s%5d RPM (%3d%%)" COLOR_RESET, color, rpm, percent);
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
    printf(COLOR_BOLD COLOR_BRIGHT_CYAN "+-- %s %s SENSORS ", icon, type_name);
    printf(COLOR_BRIGHT_BLACK "(%d detected) ", found);
    print_separator(40, 2);
    printf(COLOR_RESET "\n");
    
    for (int i = 0; i < count; i++) {
        if (sensors[i].type != type || !sensors[i].active) continue;
        
        update_sensor_data(&sensors[i]);
        
        const char *status_color = get_status_color(sensors[i].status);
        
        printf(COLOR_BRIGHT_WHITE "| " COLOR_RESET);
        printf("%-28s ", sensors[i].label);
        
        printf("%s", status_color);
        print_temperature(sensors[i].temp_current, config->use_celsius);
        printf(COLOR_RESET " ");
        
        print_temp_bar(sensors[i].temp_current, 20, 1);
        
        if (config->show_stats) {
            printf(" " COLOR_BRIGHT_BLACK "[");
            print_temperature(sensors[i].temp_min, config->use_celsius);
            printf("->");
            print_temperature(sensors[i].temp_max, config->use_celsius);
            printf("]" COLOR_RESET);
        }
        
        if (sensors[i].has_fan && config->show_fans) {
            printf(" ");
            print_fan_speed(sensors[i].fan_speed_rpm, sensors[i].fan_speed_percent);
        }
        
        if (sensors[i].status == STATUS_CRITICAL) {
            printf(" " COLOR_RED "[!] CRITICAL!" COLOR_RESET);
        } else if (sensors[i].status == STATUS_WARN) {
            printf(" " COLOR_YELLOW "[!] High" COLOR_RESET);
        }
        
        printf("\n");
    }
}

void display_fan_sensors(TempSensor *sensors, int count, DisplayConfig *config) {
    int found = 0;
    
    for (int i = 0; i < count; i++) {
        if (sensors[i].has_fan && sensors[i].fan_speed_rpm > 0) {
            found++;
        }
    }
    
    if (found == 0) return;
    
    printf("\n");
    printf(COLOR_BOLD COLOR_BRIGHT_MAGENTA "+-- [FAN] FAN SENSORS ");
    printf(COLOR_BRIGHT_BLACK "(%d detected) ", found);
    print_separator(40, 2);
    printf(COLOR_RESET "\n");
    
    for (int i = 0; i < count; i++) {
        if (!sensors[i].has_fan || sensors[i].fan_speed_rpm <= 0) continue;
        
        printf(COLOR_BRIGHT_WHITE "| " COLOR_RESET);
        printf("%-28s ", sensors[i].label);
        print_fan_speed(sensors[i].fan_speed_rpm, sensors[i].fan_speed_percent);
        
        printf(" [");
        int bar_width = 15;
        int filled = (sensors[i].fan_speed_percent * bar_width) / 100;
        for (int j = 0; j < bar_width; j++) {
            if (j < filled) {
                printf(COLOR_MAGENTA "=" COLOR_RESET);
            } else {
                printf(".");
            }
        }
        printf("]");
        
        printf("\n");
    }
    
    (void)config;
}

void display_all_sensors(TempSensor *sensors, int count, DisplayConfig *config) {
    SensorType types[] = {SENSOR_CPU, SENSOR_GPU, SENSOR_NVME, 
                          SENSOR_CHIPSET, SENSOR_MEMORY, SENSOR_VRM, 
                          SENSOR_DISK, SENSOR_OTHER};
    
    for (int i = 0; i < 8; i++) {
        display_sensor_group(sensors, count, types[i], config);
    }
    
    if (config->show_fans) {
        display_fan_sensors(sensors, count, config);
    }
}

void display_statistics(SystemStats *stats, DisplayConfig *config) {
    printf("\n");
    printf(COLOR_BRIGHT_CYAN "+-- [STATS] SYSTEM STATISTICS ");
    print_separator(54, 2);
    printf(COLOR_RESET "\n");
    
    if (stats->cpu_count > 0) {
        printf(COLOR_BRIGHT_WHITE "| " COLOR_RESET);
        printf(COLOR_GREEN "CPU Statistics:" COLOR_RESET);
        printf("  Average: ");
        print_temperature(stats->avg_cpu_temp, config->use_celsius);
        printf("  |  Peak: ");
        print_temperature(stats->max_cpu_temp, config->use_celsius);
        printf("  |  Min: ");
        print_temperature(stats->min_cpu_temp, config->use_celsius);
        printf("\n");
    }
    
    if (stats->gpu_count > 0) {
        printf(COLOR_BRIGHT_WHITE "| " COLOR_RESET);
        printf(COLOR_MAGENTA "GPU Statistics:" COLOR_RESET);
        printf("  Average: ");
        print_temperature(stats->avg_gpu_temp, config->use_celsius);
        printf("  |  Peak: ");
        print_temperature(stats->max_gpu_temp, config->use_celsius);
        printf("\n");
    }
    
    if (stats->nvme_count > 0) {
        printf(COLOR_BRIGHT_WHITE "| " COLOR_RESET);
        printf(COLOR_BLUE "NVMe Statistics:" COLOR_RESET);
        printf(" Average: ");
        print_temperature(stats->avg_nvme_temp, config->use_celsius);
        printf("\n");
    }
    
    printf(COLOR_BRIGHT_WHITE "| " COLOR_RESET);
    printf(COLOR_CYAN "System Status:" COLOR_RESET);
    printf("   Active Sensors: " COLOR_BRIGHT_GREEN "%d" COLOR_RESET, stats->total_active_sensors);
    
    if (stats->total_fans > 0) {
        printf("  |  " COLOR_MAGENTA "Fans: %d" COLOR_RESET, stats->total_fans);
    }
    
    if (stats->warnings > 0) {
        printf("  |  " COLOR_YELLOW "[!] Warnings: %d" COLOR_RESET, stats->warnings);
    }
    
    if (stats->criticals > 0) {
        printf("  |  " COLOR_RED "[!!] Critical: %d" COLOR_RESET, stats->criticals);
    }
    
    printf("\n");
    
    printf(COLOR_BRIGHT_CYAN "+");
    print_separator(84, 2);
    printf(COLOR_RESET "\n");
}

void display_sensor_list(TempSensor *sensors, int count) {
    printf("\n");
    printf(COLOR_BRIGHT_CYAN "+============================================================================+\n");
    printf("|" COLOR_RESET " " COLOR_BOLD "DETECTED TEMPERATURE SENSORS" COLOR_RESET);
    printf(" " COLOR_BRIGHT_BLACK "(%d total)" COLOR_RESET, count);
    printf("                                  " COLOR_BRIGHT_CYAN "|\n");
    printf("+============================================================================+\n" COLOR_RESET);
    
    for (int i = 0; i < count; i++) {
        printf(COLOR_BRIGHT_CYAN "|" COLOR_RESET);
        printf(" " COLOR_YELLOW "[%3d]" COLOR_RESET, i + 1);
        printf(" %-10s | %-30s | ", get_type_name(sensors[i].type), sensors[i].label);
        printf(COLOR_BRIGHT_BLACK "%s" COLOR_RESET, sensors[i].name);
        
        int name_len = (int)strlen(sensors[i].name);
        int padding = 76 - 47 - name_len;
        if (padding < 0) padding = 0;
        for (int j = 0; j < padding; j++) printf(" ");
        
        if (sensors[i].has_fan) {
            printf(COLOR_MAGENTA "[FAN]" COLOR_RESET);
        } else {
            printf("     ");
        }
        printf(COLOR_BRIGHT_CYAN "|\n" COLOR_RESET);
    }
    
    printf(COLOR_BRIGHT_CYAN "+============================================================================+\n" COLOR_RESET);
    printf("\n");
}
