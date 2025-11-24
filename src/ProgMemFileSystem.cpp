// ProgMemFileSystem

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

#include "ProgMemFileSystem.h"

ProgMemStream* ProgMemFileSystem::open(const char* path) {
  int index = findIndex(path);
  if (index >= 0) {
    return new ProgMemStream(files[index].data, files[index].length);
  }
  return nullptr;
}

bool ProgMemFileSystem::exists(const char* path) {
  return findIndex(path) >= 0;
}

size_t ProgMemFileSystem::count() const {
  return fileCount;
}

int ProgMemFileSystem::findIndex(const char* path) const {
  if (isSorted) {
    int left = 0;
    int right = fileCount - 1;
    while (left <= right) {
      int mid = left + (right - left) / 2;
      int cmp = strcmp_P(path, files[mid].path);
      if (cmp == 0) {
        return mid;
      } else if (cmp < 0) {
        right = mid - 1;
      } else {
        left = mid + 1;
      }
    }
    return -1;
  } else {
    for (size_t i = 0; i < fileCount; i++) {
      if (strcmp_P(path, files[i].path) == 0) {
        return i;
      }
    }
    return -1;
  }
}
