# 📁 Project Structure

Complete directory structure and file organization for Ultimate Hardware Temperature Monitor v0.0.1

---

## 🌳 Directory Tree

```
temp-monitor/
├── 📄 main.c                      # Main program entry point
├── 📄 sensor.c                    # Sensor detection and management
├── 📄 sensor.h                    # Sensor interface header
├── 📄 display.c                   # Display and UI functions
├── 📄 display.h                   # Display interface header
├── 📄 utils.c                     # Utility functions
├── 📄 utils.h                     # Utility interface header
├── 📄 Makefile                    # Build system configuration
├── 📄 build.sh                    # Automated build script
├── 📄 .gitignore                  # Git ignore rules
├── 📄 LICENSE                     # MIT License
├── 📄 README.md                   # Main documentation
├── 📄 INSTALL.md                  # Installation guide
├── 📄 CHANGELOG.md                # Version history
├── 📄 PROJECT_STRUCTURE.md        # This file
├── 📁 build/                      # Build artifacts (created by make)
│   ├── main.o
│   ├── sensor.o
│   ├── display.o
│   └── utils.o
└── 📁 bin/                        # Compiled executables (created by make)
    ├── tempmonitor                # Release binary
    └── tempmonitor-debug          # Debug binary
```

---

## 📋 File Descriptions

### Source Files

#### `main.c` (Main Program)
**Size:** ~500 lines  
**Purpose:** Program initialization, argument parsing, main monitoring loop

**Key Functions:**
- `main()` - Entry point
- `parse_arguments()` - Command-line argument handling
- `run_monitoring()` - Main monitoring loop
- `initialize_sensors()` - Sensor initialization
- `print_help()` - Help message
- `print_version()` - Version information
- `sigint_handler()` - Signal handler for Ctrl+C

**Dependencies:**
- sensor.h
- display.h
- utils.h
- Standard C libraries

---

#### `sensor.c` / `sensor.h` (Sensor Management)
**Size:** ~700 lines  
**Purpose:** Hardware sensor detection, temperature reading, statistics

**Key Structures:**
```c
typedef struct {
    char name[128];           // Sensor device name
    char label[128];          // Sensor label/description
    char path[512];           // File path to sensor
    SensorType type;          // Type of sensor (CPU, GPU, etc.)
    double temp_current;      // Current temperature
    double temp_max;          // Maximum recorded
    double temp_min;          // Minimum recorded
    double temp_critical;     // Critical threshold
    // ... more fields
} TempSensor;

typedef struct {
    double avg_cpu_temp;      // Average CPU temperature
    double max_cpu_temp;      // Peak CPU temperature
    int cpu_count;            // Number of CPU sensors
    // ... more statistics
} SystemStats;
```

**Key Functions:**
- `scan_temperature_sensors()` - Scan for all sensors
- `scan_hwmon_sensors()` - Scan hwmon subsystem
- `read_temperature()` - Read temperature from file
- `update_sensor_data()` - Update sensor readings
- `calculate_system_stats()` - Calculate statistics
- `detect_sensor_type()` - Identify sensor type
- `get_sensor_status()` - Check if temperature is OK/Warning/Critical

**Sensor Types Supported:**
- SENSOR_CPU (Intel, AMD)
- SENSOR_GPU (AMD, NVIDIA, Intel)
- SENSOR_NVME (NVMe drives)
- SENSOR_CHIPSET (Motherboard)
- SENSOR_MEMORY (RAM)
- SENSOR_VRM (Voltage regulators)
- SENSOR_DISK (SATA drives)
- SENSOR_OTHER (Unknown sensors)

---

#### `display.c` / `display.h` (Display & UI)
**Size:** ~600 lines  
**Purpose:** Terminal UI, visualization, color output

**Key Structures:**
```c
typedef struct {
    int use_celsius;          // Temperature unit
    int show_stats;           // Show statistics
    int show_graphs;          // Show graphs
    int compact_mode;         // Compact display
    int refresh_rate;         // Update interval
} DisplayConfig;
```

**Key Functions:**
- `print_header()` - Display header with title and time
- `print_footer()` - Display footer with controls
- `display_all_sensors()` - Display all sensor groups
- `display_sensor_group()` - Display sensors by type
- `display_statistics()` - Display system statistics
- `display_sensor_list()` - List all sensors
- `print_temp_bar()` - Temperature bar visualization
- `print_temperature()` - Format temperature output
- `clear_screen()` - Clear terminal
- `get_terminal_size()` - Get terminal dimensions

**Color Scheme:**
- Cyan: Cold (<40°C)
- Green: Safe (40-50°C)
- Bright Green: Normal (50-60°C)
- Yellow: Warm (60-70°C)
- Bright Yellow: High (70-80°C)
- Bright Red: Very High (80-90°C)
- Bold Red: Critical (>90°C)

---

#### `utils.c` / `utils.h` (Utilities)
**Size:** ~400 lines  
**Purpose:** Common utility functions, file I/O, string operations

**Key Functions:**

**File Operations:**
- `read_file()` - Read file content
- `write_file()` - Write to file
- `file_exists()` - Check file existence
- `dir_exists()` - Check directory existence

**String Operations:**
- `str_tolower()` - Convert to lowercase
- `str_toupper()` - Convert to uppercase
- `str_trim()` - Remove whitespace
- `str_startswith()` - Check prefix
- `str_endswith()` - Check suffix
- `str_contains()` - Check substring

**Path Operations:**
- `path_join()` - Join path components
- `path_basename()` - Get filename
- `path_dirname()` - Get directory

**System Utilities:**
- `is_root()` - Check root privileges
- `get_system_uptime()` - Get system uptime
- `get_cpu_count()` - Get CPU count
- `get_total_memory()` - Get total RAM

**Number Utilities:**
- `parse_int()` - Parse integer
- `parse_double()` - Parse float
- `format_number()` - Format number
- `format_bytes()` - Format byte size

---

### Build Files

#### `Makefile`
**Purpose:** GNU Make build configuration

**Targets:**
- `all` - Default build (release)
- `debug` - Debug build with symbols
- `clean` - Remove build artifacts
- `install` - Install to /usr/local/bin
- `uninstall` - Remove from system
- `run` - Build and run
- `run-stats` - Run with statistics
- `check-modules` - Check kernel modules
- `setup-sensors` - Setup lm-sensors
- `load-modules` - Load sensor modules
- `list-sensors` - List detected sensors
- `help` - Show help
- `version` - Show version

**Configuration:**
```makefile
CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11
LDFLAGS = -lm
```

---

#### `build.sh`
**Purpose:** Automated build and setup script

**Features:**
- Color output
- Dependency checking
- Multiple build modes
- Sensor setup automation
- Module loading
- System installation

**Commands:**
```bash
./build.sh --build          # Build project
./build.sh --clean          # Clean build
./build.sh --install        # Install system-wide
./build.sh --run            # Build and run
./build.sh --setup          # Setup sensors
./build.sh --check          # Check system
```

---

### Documentation Files

#### `README.md`
Main project documentation with:
- Feature overview
- Screenshots
- Quick start guide
- Usage examples
- Troubleshooting
- Contributing guidelines

#### `INSTALL.md`
Complete installation guide with:
- System requirements
- Installation methods
- Sensor setup
- Platform-specific instructions
- Troubleshooting
- Post-installation steps

#### `CHANGELOG.md`
Version history with:
- Release notes
- Feature additions
- Bug fixes
- Known issues
- Planned features

#### `PROJECT_STRUCTURE.md`
This file - complete project structure documentation

---

## 🔧 Build Process

### Compilation Flow

```
Source Files          Object Files         Executable
───────────          ────────────         ──────────
main.c     ──────►   main.o      ┐
sensor.c   ──────►   sensor.o    ├────►  tempmonitor
display.c  ──────►   display.o   │
utils.c    ──────►   utils.o     ┘
```

### Build Commands

```bash
# 1. Create directories
mkdir -p build bin

# 2. Compile object files
gcc -Wall -Wextra -O2 -c main.c -o build/main.o
gcc -Wall -Wextra -O2 -c sensor.c -o build/sensor.o
gcc -Wall -Wextra -O2 -c display.c -o build/display.o
gcc -Wall -Wextra -O2 -c utils.c -o build/utils.o

# 3. Link executable
gcc build/*.o -lm -o bin/tempmonitor
```

---

## 📊 Code Statistics

### Lines of Code

| File | Lines | Description |
|------|-------|-------------|
| main.c | ~500 | Main program logic |
| sensor.c | ~700 | Sensor management |
| display.c | ~600 | UI and visualization |
| utils.c | ~400 | Utility functions |
| **Total** | **~2200** | **Core code** |

### Header Files

| File | Lines | Description |
|------|-------|-------------|
| sensor.h | ~80 | Sensor interfaces |
| display.h | ~70 | Display interfaces |
| utils.h | ~90 | Utility interfaces |
| **Total** | **~240** | **Headers** |

### Documentation

| File | Lines | Description |
|------|-------|-------------|
| README.md | ~500 | Main docs |
| INSTALL.md | ~600 | Installation guide |
| CHANGELOG.md | ~250 | Version history |
| Makefile | ~150 | Build system |
| build.sh | ~300 | Build script |
| **Total** | **~1800** | **Documentation** |

### Grand Total: ~4200 lines

---

## 🎯 Module Dependencies

```
main.c
  ├── sensor.h
  │     └── utils.h
  ├── display.h
  │     ├── sensor.h
  │     └── utils.h
  └── utils.h

sensor.c
  ├── sensor.h
  └── utils.h

display.c
  ├── display.h
  ├── sensor.h
  └── utils.h

utils.c
  └── utils.h
```

---

## 🔍 Key Directories

### `/sys/class/hwmon/`
Linux hardware monitoring interface
```
/sys/class/hwmon/
├── hwmon0/
│   ├── name
│   ├── temp1_input
│   ├── temp1_label
│   └── temp1_crit
├── hwmon1/
└── hwmon2/
```

### `/sys/class/thermal/`
Thermal zone interface (future support)

### `/sys/class/drm/`
DRM interface for GPU sensors (future support)

---

## 🚀 Future Structure

Planned additions for v0.1.0+:

```
temp-monitor/
├── src/                    # Source directory
│   ├── core/              # Core modules
│   ├── sensors/           # Sensor implementations
│   ├── ui/                # UI components
│   └── plugins/           # Plugin system
├── include/               # Public headers
├── tests/                 # Unit tests
├── docs/                  # Extended documentation
├── examples/              # Example configurations
└── scripts/               # Utility scripts
```

---

## 📝 Coding Standards

### Style Guidelines
- **Indentation:** 4 spaces
- **Line length:** 100 characters max
- **Comments:** Descriptive, not excessive
- **Naming:** snake_case for functions, CAPS for macros

### Code Organization
- One module per responsibility
- Clear separation of interface and implementation
- Minimal dependencies between modules
- Consistent error handling

---

<div align="center">

**Project Structure Documentation v0.0.1**

Complete and ready for development! 🚀

</div>
