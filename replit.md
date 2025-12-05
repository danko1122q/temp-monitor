# Temperature Monitor

Terminal-based hardware temperature monitoring for Linux.

## Build

```bash
make
```

## Run

```bash
./bin/temp
```

## Options

- `-s` stats
- `-f` fahrenheit
- `-F` show fans
- `-l` list sensors
- `1-60` refresh rate

## Files

- `main.c` - entry point
- `sensor.c/h` - sensor detection
- `display.c/h` - terminal output
- `utils.c/h` - utilities
- `Makefile` - build

## Notes

Reads sensors from `/sys/class/hwmon/`.
Works on most Linux distributions.
