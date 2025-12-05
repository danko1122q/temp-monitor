/**
 * @file main.c
 * @brief Temp Monitor - Main Entry Point
 * 
 * This file contains the main entry point for the temperature monitoring
 * application. It handles command-line argument parsing, signal handling,
 * sensor initialization, and the main monitoring loop.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "sensor.h"
#include "display.h"
#include "utils.h"

/* Application version */
#define VERSION "0.0.2"

/* Global flag for graceful shutdown on SIGINT (Ctrl+C) */
volatile sig_atomic_t keep_running = 1;

/* Array to store all detected temperature sensors */
TempSensor sensors[MAX_SENSORS];

/* Number of detected sensors */
int sensor_count = 0;

/* Display configuration with default values */
DisplayConfig config = {
    .use_celsius = 1,
    .show_stats = 0,
    .show_graphs = 0,
    .show_fans = 1,
    .compact_mode = 0,
    .color_mode = 1,
    .refresh_rate = 2
};

/**
 * @brief Signal handler for SIGINT (Ctrl+C)
 * 
 * Sets keep_running to 0 to trigger graceful shutdown of the monitoring loop.
 * 
 * @param sig Signal number (unused)
 */
void sigint_handler(int sig) {
    (void)sig;
    keep_running = 0;
}

/**
 * @brief Prints the help message with usage information
 * 
 * Displays detailed help including options, arguments, examples,
 * keyboard controls, features, and troubleshooting tips.
 * 
 * @param prog_name Name of the program executable
 */
void print_help(const char *prog_name) {
    printf("\n");
    printf(COLOR_BRIGHT_CYAN "==========================================================\n");
    printf("  " COLOR_RESET COLOR_BOLD COLOR_BRIGHT_WHITE "Temp Monitor" COLOR_RESET " v%s\n", VERSION);
    printf(COLOR_BRIGHT_CYAN "==========================================================\n" COLOR_RESET);
    printf("  Real-time hardware temperature monitoring for Linux\n");
    printf(COLOR_BRIGHT_CYAN "==========================================================\n" COLOR_RESET);
    
    printf("\n" COLOR_BOLD COLOR_GREEN "USAGE:\n" COLOR_RESET);
    printf("  %s [OPTIONS] [REFRESH_RATE]\n\n", prog_name);
    
    printf(COLOR_BOLD COLOR_GREEN "OPTIONS:\n" COLOR_RESET);
    printf("  " COLOR_YELLOW "-h, --help" COLOR_RESET "          Show this help message\n");
    printf("  " COLOR_YELLOW "-v, --version" COLOR_RESET "       Show version information\n");
    printf("  " COLOR_YELLOW "-f, --fahrenheit" COLOR_RESET "    Use Fahrenheit instead of Celsius\n");
    printf("  " COLOR_YELLOW "-s, --stats" COLOR_RESET "         Show detailed statistics\n");
    printf("  " COLOR_YELLOW "-l, --list" COLOR_RESET "          List all detected sensors and exit\n");
    printf("  " COLOR_YELLOW "-c, --compact" COLOR_RESET "       Use compact display mode\n");
    printf("  " COLOR_YELLOW "-F, --fans" COLOR_RESET "          Show fan speed monitoring\n");
    printf("  " COLOR_YELLOW "-n, --no-fans" COLOR_RESET "       Disable fan speed monitoring\n");
    printf("  " COLOR_YELLOW "-g, --graphs" COLOR_RESET "        Show temperature graphs (coming soon)\n");
    
    printf("\n" COLOR_BOLD COLOR_GREEN "ARGUMENTS:\n" COLOR_RESET);
    printf("  " COLOR_CYAN "REFRESH_RATE" COLOR_RESET "        Update interval in seconds (1-60, default: 2)\n");
    
    printf("\n" COLOR_BOLD COLOR_GREEN "EXAMPLES:\n" COLOR_RESET);
    printf("  %s                   # Run with default settings (2s refresh)\n", prog_name);
    printf("  %s 1                 # Update every 1 second\n", prog_name);
    printf("  %s -s 3              # Show stats, update every 3 seconds\n", prog_name);
    printf("  %s -f -s 5           # Fahrenheit + stats, 5 second refresh\n", prog_name);
    printf("  %s --list            # List all sensors\n\n", prog_name);
    
    printf(COLOR_BOLD COLOR_CYAN "KEYBOARD CONTROLS (during monitoring):\n" COLOR_RESET);
    printf("  " COLOR_RED "Ctrl+C" COLOR_RESET "              Exit the program\n");
    printf("  " COLOR_GREEN "F" COLOR_RESET " / " COLOR_GREEN "C" COLOR_RESET "               Toggle between Fahrenheit/Celsius\n");
    printf("  " COLOR_YELLOW "S" COLOR_RESET "                   Toggle statistics display\n");
    
    printf("\n" COLOR_BOLD COLOR_GREEN "SUPPORTED SENSORS:\n" COLOR_RESET);
    printf("  CPU, GPU, NVMe, Chipset, Memory, VRM, Disk\n");
    
    printf("\n" COLOR_BOLD COLOR_YELLOW "TROUBLESHOOTING:\n" COLOR_RESET);
    printf("  No sensors? Try: sudo modprobe coretemp k10temp\n");
    printf("  Or install lm-sensors and run: sudo sensors-detect\n\n");
}

/**
 * @brief Prints version information
 * 
 * Displays the application name, version, author, and license.
 */
void print_version(void) {
    printf("\n");
    printf(COLOR_BOLD COLOR_BRIGHT_WHITE "Temp Monitor\n" COLOR_RESET);
    printf(COLOR_BRIGHT_BLACK "Version: " COLOR_RESET "%s\n", VERSION);
    printf(COLOR_BRIGHT_BLACK "Author:  " COLOR_RESET "Danko\n");
    printf(COLOR_BRIGHT_BLACK "License: " COLOR_RESET "MIT\n");
    printf("\n");
}

/**
 * @brief Main monitoring loop
 * 
 * Enters alternate screen buffer to prevent display artifacts,
 * then continuously updates and displays sensor data until
 * the user presses Ctrl+C. Uses alternate screen buffer technique
 * similar to vim/htop to keep terminal clean.
 */
void run_monitoring(void) {
    /* Switch to alternate screen buffer for clean display */
    enter_alternate_screen();
    hide_cursor();
    
    while (keep_running) {
        clear_screen();
        print_header(VERSION);
        
        display_all_sensors(sensors, sensor_count, &config);
        
        if (config.show_stats) {
            SystemStats stats;
            calculate_system_stats(sensors, sensor_count, &stats);
            display_statistics(&stats, &config);
        }
        
        print_footer(&config);
        
        sleep(config.refresh_rate);
    }
    
    show_cursor();
    exit_alternate_screen();
}

/**
 * @brief Initializes and scans for temperature sensors
 * 
 * Scans the hwmon and thermal subsystems for available temperature
 * sensors and populates the global sensors array.
 * 
 * @return 1 if sensors were found, 0 otherwise
 */
int initialize_sensors(void) {
    printf(COLOR_BRIGHT_YELLOW "[*] Initializing temperature monitoring system...\n" COLOR_RESET);
    printf(COLOR_CYAN "[~] Scanning for hardware sensors...\n" COLOR_RESET);
    
    sensor_count = scan_temperature_sensors(sensors);
    
    if (sensor_count == 0) {
        printf("\n");
        printf(COLOR_RED COLOR_BOLD "[X] ERROR: No temperature sensors detected!\n" COLOR_RESET);
        printf("\n");
        printf(COLOR_YELLOW "TROUBLESHOOTING STEPS:\n" COLOR_RESET);
        printf("\n");
        
        printf(COLOR_BRIGHT_WHITE "1. Load Kernel Modules:\n" COLOR_RESET);
        printf("   " COLOR_CYAN "# For Intel CPUs:\n" COLOR_RESET);
        printf("   " COLOR_BRIGHT_BLACK "sudo modprobe coretemp\n" COLOR_RESET);
        printf("   " COLOR_CYAN "# For AMD CPUs:\n" COLOR_RESET);
        printf("   " COLOR_BRIGHT_BLACK "sudo modprobe k10temp\n" COLOR_RESET);
        printf("   " COLOR_CYAN "# For AMD Ryzen (alternative):\n" COLOR_RESET);
        printf("   " COLOR_BRIGHT_BLACK "sudo modprobe zenpower\n" COLOR_RESET);
        printf("\n");
        
        printf(COLOR_BRIGHT_WHITE "2. Install and Configure lm-sensors:\n" COLOR_RESET);
        printf("   " COLOR_BRIGHT_BLACK "sudo apt install lm-sensors\n" COLOR_RESET);
        printf("   " COLOR_BRIGHT_BLACK "sudo sensors-detect\n" COLOR_RESET);
        printf("   " COLOR_BRIGHT_BLACK "sudo systemctl restart kmod\n" COLOR_RESET);
        printf("\n");
        
        printf(COLOR_BRIGHT_WHITE "3. Verify Sensors:\n" COLOR_RESET);
        printf("   " COLOR_BRIGHT_BLACK "ls -la /sys/class/hwmon/\n" COLOR_RESET);
        printf("   " COLOR_BRIGHT_BLACK "sensors\n" COLOR_RESET);
        printf("\n");
        
        printf(COLOR_BRIGHT_WHITE "4. Check Permissions:\n" COLOR_RESET);
        printf("   Make sure you have read access to /sys/class/hwmon/\n");
        printf("\n");
        
        return 0;
    }
    
    printf(COLOR_GREEN "[+] Successfully detected %d temperature sensor%s!\n" COLOR_RESET, 
           sensor_count, sensor_count != 1 ? "s" : "");
    
    int cpu_sensors = 0, gpu_sensors = 0, nvme_sensors = 0, other_sensors = 0;
    for (int i = 0; i < sensor_count; i++) {
        switch (sensors[i].type) {
            case SENSOR_CPU: cpu_sensors++; break;
            case SENSOR_GPU: gpu_sensors++; break;
            case SENSOR_NVME: nvme_sensors++; break;
            default: other_sensors++; break;
        }
    }
    
    printf(COLOR_CYAN "    CPU: %d  |  GPU: %d  |  NVMe: %d  |  Other: %d\n" COLOR_RESET,
           cpu_sensors, gpu_sensors, nvme_sensors, other_sensors);
    
    return 1;
}

/**
 * @brief Parses command-line arguments
 * 
 * Processes all command-line options and sets the appropriate
 * configuration values. Supports short (-h) and long (--help) options.
 * 
 * @param argc Argument count
 * @param argv Argument vector
 */
void parse_arguments(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_help(argv[0]);
            exit(0);
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            print_version();
            exit(0);
        } else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--fahrenheit") == 0) {
            config.use_celsius = 0;
        } else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--stats") == 0) {
            config.show_stats = 1;
        } else if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--list") == 0) {
            if (!initialize_sensors()) exit(1);
            display_sensor_list(sensors, sensor_count);
            exit(0);
        } else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--compact") == 0) {
            config.compact_mode = 1;
        } else if (strcmp(argv[i], "-F") == 0 || strcmp(argv[i], "--fans") == 0) {
            config.show_fans = 1;
        } else if (strcmp(argv[i], "-n") == 0 || strcmp(argv[i], "--no-fans") == 0) {
            config.show_fans = 0;
        } else if (strcmp(argv[i], "-g") == 0 || strcmp(argv[i], "--graphs") == 0) {
            config.show_graphs = 1;
            printf(COLOR_YELLOW "Note: Graph feature is coming soon!\n" COLOR_RESET);
        } else {
            int rate = parse_int(argv[i], -1);
            if (rate >= 1 && rate <= 60) {
                config.refresh_rate = rate;
            } else if (rate != -1) {
                printf(COLOR_RED "Error: Invalid refresh rate '%s'. Must be between 1-60.\n" COLOR_RESET, argv[i]);
                exit(1);
            }
        }
    }
}

/**
 * @brief Main entry point
 * 
 * Program flow:
 * 1. Setup signal handler for graceful shutdown
 * 2. Parse command-line arguments
 * 3. Initialize and scan for sensors
 * 4. Run the monitoring loop
 * 5. Cleanup and exit
 * 
 * @param argc Argument count
 * @param argv Argument vector
 * @return 0 on success, 1 on failure
 */
int main(int argc, char *argv[]) {
    /* Setup signal handler for Ctrl+C */
    signal(SIGINT, sigint_handler);
    
    /* Parse command-line arguments */
    parse_arguments(argc, argv);
    
    if (!initialize_sensors()) {
        return 1;
    }
    
    sleep(1);
    
    printf(COLOR_BRIGHT_CYAN "[~] Starting real-time monitoring" COLOR_RESET);
    printf(COLOR_BRIGHT_BLACK " (refresh rate: %ds)...\n" COLOR_RESET, config.refresh_rate);
    sleep(1);
    
    run_monitoring();
    
    printf("\n");
    printf(COLOR_BRIGHT_WHITE "Monitoring stopped.\n" COLOR_RESET);
    printf(COLOR_BRIGHT_BLACK "Temp Monitor v%s\n" COLOR_RESET, VERSION);
    printf("\n");
    
    return 0;
}
