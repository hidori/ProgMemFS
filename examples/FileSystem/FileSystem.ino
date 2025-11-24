// FileSystem - Example sketch for ProgMemFS library

#include <Arduino.h>
#include "ProgMemFileSystem.h"

// Define files in program memory
const char file1_data[] PROGMEM = "Hello, World!";
const char file2_data[] PROGMEM = "Arduino ProgMemFS\nFile System Example";

// Define file paths in program memory
const char path1[] PROGMEM = "/hello.txt";
const char path2[] PROGMEM = "/readme.txt";

// Create file system entries
const FileSystemEntry files[] PROGMEM = {
  {path1, file1_data, sizeof(file1_data) - 1},
  {path2, file2_data, sizeof(file2_data) - 1}
};

// Create ProgMemFileSystem instance
ProgMemFileSystem fs(files, sizeof(files) / sizeof(FileSystemEntry));

void setup() {
  // Initialize Serial
  Serial.begin(115200);
  while (!Serial < 1000);

  // Demonstrate file system usage
  Serial.println("ProgMemFS FileSystem Example");
  Serial.print("Files: ");
  Serial.println(fs.count());

  // Check existence of files
  Serial.print("/hello.txt exists: ");
  Serial.println(fs.exists("/hello.txt") ? "Yes" : "No");
  Serial.print("/readme.txt exists: ");
  Serial.println(fs.exists("/readme.txt") ? "Yes" : "No");
  Serial.print("/notfound.txt exists: ");
  Serial.println(fs.exists("/notfound.txt") ? "Yes" : "No");

  // Open and read /hello.txt
  if (fs.exists("/hello.txt")) {
    Serial.println("Opening /hello.txt...");
    ProgMemStream* stream = fs.open("/hello.txt");
    if (stream) {
      // Read and print file content
      while (stream->available()) {
        Serial.write(stream->read());
      }
      Serial.println();
      delete stream;
    }
  }
}

void loop() {
}
