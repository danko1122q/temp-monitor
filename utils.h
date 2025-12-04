#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

// File operations
int read_file(const char *path, char *buffer, size_t size);
int write_file(const char *path, const char *data);
int file_exists(const char *path);
int dir_exists(const char *path);

// String utilities
void str_tolower(char *str);
void str_toupper(char *str);
void str_trim(char *str);
int str_startswith(const char *str, const char *prefix);
int str_endswith(const char *str, const char *suffix);
int str_contains(const char *str, const char *substr);
void str_replace(char *str, const char *find, const char *replace);

// Path utilities
void path_join(char *dest, size_t size, const char *path1, const char *path2);
void path_basename(const char *path, char *basename, size_t size);
void path_dirname(const char *path, char *dirname, size_t size);

// Number utilities
int parse_int(const char *str, int default_val);
double parse_double(const char *str, double default_val);
void format_number(double value, char *buffer, size_t size, int decimals);
void format_bytes(long bytes, char *buffer, size_t size);

// System utilities
int is_root(void);
long get_system_uptime(void);
int get_cpu_count(void);
long get_total_memory(void);

// Color definitions
#define COLOR_RESET     "\033[0m"
#define COLOR_BOLD      "\033[1m"
#define COLOR_DIM       "\033[2m"
#define COLOR_ITALIC    "\033[3m"
#define COLOR_UNDERLINE "\033[4m"

#define COLOR_BLACK     "\033[30m"
#define COLOR_RED       "\033[31m"
#define COLOR_GREEN     "\033[32m"
#define COLOR_YELLOW    "\033[33m"
#define COLOR_BLUE      "\033[34m"
#define COLOR_MAGENTA   "\033[35m"
#define COLOR_CYAN      "\033[36m"
#define COLOR_WHITE     "\033[37m"

#define COLOR_BRIGHT_BLACK   "\033[90m"
#define COLOR_BRIGHT_RED     "\033[91m"
#define COLOR_BRIGHT_GREEN   "\033[92m"
#define COLOR_BRIGHT_YELLOW  "\033[93m"
#define COLOR_BRIGHT_BLUE    "\033[94m"
#define COLOR_BRIGHT_MAGENTA "\033[95m"
#define COLOR_BRIGHT_CYAN    "\033[96m"
#define COLOR_BRIGHT_WHITE   "\033[97m"

#define BG_BLACK     "\033[40m"
#define BG_RED       "\033[41m"
#define BG_GREEN     "\033[42m"
#define BG_YELLOW    "\033[43m"
#define BG_BLUE      "\033[44m"
#define BG_MAGENTA   "\033[45m"
#define BG_CYAN      "\033[46m"
#define BG_WHITE     "\033[47m"

// Unicode symbols
#define SYMBOL_DEGREES  "°"
#define SYMBOL_UP       "↑"
#define SYMBOL_DOWN     "↓"
#define SYMBOL_ARROW    "→"
#define SYMBOL_CHECK    "✓"
#define SYMBOL_CROSS    "✗"
#define SYMBOL_WARNING  "⚠"
#define SYMBOL_FIRE     "🔥"
#define SYMBOL_COOL     "❄"
#define SYMBOL_CPU      "⚙"
#define SYMBOL_GPU      "🎮"
#define SYMBOL_DISK     "💾"
#define SYMBOL_CHART    "📊"

#endif // UTILS_H
