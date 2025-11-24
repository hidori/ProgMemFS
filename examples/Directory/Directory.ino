// Directory - Directory enumeration example for ProgMemFS library

#include <Arduino.h>

#include "ProgMemFS.h"

// Define file data in program memory
const char file1_data[] PROGMEM = "Root file content";
const char file2_data[] PROGMEM = "Document 1 content";
const char file3_data[] PROGMEM = "Document 2 content";
const char file4_data[] PROGMEM = "Image data here";

// Define file paths in program memory (files without trailing /, directories with trailing /)
const char path1[] PROGMEM = "/readme.txt";
const char path2[] PROGMEM = "/docs/";
const char path3[] PROGMEM = "/docs/doc1.txt";
const char path4[] PROGMEM = "/docs/doc2.txt";
const char path5[] PROGMEM = "/images/";
const char path6[] PROGMEM = "/images/logo.png";

// Create file system entries
const ProgMemFSEntry files[] PROGMEM = {{path1, file1_data, sizeof(file1_data) - 1}, {path2, nullptr, 0}, {path3, file2_data, sizeof(file2_data) - 1},
                                        {path4, file3_data, sizeof(file3_data) - 1}, {path5, nullptr, 0}, {path6, file4_data, sizeof(file4_data) - 1}};

// Create ProgMemFS instance
ProgMemFS fs(files, sizeof(files) / sizeof(ProgMemFSEntry));

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 1000);

  Serial.println("ProgMemFS DirectoryList Example");
  Serial.println("================================");

  // Check file vs directory
  Serial.println("\n--- Checking isFile() and isDirectory() ---");
  Serial.print("/readme.txt isFile: ");
  Serial.println(fs.isFile("/readme.txt") ? "Yes" : "No");
  Serial.print("/readme.txt isDirectory: ");
  Serial.println(fs.isDirectory("/readme.txt") ? "Yes" : "No");
  Serial.print("/docs/ isFile: ");
  Serial.println(fs.isFile("/docs/") ? "Yes" : "No");
  Serial.print("/docs/ isDirectory: ");
  Serial.println(fs.isDirectory("/docs/") ? "Yes" : "No");

  // List root directory
  Serial.println("\n--- Listing root directory (/) ---");
  listDirectory("/");

  // List docs directory
  Serial.println("\n--- Listing /docs/ directory ---");
  listDirectory("/docs/");

  // List images directory
  Serial.println("\n--- Listing /images/ directory ---");
  listDirectory("/images/");

  // Demonstrate rewind
  Serial.println("\n--- Demonstrating rewind() ---");
  ProgMemDirectory* dir = fs.openDirectory("/docs/");
  if (dir) {
    // First pass - enumerate files
    Serial.println("First enumeration:");
    ProgMemFile* file;
    while ((file = dir->openNextFile()) != nullptr) {
      Serial.print("  ");
      Serial.println(file->name());
      delete file;
    }

    // Rewind and enumerate again
    dir->rewind();
    Serial.println("After rewind(), second enumeration:");
    while ((file = dir->openNextFile()) != nullptr) {
      Serial.print("  ");
      Serial.println(file->name());
      delete file;
    }

    delete dir;
  }
}

// Helper function to list directory contents
void listDirectory(const char* path) {
  // Open directory
  ProgMemDirectory* dir = fs.openDirectory(path);
  if (!dir) {
    Serial.print("Failed to open directory: ");
    Serial.println(path);
    return;
  }

  Serial.print("Directory: ");
  Serial.println(dir->name());

  // List subdirectories
  Serial.println("  Subdirectories:");
  ProgMemDirectory* subdir;
  int subdirCount = 0;
  while ((subdir = dir->openNextDirectory()) != nullptr) {
    Serial.print("    [DIR] ");
    Serial.println(subdir->name());
    delete subdir;
    subdirCount++;
  }
  if (subdirCount == 0) {
    Serial.println("    (none)");
  }

  // Rewind to enumerate files
  dir->rewind();

  // List files
  Serial.println("  Files:");
  ProgMemFile* file;
  int fileCount = 0;
  while ((file = dir->openNextFile()) != nullptr) {
    Serial.print("    ");
    Serial.print(file->name());
    Serial.print(" (");
    Serial.print(file->size());
    Serial.println(" bytes)");
    delete file;
    fileCount++;
  }
  if (fileCount == 0) {
    Serial.println("    (none)");
  }

  // Clean up
  delete dir;
}

void loop() {}
