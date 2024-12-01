# watch-cleaning-machine

## Frequently used commands
 - `pio run --target clean && pio run --target uploadfs`
 - `pio run --target upload && pio device monitor`
 - `find src lib include -type f \( -name "*.cpp" -o -name "*.h" \) -exec clang-format -i {} +`