// ProgMemFileSystem

#ifndef _PROGMEM_FILESYSTEM_H_
#define _PROGMEM_FILESYSTEM_H_

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

#include <Arduino.h>
#include "ProgMemStream.h"

struct FileSystemEntry {
  const char* path;
  const char* data;
  size_t length;
};

class ProgMemFileSystem {
 public:
  ProgMemFileSystem(const FileSystemEntry* entries, size_t numEntries, bool sorted = false)
    : files(entries), fileCount(numEntries), isSorted(sorted) {}

  ProgMemStream* open(const char* path);
  bool exists(const char* path);
  size_t count() const;

 private:
  int findIndex(const char* path) const;
  const FileSystemEntry* files;
  size_t fileCount;
  bool isSorted;
};

#endif  // _PROGMEM_FILESYSTEM_H_
