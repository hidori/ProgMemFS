# Arduino: ProgMemFS - Program Memory File System

A lightweight file system library that stores static data in PROGMEM (flash memory) to save RAM. Perfect for storing HTML, JSON, configuration files, or any static content on Arduino.

## Features

- Store files in flash memory (PROGMEM) instead of RAM
- Stream-compatible interface for reading data
- Virtual file system with path-based file access
- Support for sorted file lists with binary search
- Simple API with minimal setup

## Installation

### Arduino IDE Library Manager

1. Open Arduino IDE
2. Go to Sketch → Include Library → Manage Libraries
3. Search for "ProgMemFS"
4. Click Install

### Manual Installation

1. Download this repository as ZIP
2. In Arduino IDE: Sketch → Include Library → Add .ZIP Library
3. Select the downloaded ZIP file

## Quick Start

```cpp
#include <ProgMemFS.h>

// Define file data in program memory
const char file_data[] PROGMEM = "Hello, World!";
const char file_path[] PROGMEM = "/hello.txt";

// Create file system entries
const ProgMemFSEntry files[] PROGMEM = {
  {file_path, file_data, sizeof(file_data) - 1}
};

// Create ProgMemFS instance
ProgMemFS fs(files, sizeof(files) / sizeof(ProgMemFSEntry));

void setup() {
  Serial.begin(115200);

  // Open and read file
  ProgMemFile* file = fs.open("/hello.txt");
  if (file) {
    while (file->available()) {
      Serial.write(file->read());
    }
    delete file;
  }
}

void loop() {
}
```

## Examples

See the `examples/` directory for complete usage examples:

- **Basic**: Minimal example (file existence check and reading)
- **File**: Detailed file operations (readBytes, seek, position, size)
- **Directory**: Directory enumeration (openDirectory, openNextFile, openNextDirectory, rewind)

## API Reference

### ProgMemFSEntry Structure

```cpp
struct ProgMemFSEntry {
  const char* path;    // File path (e.g., "/index.html")
  const char* data;    // Pointer to PROGMEM data
  size_t length;       // Data length in bytes
};
```

### ProgMemFS Class

#### Constructor

##### `ProgMemFS(const ProgMemFSEntry* entries, size_t numEntries, bool sorted = false)`

Create a new file system instance.

**Parameters:**
- `entries`: Array of `ProgMemFSEntry` structures
- `numEntries`: Number of entries in the array
- `sorted`: Set to `true` if entries are sorted by path (enables binary search)

**Example:**
```cpp
// Unsorted file list (linear search)
ProgMemFS fs(files, 3);

// Sorted file list (binary search for better performance)
ProgMemFS fs(sorted_files, 3, true);
```

#### Methods

##### `ProgMemFile* open(const char* path)`

Open a file and return a file object for reading.

**Parameters:**
- `path`: File path to open

**Returns:** Pointer to `ProgMemFile` if found, `nullptr` otherwise

**Note:** You must `delete` the returned file when done to avoid memory leaks.

**Example:**
```cpp
ProgMemFile* file = fs.open("/hello.txt");
if (file) {
  // Read from file...
  delete file;
}
```

##### `bool isFile(const char* path)`

Check if a path is a file in the file system.

**Returns:** `true` if path is a file, `false` otherwise

**Example:**
```cpp
if (fs.isFile("/config.json")) {
  Serial.println("Config file found");
}
```

### ProgMemFile Class

A Stream-compatible class for reading PROGMEM data.

#### Methods

##### `int available()`

Get number of bytes available for reading.

##### `int read()`

Read a single byte from the stream.

##### `int peek()`

Peek at the next byte without advancing position.

##### `size_t readBytes(char* buffer, size_t length)`

Read multiple bytes into a buffer.

##### `bool seek(size_t position)`

Seek to a specific position in the stream.

##### `size_t position()`

Get the current read position.

##### `size_t size()`

Get the total size of the stream.

##### `const char* name()`

Get the file path.

## Supported Platforms

This library is compatible with Arduino platforms that support PROGMEM:

- Arduino UNO, Nano, Pro Mini
- Arduino Leonardo, Pro Micro
- Arduino UNO R4 (Minima, WiFi)
- ATtiny85
- Raspberry Pi Pico, RP2040
- ESP32-C3
- LGT8F328P
- CH32V003
- And other Arduino-compatible platforms

**Note:** ATtiny13 is not supported (no Stream.h).

## License

MIT License - see LICENSE file for details.

## Contributing

Pull requests and issues are welcome on GitHub.
