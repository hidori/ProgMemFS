// ProgMemStream

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

#include "ProgMemStream.h"

int ProgMemStream::available() {
  if (closed) {
    return 0;
  }
  return dataLength - pos;
}

int ProgMemStream::read() {
  if (closed || pos >= dataLength) {
    return -1;
  }
  char c = pgm_read_byte_near(data + pos);
  pos++;
  return c;
}

int ProgMemStream::peek() {
  if (closed || pos >= dataLength) {
    return -1;
  }
  return pgm_read_byte_near(data + pos);
}

size_t ProgMemStream::readBytes(char* buffer, size_t length) {
  if (closed || pos >= dataLength) {
    return 0;
  }
  size_t bytesToRead = min(length, dataLength - pos);
  if (bytesToRead == 0) {
    return 0;
  }
  memcpy_P(buffer, data + pos, bytesToRead);
  pos += bytesToRead;
  return bytesToRead;
}

size_t ProgMemStream::write(uint8_t) { return 0; }

size_t ProgMemStream::write(const uint8_t*, size_t) { return 0; }

void ProgMemStream::flush() {}

bool ProgMemStream::seek(size_t position) {
  if (closed || position > dataLength) {
    return false;
  }
  pos = position;
  return true;
}

size_t ProgMemStream::position() const { return pos; }

size_t ProgMemStream::size() const { return dataLength; }

void ProgMemStream::close() { closed = true; }
