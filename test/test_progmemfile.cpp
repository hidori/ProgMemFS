// test_progmemfile.cpp

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

#include "../src/ProgMemFile.h"

TEST(ProgMemFileTest, Constructor) {
  const char data[] = "Hello, World!";
  ProgMemFile file("/test.txt", data, strlen(data));

  EXPECT_EQ(file.size(), strlen(data));
  EXPECT_EQ(file.position(), 0);
  EXPECT_EQ(file.available(), strlen(data));
  EXPECT_STREQ(file.name(), "/test.txt");
}

TEST(ProgMemFileTest, Read) {
  const char data[] = "Hello";
  ProgMemFile file("/hello.txt", data, strlen(data));

  EXPECT_EQ(file.read(), 'H');
  EXPECT_EQ(file.read(), 'e');
  EXPECT_EQ(file.read(), 'l');
  EXPECT_EQ(file.position(), 3);
  EXPECT_EQ(file.available(), 2);
}

TEST(ProgMemFileTest, Peek) {
  const char data[] = "Hello";
  ProgMemFile file("/hello.txt", data, strlen(data));

  EXPECT_EQ(file.peek(), 'H');
  EXPECT_EQ(file.peek(), 'H');
  EXPECT_EQ(file.position(), 0);

  file.read();
  EXPECT_EQ(file.peek(), 'e');
  EXPECT_EQ(file.position(), 1);
}

TEST(ProgMemFileTest, ReadBytes) {
  const char data[] = "Hello, World!";
  ProgMemFile file("/hello.txt", data, strlen(data));
  char buffer[10];

  size_t bytesRead = file.readBytes(buffer, 5);
  EXPECT_EQ(bytesRead, 5);
  EXPECT_EQ(strncmp(buffer, "Hello", 5), 0);
  EXPECT_EQ(file.position(), 5);

  bytesRead = file.readBytes(buffer, 8);
  EXPECT_EQ(bytesRead, 8);
  EXPECT_EQ(strncmp(buffer, ", World!", 8), 0);
  EXPECT_EQ(file.position(), 13);

  bytesRead = file.readBytes(buffer, 10);
  EXPECT_EQ(bytesRead, 0);
}

TEST(ProgMemFileTest, Seek) {
  const char data[] = "Hello, World!";
  ProgMemFile file("/hello.txt", data, strlen(data));

  EXPECT_TRUE(file.seek(7));
  EXPECT_EQ(file.position(), 7);
  EXPECT_EQ(file.read(), 'W');

  EXPECT_TRUE(file.seek(0));
  EXPECT_EQ(file.position(), 0);
  EXPECT_EQ(file.read(), 'H');

  EXPECT_TRUE(file.seek(strlen(data)));
  EXPECT_EQ(file.position(), strlen(data));
  EXPECT_EQ(file.available(), 0);

  EXPECT_FALSE(file.seek(strlen(data) + 1));
}

TEST(ProgMemFileTest, Write) {
  const char data[] = "Hello";
  ProgMemFile file("/hello.txt", data, strlen(data));

  EXPECT_EQ(file.write((uint8_t)'A'), 0);

  const uint8_t buf[] = "test";
  EXPECT_EQ(file.write(buf, 4), 0);
}

TEST(ProgMemFileTest, Flush) {
  const char data[] = "Hello";
  ProgMemFile file("/hello.txt", data, strlen(data));

  file.flush();
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
