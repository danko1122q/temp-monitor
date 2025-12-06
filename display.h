/**
 * @file display.h
 * @brief Temp Monitor - Display and UI functions
 *
 * Header file containing display function prototypes and
 * data structures for terminal UI rendering.
 *
 * @version 0.0.2
 * @date 2024-12-05
 *
 * MIT License
 * Copyright (c) 2024 Danko
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include "sensor.h"

#include <stddef.h>

typedef struct
{
  int use_celsius;
  int show_stats;
  int show_graphs;
  int show_fans;
  int compact_mode;
  int color_mode;
  int refresh_rate;
} DisplayConfig;

typedef struct
{
  double temps[100];
  int    index;
  int    size;
} TempHistory;

void clear_screen(void);
void get_terminal_size(int *rows, int *cols);
void move_cursor(int row, int col);
void hide_cursor(void);
void show_cursor(void);
void enter_alternate_screen(void);
void exit_alternate_screen(void);
void set_color(const char *color);
void reset_color(void);

void   print_temperature(double temp, int use_celsius);
double celsius_to_fahrenheit(double celsius);

void print_header(const char *version);
void print_footer(DisplayConfig *config);
void print_separator(int width, int style);
void print_box_top(int width);
void print_box_bottom(int width);
void print_box_line(const char *text, int width);

void print_temp_bar(double temp, int width, int use_gradient);
void print_sparkline(TempHistory *history, int width);
void print_gauge(double value, double max, int width);
void print_fan_speed(int rpm, int percent);

void display_sensor_group(TempSensor *sensors, int count, SensorType type, DisplayConfig *config);
void display_all_sensors(TempSensor *sensors, int count, DisplayConfig *config);
void display_fan_sensors(TempSensor *sensors, int count, DisplayConfig *config);
void display_statistics(SystemStats *stats, DisplayConfig *config);
void display_system_info(void);
void display_sensor_list(TempSensor *sensors, int count);

void init_temp_history(TempHistory *history, int size);
void add_temp_history(TempHistory *history, double temp);
void display_temp_graph(TempHistory *history, const char *label, int width, int height);

void        get_current_time(char *buffer, size_t size);
void        format_uptime(char *buffer, size_t size);
const char *get_bar_char(double temp);

#endif
