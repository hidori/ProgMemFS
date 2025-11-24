// ProgMemDirectory

// MIT License
//
// Copyright (c) 2025 Hiroaki SHIBUKI a.k.a. hidori
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "ProgMemDirectory.h"

#include "ProgMemFS.h"
#include "ProgMemFile.h"

ProgMemFile* ProgMemDirectory::openNextFile() {
  static char fullPath[64];
  while (currentIndex < fileCount) {
    const char* filePath = files[currentIndex].path;
    if (isDirectChildFile(filePath)) {
      size_t pathLen = strlen_P(filePath);
      size_t copyLen = pathLen < sizeof(fullPath) - 1 ? pathLen : sizeof(fullPath) - 1;
      strncpy_P(fullPath, filePath, copyLen);
      fullPath[copyLen] = '\0';
      ProgMemFile* file = new ProgMemFile(fullPath, files[currentIndex].data, files[currentIndex].length);
      currentIndex++;
      return file;
    }
    currentIndex++;
  }
  return nullptr;
}

ProgMemDirectory* ProgMemDirectory::openNextDirectory() {
  while (currentIndex < fileCount) {
    const char* filePath = files[currentIndex].path;
    size_t childDirEndIndex;
    if (isDirectChildDirectory(filePath, childDirEndIndex)) {
      if (!alreadyReturnedDirectory(filePath, childDirEndIndex)) {
        if (returnedDirCount < MAX_RETURNED_DIRS) {
          returnedDirEndIndices[returnedDirCount++] = childDirEndIndex;
        }
        static char dirPath[64];
        size_t copyLen = childDirEndIndex < sizeof(dirPath) - 1 ? childDirEndIndex : sizeof(dirPath) - 1;
        strncpy_P(dirPath, filePath, copyLen);
        dirPath[copyLen] = '\0';
        return new ProgMemDirectory(dirPath, files, fileCount);
      }
    }
    currentIndex++;
  }
  return nullptr;
}

void ProgMemDirectory::rewind() {
  currentIndex = 0;
  returnedDirCount = 0;
}

const char* ProgMemDirectory::name() const { return path; }

bool ProgMemDirectory::isDirectChildFile(const char* filePath) const {
  size_t filePathLen = strlen_P(filePath);

  if (filePathLen <= pathLength) {
    return false;
  }

  if (strncmp_P(path, filePath, pathLength) != 0) {
    return false;
  }

  const char* remaining = filePath + pathLength;
  for (size_t i = 0; remaining[i] != '\0'; i++) {
    if (pgm_read_byte(&remaining[i]) == '/') {
      return false;
    }
  }

  return true;
}

bool ProgMemDirectory::isDirectChildDirectory(const char* filePath, size_t& childDirEndIndex) const {
  size_t filePathLen = strlen_P(filePath);

  if (filePathLen <= pathLength) {
    return false;
  }

  if (strncmp_P(path, filePath, pathLength) != 0) {
    return false;
  }

  const char* remaining = filePath + pathLength;
  for (size_t i = 0;; i++) {
    char c = pgm_read_byte(&remaining[i]);
    if (c == '\0') {
      return false;
    }
    if (c == '/') {
      childDirEndIndex = pathLength + i + 1;
      return true;
    }
  }
}

bool ProgMemDirectory::alreadyReturnedDirectory(const char* dirPath, size_t dirEndIndex) const {
  for (size_t i = 0; i < returnedDirCount; i++) {
    if (returnedDirEndIndices[i] == dirEndIndex) {
      return true;
    }
  }
  return false;
}
