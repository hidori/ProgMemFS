// ProgMemStream

#ifndef _PROGMEM_STREAM_H_
#define _PROGMEM_STREAM_H_

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
#if __has_include(<Stream.h>)
#include <Stream.h>
#endif

// ProgMemStream class definition
class ProgMemStream : public Stream {
 public:
  // Constructor
  ProgMemStream(const char* progmemData, size_t length) : data(progmemData), dataLength(length), pos(0), closed(false) {}

  // Get number of bytes available for reading
  int available() override;
  // Read a byte from the stream
  int read() override;
  // Peek at the next byte without advancing the position
  int peek() override;
  // Read multiple bytes into a buffer
  size_t readBytes(char* buffer, size_t length);
  // Write a byte to the stream (not supported)
  size_t write(uint8_t) override;
  // Write multiple bytes to the stream (not supported)
  size_t write(const uint8_t*, size_t) override;
  // Flush the stream (no-op for read-only stream)
  void flush() override;
  // Seek to a specific position in the stream
  bool seek(size_t position);
  // Get the current position in the stream
  size_t position() const;
  // Get the size of the stream
  size_t size() const;
  // Close the stream
  void close();

 private:
  const char* data;
  size_t dataLength;
  size_t pos;
  bool closed;
};

#endif  // _PROGMEM_STREAM_H_
