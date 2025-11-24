// test_progmemstream.cpp

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

#include <gtest/gtest.h>
#include <cstring>

#include "../src/ProgMemStream.h"

TEST(ProgMemStreamTest, Constructor) {
  const char data[] = "Hello, World!";
  ProgMemStream stream(data, strlen(data));

  EXPECT_EQ(stream.size(), strlen(data));
  EXPECT_EQ(stream.position(), 0);
  EXPECT_EQ(stream.available(), strlen(data));
}

TEST(ProgMemStreamTest, Read) {
  const char data[] = "Hello";
  ProgMemStream stream(data, strlen(data));

  EXPECT_EQ(stream.read(), 'H');
  EXPECT_EQ(stream.read(), 'e');
  EXPECT_EQ(stream.read(), 'l');
  EXPECT_EQ(stream.position(), 3);
  EXPECT_EQ(stream.available(), 2);
}

TEST(ProgMemStreamTest, Peek) {
  const char data[] = "Hello";
  ProgMemStream stream(data, strlen(data));

  EXPECT_EQ(stream.peek(), 'H');
  EXPECT_EQ(stream.peek(), 'H');
  EXPECT_EQ(stream.position(), 0);

  stream.read();
  EXPECT_EQ(stream.peek(), 'e');
  EXPECT_EQ(stream.position(), 1);
}

TEST(ProgMemStreamTest, ReadBytes) {
  const char data[] = "Hello, World!";
  ProgMemStream stream(data, strlen(data));
  char buffer[10];

  size_t bytesRead = stream.readBytes(buffer, 5);
  EXPECT_EQ(bytesRead, 5);
  EXPECT_EQ(strncmp(buffer, "Hello", 5), 0);
  EXPECT_EQ(stream.position(), 5);

  bytesRead = stream.readBytes(buffer, 8);
  EXPECT_EQ(bytesRead, 8);
  EXPECT_EQ(strncmp(buffer, ", World!", 8), 0);
  EXPECT_EQ(stream.position(), 13);

  bytesRead = stream.readBytes(buffer, 10);
  EXPECT_EQ(bytesRead, 0);
}

TEST(ProgMemStreamTest, Seek) {
  const char data[] = "Hello, World!";
  ProgMemStream stream(data, strlen(data));

  EXPECT_TRUE(stream.seek(7));
  EXPECT_EQ(stream.position(), 7);
  EXPECT_EQ(stream.read(), 'W');

  EXPECT_TRUE(stream.seek(0));
  EXPECT_EQ(stream.position(), 0);
  EXPECT_EQ(stream.read(), 'H');

  EXPECT_TRUE(stream.seek(strlen(data)));
  EXPECT_EQ(stream.position(), strlen(data));
  EXPECT_EQ(stream.available(), 0);

  EXPECT_FALSE(stream.seek(strlen(data) + 1));
}

TEST(ProgMemStreamTest, Close) {
  const char data[] = "Hello";
  ProgMemStream stream(data, strlen(data));

  stream.close();

  EXPECT_EQ(stream.available(), 0);
  EXPECT_EQ(stream.read(), -1);
  EXPECT_EQ(stream.peek(), -1);

  char buffer[10];
  EXPECT_EQ(stream.readBytes(buffer, 10), 0);

  EXPECT_FALSE(stream.seek(0));
}

TEST(ProgMemStreamTest, Write) {
  const char data[] = "Hello";
  ProgMemStream stream(data, strlen(data));

  EXPECT_EQ(stream.write((uint8_t)'A'), 0);

  const uint8_t buf[] = "test";
  EXPECT_EQ(stream.write(buf, 4), 0);
}

TEST(ProgMemStreamTest, Flush) {
  const char data[] = "Hello";
  ProgMemStream stream(data, strlen(data));

  stream.flush();
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
