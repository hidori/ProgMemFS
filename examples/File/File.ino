// File - Detailed file operations example for ProgMemFS library

#include <Arduino.h>

#include "ProgMemFS.h"

// Define file data in program memory
const char file_data[] PROGMEM = "Arduino ProgMemFS\nFile System Example\nLine 3";

// Define file path in program memory
const char file_path[] PROGMEM = "/readme.txt";

// Create file system entries
const ProgMemFSEntry files[] PROGMEM = {{file_path, file_data, sizeof(file_data) - 1}};

// Create ProgMemFS instance
ProgMemFS fs(files, sizeof(files) / sizeof(ProgMemFSEntry));

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 1000);

  Serial.println("ProgMemFS FileRead Example");
  Serial.println("==========================");

  // Open the file
  ProgMemFile* file = fs.open("/readme.txt");
  if (!file) {
    Serial.println("Failed to open file");
    return;
  }

  // Display file size
  Serial.print("File size: ");
  Serial.print(file->size());
  Serial.println(" bytes");

  // Read first 10 bytes using readBytes()
  Serial.println("\n--- Reading first 10 bytes with readBytes() ---");
  char buffer[11];
  size_t bytesRead = file->readBytes(buffer, 10);
  buffer[bytesRead] = '\0';
  Serial.print("Read ");
  Serial.print(bytesRead);
  Serial.print(" bytes: \"");
  Serial.print(buffer);
  Serial.println("\"");

  // Show current position after reading
  Serial.print("Current position: ");
  Serial.println(file->position());

  // Show available bytes
  Serial.print("Available bytes: ");
  Serial.println(file->available());

  // Seek to position 8
  Serial.println("\n--- Seeking to position 8 ---");
  file->seek(8);
  Serial.print("Position after seek(8): ");
  Serial.println(file->position());

  // Read remaining content
  Serial.print("Content from position 8: \"");
  while (file->available()) {
    Serial.write(file->read());
  }
  Serial.println("\"");

  // Seek back to beginning
  Serial.println("\n--- Seeking back to beginning ---");
  file->seek(0);
  Serial.print("Position after seek(0): ");
  Serial.println(file->position());

  // Read full content byte by byte
  Serial.print("Full content: \"");
  while (file->available()) {
    Serial.write(file->read());
  }
  Serial.println("\"");

  // Clean up
  delete file;
}

void loop() {}
