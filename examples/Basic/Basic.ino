// Basic - Minimal example sketch for ProgMemFS library

#include <Arduino.h>

#include "ProgMemFS.h"

// Define file data in program memory
const char file_data[] PROGMEM = "Hello, World!";

// Define file path in program memory
const char file_path[] PROGMEM = "/hello.txt";

// Create file system entries
const ProgMemFSEntry files[] PROGMEM = {{file_path, file_data, sizeof(file_data) - 1}};

// Create ProgMemFS instance
ProgMemFS fs(files, sizeof(files) / sizeof(ProgMemFSEntry));

void setup() {
  Serial.begin(115200);

  // Open and read file
  ProgMemFile* file = fs.open("/hello.txt");
  if (file) {
    // Read and print file content byte by byte
    while (file->available()) {
      Serial.write(file->read());
    }
    delete file;
  }
}

void loop() {}
