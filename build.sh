#!/bin/bash

set -e

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m'

PROJECT_NAME="Ultimate Hardware Temperature Monitor"
VERSION="0.0.1"
BUILD_DIR="build"
BIN_DIR="bin"
TARGET="tempmonitor"

print_msg() {
    local color="$1"
    shift
    echo -e "${color}$*${NC}"
}

print_header() {
    echo ""
    print_msg "$CYAN" "================================================================"
    print_msg "$CYAN" "  $PROJECT_NAME v$VERSION"
    print_msg "$CYAN" "================================================================"
    echo ""
}

check_dependencies() {
    print_msg "$YELLOW" "[*] Checking dependencies..."
    
    local missing_deps=0
    
    if ! command -v gcc &> /dev/null; then
        print_msg "$RED" "[X] GCC compiler not found"
        missing_deps=1
    else
        print_msg "$GREEN" "[+] GCC compiler found"
    fi
    
    if ! command -v make &> /dev/null; then
        print_msg "$RED" "[X] Make not found"
        missing_deps=1
    else
        print_msg "$GREEN" "[+] Make found"
    fi
    
    if [ $missing_deps -eq 1 ]; then
        echo ""
        print_msg "$YELLOW" "Installing missing dependencies..."
        
        if command -v apt-get &> /dev/null; then
            print_msg "$CYAN" "Detected: Debian/Ubuntu"
            print_msg "$CYAN" "Run: sudo apt-get install build-essential"
        elif command -v dnf &> /dev/null; then
            print_msg "$CYAN" "Detected: Fedora/RHEL"
            print_msg "$CYAN" "Run: sudo dnf install gcc make"
        elif command -v yum &> /dev/null; then
            print_msg "$CYAN" "Detected: CentOS/RHEL"
            print_msg "$CYAN" "Run: sudo yum install gcc make"
        elif command -v pacman &> /dev/null; then
            print_msg "$CYAN" "Detected: Arch Linux"
            print_msg "$CYAN" "Run: sudo pacman -S base-devel"
        elif command -v zypper &> /dev/null; then
            print_msg "$CYAN" "Detected: openSUSE"
            print_msg "$CYAN" "Run: sudo zypper install gcc make"
        elif command -v apk &> /dev/null; then
            print_msg "$CYAN" "Detected: Alpine Linux"
            print_msg "$CYAN" "Run: sudo apk add build-base"
        elif command -v emerge &> /dev/null; then
            print_msg "$CYAN" "Detected: Gentoo"
            print_msg "$CYAN" "Run: sudo emerge sys-devel/gcc sys-devel/make"
        elif command -v nix-env &> /dev/null; then
            print_msg "$CYAN" "Detected: NixOS"
            print_msg "$CYAN" "Run: nix-env -iA nixpkgs.gcc nixpkgs.gnumake"
        else
            print_msg "$RED" "Unknown distribution. Please install gcc and make manually."
        fi
        exit 1
    fi
    
    echo ""
}

clean_build() {
    print_msg "$YELLOW" "[*] Cleaning build files..."
    make clean &>/dev/null || rm -rf "$BUILD_DIR" "$BIN_DIR" 2>/dev/null || true
    print_msg "$GREEN" "[+] Clean completed"
    echo ""
}

build_project() {
    print_msg "$YELLOW" "[*] Building $PROJECT_NAME..."
    echo ""
    
    if make; then
        echo ""
        print_msg "$GREEN" "[+] Build successful!"
        echo ""
        print_msg "$CYAN" "Executable: ./$BIN_DIR/$TARGET"
        print_msg "$YELLOW" "Run with: ./$BIN_DIR/$TARGET"
    else
        echo ""
        print_msg "$RED" "[X] Build failed!"
        exit 1
    fi
}

build_debug() {
    print_msg "$YELLOW" "[*] Building debug version..."
    echo ""
    
    if make debug; then
        echo ""
        print_msg "$GREEN" "[+] Debug build successful!"
        print_msg "$CYAN" "Debug executable: ./$BIN_DIR/$TARGET-debug"
        print_msg "$YELLOW" "Run with: gdb ./$BIN_DIR/$TARGET-debug"
    else
        print_msg "$RED" "[X] Debug build failed!"
        exit 1
    fi
}

check_sensors() {
    print_msg "$YELLOW" "[*] Checking for sensor modules..."
    echo ""
    
    if command -v lsmod &> /dev/null; then
        if lsmod | grep -qE "coretemp|k10temp|zenpower"; then
            print_msg "$GREEN" "[+] Temperature sensor modules loaded:"
            lsmod | grep -E "coretemp|k10temp|zenpower" | awk '{print "  - " $1}'
        else
            print_msg "$YELLOW" "[!] No temperature sensor modules loaded"
            print_msg "$CYAN" "Tip: Run './build.sh --load-modules' to load them"
        fi
    else
        print_msg "$YELLOW" "[!] lsmod not available"
    fi
    
    echo ""
    
    if [ -d "/sys/class/hwmon" ]; then
        local hwmon_count
        hwmon_count=$(find /sys/class/hwmon -maxdepth 1 -name "hwmon*" 2>/dev/null | wc -l)
        print_msg "$GREEN" "[+] Found $hwmon_count hwmon device(s)"
        
        if [ "$hwmon_count" -eq 0 ]; then
            print_msg "$RED" "[!] No hwmon devices detected!"
            print_msg "$YELLOW" "Run './build.sh --setup' to configure sensors"
        fi
    else
        print_msg "$RED" "[X] /sys/class/hwmon not accessible"
    fi
    
    echo ""
}

setup_sensors() {
    print_msg "$YELLOW" "[*] Setting up lm-sensors..."
    echo ""
    
    if command -v apt-get &> /dev/null; then
        print_msg "$CYAN" "Installing lm-sensors (Debian/Ubuntu)..."
        sudo apt-get update && sudo apt-get install -y lm-sensors
    elif command -v dnf &> /dev/null; then
        print_msg "$CYAN" "Installing lm_sensors (Fedora/RHEL)..."
        sudo dnf install -y lm_sensors
    elif command -v yum &> /dev/null; then
        print_msg "$CYAN" "Installing lm_sensors (CentOS/RHEL)..."
        sudo yum install -y lm_sensors
    elif command -v pacman &> /dev/null; then
        print_msg "$CYAN" "Installing lm_sensors (Arch Linux)..."
        sudo pacman -S --noconfirm lm_sensors
    elif command -v zypper &> /dev/null; then
        print_msg "$CYAN" "Installing sensors (openSUSE)..."
        sudo zypper install -y sensors
    elif command -v apk &> /dev/null; then
        print_msg "$CYAN" "Installing lm-sensors (Alpine Linux)..."
        sudo apk add lm-sensors
    else
        print_msg "$RED" "Unknown distribution. Please install lm-sensors manually."
        return 1
    fi
    
    if command -v sensors-detect &> /dev/null; then
        print_msg "$CYAN" "Running sensors-detect..."
        sudo sensors-detect --auto || true
    fi
    
    if command -v systemctl &> /dev/null; then
        print_msg "$CYAN" "Restarting kmod service..."
        sudo systemctl restart kmod 2>/dev/null || true
    fi
    
    echo ""
    print_msg "$GREEN" "[+] Sensor setup completed!"
    print_msg "$YELLOW" "Try running: sensors"
    echo ""
}

load_modules() {
    print_msg "$YELLOW" "[*] Loading temperature sensor modules..."
    echo ""
    
    if sudo modprobe coretemp 2>/dev/null; then
        print_msg "$GREEN" "[+] Loaded coretemp (Intel)"
    else
        print_msg "$YELLOW" "[!] coretemp not available"
    fi
    
    if sudo modprobe k10temp 2>/dev/null; then
        print_msg "$GREEN" "[+] Loaded k10temp (AMD)"
    else
        print_msg "$YELLOW" "[!] k10temp not available"
    fi
    
    if sudo modprobe zenpower 2>/dev/null; then
        print_msg "$GREEN" "[+] Loaded zenpower (AMD Ryzen)"
    else
        print_msg "$YELLOW" "[!] zenpower not available"
    fi
    
    if sudo modprobe nct6775 2>/dev/null; then
        print_msg "$GREEN" "[+] Loaded nct6775 (Nuvoton SuperIO)"
    else
        print_msg "$YELLOW" "[!] nct6775 not available"
    fi
    
    if sudo modprobe it87 2>/dev/null; then
        print_msg "$GREEN" "[+] Loaded it87 (ITE SuperIO)"
    else
        print_msg "$YELLOW" "[!] it87 not available"
    fi
    
    echo ""
}

install_system() {
    if [ ! -f "$BIN_DIR/$TARGET" ]; then
        print_msg "$RED" "[X] Binary not found. Build first!"
        exit 1
    fi
    
    print_msg "$YELLOW" "[*] Installing to /usr/local/bin..."
    
    sudo cp "$BIN_DIR/$TARGET" /usr/local/bin/
    sudo chmod +x "/usr/local/bin/$TARGET"
    
    print_msg "$GREEN" "[+] Installation completed!"
    print_msg "$CYAN" "You can now run: $TARGET"
    echo ""
}

uninstall_system() {
    print_msg "$YELLOW" "[*] Uninstalling from /usr/local/bin..."
    
    sudo rm -f "/usr/local/bin/$TARGET"
    
    print_msg "$GREEN" "[+] Uninstallation completed!"
    echo ""
}

run_program() {
    if [ ! -f "$BIN_DIR/$TARGET" ]; then
        print_msg "$RED" "[X] Binary not found. Building first..."
        build_project
    fi
    
    print_msg "$CYAN" "[*] Running $PROJECT_NAME..."
    echo ""
    sleep 1
    
    "./$BIN_DIR/$TARGET" "$@"
}

show_help() {
    print_header
    
    echo "Build script for $PROJECT_NAME"
    echo ""
    echo "Usage: $0 [OPTION]"
    echo ""
    echo "Options:"
    echo "  --help              Show this help message"
    echo "  --build             Build the project (default)"
    echo "  --debug             Build debug version"
    echo "  --clean             Clean build files"
    echo "  --rebuild           Clean and rebuild"
    echo "  --install           Install to /usr/local/bin"
    echo "  --uninstall         Uninstall from system"
    echo "  --run [args]        Build and run"
    echo "  --check             Check dependencies and sensors"
    echo "  --setup             Setup lm-sensors"
    echo "  --load-modules      Load sensor kernel modules"
    echo ""
    echo "Supported distributions:"
    echo "  - Debian, Ubuntu, Linux Mint"
    echo "  - Fedora, RHEL, CentOS, Rocky Linux"
    echo "  - Arch Linux, Manjaro"
    echo "  - openSUSE"
    echo "  - Alpine Linux"
    echo "  - Gentoo"
    echo "  - NixOS"
    echo ""
    echo "Examples:"
    echo "  $0                  # Build the project"
    echo "  $0 --rebuild        # Clean and rebuild"
    echo "  $0 --run -s         # Build and run with stats"
    echo "  $0 --setup          # Setup sensors"
    echo ""
}

main() {
    case "${1:-}" in
        --help|-h)
            show_help
            ;;
        --clean)
            print_header
            clean_build
            ;;
        --debug)
            print_header
            check_dependencies
            build_debug
            ;;
        --rebuild)
            print_header
            check_dependencies
            clean_build
            build_project
            ;;
        --install)
            print_header
            install_system
            ;;
        --uninstall)
            print_header
            uninstall_system
            ;;
        --run)
            print_header
            shift
            run_program "$@"
            ;;
        --check)
            print_header
            check_dependencies
            check_sensors
            ;;
        --setup)
            print_header
            setup_sensors
            ;;
        --load-modules)
            print_header
            load_modules
            check_sensors
            ;;
        --build|"")
            print_header
            check_dependencies
            build_project
            check_sensors
            ;;
        *)
            print_msg "$RED" "Unknown option: $1"
            echo ""
            show_help
            exit 1
            ;;
    esac
}

main "$@"
