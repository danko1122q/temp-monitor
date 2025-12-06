/**
 * @file utils.c
 * @brief Temp Monitor - Utility functions implementation
 *
 * This file implements common utility functions including
 * file I/O, string manipulation, path handling, and
 * system information retrieval.
 *
 * @version 0.0.2
 * @date 2024-12-05
 *
 * MIT License
 * Copyright (c) 2024 Danko
 */

#include "utils.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

/**
 * @brief Reads entire file contents into buffer
 *
 * @param path Path to file
 * @param buffer Output buffer
 * @param size Size of buffer
 * @return 1 on success, 0 on failure
 */
int read_file(const char *path, char *buffer, size_t size)
{
  FILE *file = fopen(path, "r");
  if (!file)
    return 0;

  size_t bytes_read  = fread(buffer, 1, size - 1, file);
  buffer[bytes_read] = '\0';
  fclose(file);

  return bytes_read > 0;
}

/**
 * @brief Writes data to a file
 *
 * @param path Path to file
 * @param data Data to write
 * @return 1 on success, 0 on failure
 */
int write_file(const char *path, const char *data)
{
  FILE *file = fopen(path, "w");
  if (!file)
    return 0;

  size_t len     = strlen(data);
  size_t written = fwrite(data, 1, len, file);
  fclose(file);

  return written == len;
}

/**
 * @brief Checks if a file exists
 */
int file_exists(const char *path)
{
  return access(path, F_OK) == 0;
}

/**
 * @brief Checks if a directory exists
 */
int dir_exists(const char *path)
{
  struct stat st;
  if (stat(path, &st) != 0)
    return 0;
  return S_ISDIR(st.st_mode);
}

/**
 * @brief Converts string to lowercase in-place
 */
void str_tolower(char *str)
{
  for (int i = 0; str[i]; i++)
  {
    str[i] = tolower((unsigned char) str[i]);
  }
}

/**
 * @brief Converts string to uppercase in-place
 */
void str_toupper(char *str)
{
  for (int i = 0; str[i]; i++)
  {
    str[i] = toupper((unsigned char) str[i]);
  }
}

/**
 * @brief Trims leading and trailing whitespace in-place
 */
void str_trim(char *str)
{
  if (!str)
    return;

  /* Trim leading */
  char *start = str;
  while (isspace((unsigned char) *start))
    start++;

  if (*start == 0)
  {
    *str = 0;
    return;
  }

  /* Trim trailing */
  char *end = start + strlen(start) - 1;
  while (end > start && isspace((unsigned char) *end))
    end--;

  *(end + 1) = '\0';

  /* Move trimmed string to beginning */
  if (start != str)
  {
    memmove(str, start, strlen(start) + 1);
  }
}

/**
 * @brief Checks if string starts with prefix
 */
int str_startswith(const char *str, const char *prefix)
{
  size_t str_len    = strlen(str);
  size_t prefix_len = strlen(prefix);

  if (prefix_len > str_len)
    return 0;
  return strncmp(str, prefix, prefix_len) == 0;
}

/**
 * @brief Checks if string ends with suffix
 */
int str_endswith(const char *str, const char *suffix)
{
  size_t str_len    = strlen(str);
  size_t suffix_len = strlen(suffix);

  if (suffix_len > str_len)
    return 0;
  return strcmp(str + str_len - suffix_len, suffix) == 0;
}

/**
 * @brief Checks if string contains substring
 */
int str_contains(const char *str, const char *substr)
{
  return strstr(str, substr) != NULL;
}

/**
 * @brief Joins two path components
 */
void path_join(char *dest, size_t size, const char *path1, const char *path2)
{
  size_t len1 = strlen(path1);

  if (len1 > 0 && path1[len1 - 1] == '/')
  {
    snprintf(dest, size, "%s%s", path1, path2);
  }
  else
  {
    snprintf(dest, size, "%s/%s", path1, path2);
  }
}

/**
 * @brief Parses string to integer with default value
 */
int parse_int(const char *str, int default_val)
{
  if (!str)
    return default_val;

  char *endptr;
  long  val = strtol(str, &endptr, 10);

  if (endptr == str || *endptr != '\0')
  {
    return default_val;
  }

  return (int) val;
}

/**
 * @brief Parses string to double with default value
 */
double parse_double(const char *str, double default_val)
{
  if (!str)
    return default_val;

  char * endptr;
  double val = strtod(str, &endptr);

  if (endptr == str)
  {
    return default_val;
  }

  return val;
}

/**
 * @brief Formats a number with specified decimal places
 */
void format_number(double value, char *buffer, size_t size, int decimals)
{
  char format[16];
  snprintf(format, sizeof(format), "%%.%df", decimals);
  snprintf(buffer, size, format, value);
}

/**
 * @brief Formats bytes to human-readable size (KB, MB, GB, etc.)
 */
void format_bytes(long bytes, char *buffer, size_t size)
{
  const char *units[] = {"B", "KB", "MB", "GB", "TB"};
  int         unit    = 0;
  double      value   = (double) bytes;

  while (value >= 1024.0 && unit < 4)
  {
    value /= 1024.0;
    unit++;
  }

  snprintf(buffer, size, "%.2f %s", value, units[unit]);
}

/**
 * @brief Checks if running as root
 */
int is_root(void)
{
  return geteuid() == 0;
}

/**
 * @brief Gets system uptime in seconds
 */
long get_system_uptime(void)
{
  char buffer[128];
  if (!read_file("/proc/uptime", buffer, sizeof(buffer)))
  {
    return 0;
  }

  double uptime;
  if (sscanf(buffer, "%lf", &uptime) != 1)
  {
    return 0;
  }

  return (long) uptime;
}

/**
 * @brief Gets number of online CPUs
 */
int get_cpu_count(void)
{
  long count = sysconf(_SC_NPROCESSORS_ONLN);
  return (count > 0) ? (int) count : 1;
}

/**
 * @brief Gets total system memory in bytes
 */
long get_total_memory(void)
{
  long pages     = sysconf(_SC_PHYS_PAGES);
  long page_size = sysconf(_SC_PAGE_SIZE);

  if (pages <= 0 || page_size <= 0)
    return 0;

  return pages * page_size;
}
