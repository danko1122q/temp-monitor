/**
 * @file main.h
 * @brief Temp Monitor - Header file for main module
 *
 * Contains global definitions, configuration structures,
 * and function prototypes for the main application.
 *
 * @version 0.0.2
 * @date 2024-12-05
 *
 * MIT License
 * Copyright (c) 2024 Danko
 */

#include "display.h"
#include "sensor.h"
#include "utils.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Application version */
#define VERSION "0.0.2"

// Global variables
volatile sig_atomic_t keep_running = 1;
TempSensor            sensors[MAX_SENSORS];
int                   sensor_count = 0;
DisplayConfig         config       = {.use_celsius  = 1,
                        .show_stats   = 0,
                        .show_graphs  = 0,
                        .show_fans    = 0,
                        .compact_mode = 0,
                        .color_mode   = 1,
                        .refresh_rate = 2};

// Signal handler
void sigint_handler(int sig)
{
  (void) sig;
  keep_running = 0;
}

// Print help message
void print_help(const char *prog_name)
{
  printf("\n");
  printf(COLOR_BRIGHT_CYAN "==========================================================\n");
  printf("  " COLOR_RESET COLOR_BOLD COLOR_BRIGHT_WHITE "Temp Monitor" COLOR_RESET " v%s\n",
         VERSION);
  printf(COLOR_BRIGHT_CYAN
         "==========================================================\n" COLOR_RESET);
  printf("  Real-time hardware temperature monitoring for Linux\n");
  printf(COLOR_BRIGHT_CYAN
         "==========================================================\n" COLOR_RESET);

  printf("\n" COLOR_BOLD COLOR_GREEN "USAGE:\n" COLOR_RESET);
  printf("  %s [OPTIONS] [REFRESH_RATE]\n\n", prog_name);

  printf(COLOR_BOLD COLOR_GREEN "OPTIONS:\n" COLOR_RESET);
  printf("  " COLOR_YELLOW "-h, --help" COLOR_RESET "          Show this help message\n");
  printf("  " COLOR_YELLOW "-v, --version" COLOR_RESET "       Show version information\n");
  printf("  " COLOR_YELLOW "-f, --fahrenheit" COLOR_RESET
         "    Use Fahrenheit instead of Celsius\n");
  printf("  " COLOR_YELLOW "-s, --stats" COLOR_RESET "         Show detailed statistics\n");
  printf("  " COLOR_YELLOW "-l, --list" COLOR_RESET
         "          List all detected sensors and exit\n");
  printf("  " COLOR_YELLOW "-c, --compact" COLOR_RESET "       Use compact display mode\n");
  printf("  " COLOR_YELLOW "-g, --graphs" COLOR_RESET
         "        Show temperature graphs (coming soon)\n");

  printf("\n" COLOR_BOLD COLOR_GREEN "ARGUMENTS:\n" COLOR_RESET);
  printf("  " COLOR_CYAN "REFRESH_RATE" COLOR_RESET
         "        Update interval in seconds (1-60, default: 2)\n");

  printf("\n" COLOR_BOLD COLOR_GREEN "EXAMPLES:\n" COLOR_RESET);
  printf("  %s                   # Run with default settings (2s refresh)\n", prog_name);
  printf("  %s 1                 # Update every 1 second\n", prog_name);
  printf("  %s -s 3              # Show stats, update every 3 seconds\n", prog_name);
  printf("  %s -f -s 5           # Fahrenheit + stats, 5 second refresh\n", prog_name);
  printf("  %s --list            # List all sensors\n\n", prog_name);

  printf(COLOR_BOLD COLOR_CYAN "KEYBOARD CONTROLS (during monitoring):\n" COLOR_RESET);
  printf("  " COLOR_RED "Ctrl+C" COLOR_RESET "              Exit the program\n");
  printf("  " COLOR_GREEN "F" COLOR_RESET " / " COLOR_GREEN "C" COLOR_RESET
         "               Toggle between Fahrenheit/Celsius\n");
  printf("  " COLOR_YELLOW "S" COLOR_RESET "                   Toggle statistics display\n");

  printf("\n" COLOR_BOLD COLOR_MAGENTA "FEATURES:\n" COLOR_RESET);
  printf("  ✓ Real-time temperature monitoring\n");
  printf("  ✓ Automatic sensor detection (CPU, GPU, NVMe, Chipset)\n");
  printf("  ✓ Color-coded temperature bars with visual indicators\n");
  printf("  ✓ Min/Max/Average temperature tracking\n");
  printf("  ✓ Warning and critical temperature alerts\n");
  printf("  ✓ Support for multiple CPUs and GPUs\n");
  printf("  ✓ Beautiful Unicode UI with boxes and icons\n");

  printf("\n" COLOR_BOLD COLOR_YELLOW "TROUBLESHOOTING:\n" COLOR_RESET);
  printf("  If no sensors are detected, try:\n");
  printf("  1. Load kernel modules:\n");
  printf("     " COLOR_BRIGHT_BLACK "sudo modprobe coretemp k10temp" COLOR_RESET
         " (for CPU sensors)\n");
  printf("  2. Install and configure lm-sensors:\n");
  printf("     " COLOR_BRIGHT_BLACK "sudo apt install lm-sensors\n");
  printf("     sudo sensors-detect" COLOR_RESET "\n");
  printf("  3. Check for available sensors:\n");
  printf("     " COLOR_BRIGHT_BLACK "ls -la /sys/class/hwmon/" COLOR_RESET "\n\n");

  printf(COLOR_BRIGHT_BLACK
         "For more information, visit: https://github.com/yourusername/temp-monitor\n" COLOR_RESET);
  printf("\n");
}

// Print version
void print_version(void)
{
  printf("\n");
  printf(COLOR_BOLD COLOR_BRIGHT_WHITE "Temp Monitor\n" COLOR_RESET);
  printf(COLOR_BRIGHT_BLACK "Version: " COLOR_RESET "%s\n", VERSION);
  printf(COLOR_BRIGHT_BLACK "Author:  " COLOR_RESET "Danko\n");
  printf(COLOR_BRIGHT_BLACK "License: " COLOR_RESET "MIT\n");
  printf("\n");
}

// Main monitoring loop
void run_monitoring(void)
{
  hide_cursor();

  while (keep_running)
  {
    clear_screen();
    print_header(VERSION);

    // Display all sensors grouped by type
    display_all_sensors(sensors, sensor_count, &config);

    // Display statistics if enabled
    if (config.show_stats)
    {
      SystemStats stats;
      calculate_system_stats(sensors, sensor_count, &stats);
      display_statistics(&stats, &config);
    }

    print_footer(&config);

    sleep(config.refresh_rate);
  }

  show_cursor();
}

// Initialize and scan sensors
int initialize_sensors(void)
{
  printf(COLOR_BRIGHT_YELLOW "⚡ Initializing temperature monitoring system...\n" COLOR_RESET);
  printf(COLOR_CYAN "⟳ Scanning for hardware sensors...\n" COLOR_RESET);

  sensor_count = scan_temperature_sensors(sensors);

  if (sensor_count == 0)
  {
    printf("\n");
    printf(COLOR_RED COLOR_BOLD "✗ ERROR: No temperature sensors detected!\n" COLOR_RESET);
    printf("\n");
    printf(COLOR_YELLOW "📋 TROUBLESHOOTING STEPS:\n" COLOR_RESET);
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

  printf(COLOR_GREEN "✓ Successfully detected %d temperature sensor%s!\n" COLOR_RESET, sensor_count,
         sensor_count != 1 ? "s" : "");

  // Count sensors by type
  int cpu_sensors = 0, gpu_sensors = 0, nvme_sensors = 0, other_sensors = 0;
  for (int i = 0; i < sensor_count; i++)
  {
    switch (sensors[i].type)
    {
      case SENSOR_CPU:
        cpu_sensors++;
        break;
      case SENSOR_GPU:
        gpu_sensors++;
        break;
      case SENSOR_NVME:
        nvme_sensors++;
        break;
      default:
        other_sensors++;
        break;
    }
  }

  printf(COLOR_CYAN "  └─ CPU: %d  │  GPU: %d  │  NVMe: %d  │  Other: %d\n" COLOR_RESET,
         cpu_sensors, gpu_sensors, nvme_sensors, other_sensors);

  return 1;
}

// Parse command line arguments
void parse_arguments(int argc, char *argv[])
{
  for (int i = 1; i < argc; i++)
  {
    if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
    {
      print_help(argv[0]);
      exit(0);
    }
    else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0)
    {
      print_version();
      exit(0);
    }
    else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--fahrenheit") == 0)
    {
      config.use_celsius = 0;
    }
    else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--stats") == 0)
    {
      config.show_stats = 1;
    }
    else if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--list") == 0)
    {
      if (!initialize_sensors())
        exit(1);
      display_sensor_list(sensors, sensor_count);
      exit(0);
    }
    else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--compact") == 0)
    {
      config.compact_mode = 1;
    }
    else if (strcmp(argv[i], "-g") == 0 || strcmp(argv[i], "--graphs") == 0)
    {
      config.show_graphs = 1;
      printf(COLOR_YELLOW "Note: Graph feature is coming soon!\n" COLOR_RESET);
    }
    else
    {
      int rate = parse_int(argv[i], -1);
      if (rate >= 1 && rate <= 60)
      {
        config.refresh_rate = rate;
      }
      else if (rate != -1)
      {
        printf(COLOR_RED "Error: Invalid refresh rate '%s'. Must be between 1-60.\n" COLOR_RESET,
               argv[i]);
        exit(1);
      }
    }
  }
}

int main(int argc, char *argv[])
{
  // Setup signal handler
  signal(SIGINT, sigint_handler);

  // Parse arguments
  parse_arguments(argc, argv);

  // Initialize sensors
  if (!initialize_sensors())
  {
    return 1;
  }

  // Wait a moment
  sleep(1);

  printf(COLOR_BRIGHT_CYAN "⟳ Starting real-time monitoring" COLOR_RESET);
  printf(COLOR_BRIGHT_BLACK " (refresh rate: %ds)...\n" COLOR_RESET, config.refresh_rate);
  sleep(1);

  // Run monitoring loop
  run_monitoring();

  // Cleanup
  clear_screen();
  show_cursor();
  printf("\n");
  printf(COLOR_BRIGHT_WHITE "Monitoring stopped.\n" COLOR_RESET);
  printf(COLOR_BRIGHT_BLACK "Temp Monitor v%s\n" COLOR_RESET, VERSION);
  printf("\n");

  return 0;
}
