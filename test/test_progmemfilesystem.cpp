// test_progmemfilesystem.cpp

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
#include "ProgMemFileSystem.h"

const char test_file1[] = "Hello, World!";
const char test_file2[] = "Second file content";
const char test_file3[] = "Third file\nwith multiple\nlines";

const char path1[] = "/test1.txt";
const char path2[] = "/dir/test2.txt";
const char path3[] = "/test3.txt";

const FileSystemEntry test_files[] = {
  {path1, test_file1, sizeof(test_file1) - 1},
  {path2, test_file2, sizeof(test_file2) - 1},
  {path3, test_file3, sizeof(test_file3) - 1}
};

const char sorted_path1[] = "/a.txt";
const char sorted_path2[] = "/b.txt";
const char sorted_path3[] = "/c.txt";
const char sorted_path4[] = "/d.txt";

const FileSystemEntry sorted_files[] = {
  {sorted_path1, test_file1, sizeof(test_file1) - 1},
  {sorted_path2, test_file2, sizeof(test_file2) - 1},
  {sorted_path3, test_file3, sizeof(test_file3) - 1},
  {sorted_path4, test_file1, sizeof(test_file1) - 1}
};

TEST(ProgMemFileSystemTest, Count) {
  ProgMemFileSystem fs(test_files, sizeof(test_files) / sizeof(FileSystemEntry));
  EXPECT_EQ(fs.count(), 3);
}

TEST(ProgMemFileSystemTest, Exists) {
  ProgMemFileSystem fs(test_files, sizeof(test_files) / sizeof(FileSystemEntry));

  EXPECT_TRUE(fs.exists("/test1.txt"));
  EXPECT_TRUE(fs.exists("/dir/test2.txt"));
  EXPECT_TRUE(fs.exists("/test3.txt"));
  EXPECT_FALSE(fs.exists("/notfound.txt"));
  EXPECT_FALSE(fs.exists("/test"));
}

TEST(ProgMemFileSystemTest, OpenValidFile) {
  ProgMemFileSystem fs(test_files, sizeof(test_files) / sizeof(FileSystemEntry));

  ProgMemStream* stream = fs.open("/test1.txt");
  ASSERT_NE(stream, nullptr);

  EXPECT_EQ(stream->size(), 13);
  EXPECT_EQ(stream->available(), 13);

  char buffer[14];
  size_t read = stream->readBytes(buffer, 13);
  buffer[read] = '\0';

  EXPECT_EQ(read, 13);
  EXPECT_STREQ(buffer, "Hello, World!");

  delete stream;
}

TEST(ProgMemFileSystemTest, OpenInvalidFile) {
  ProgMemFileSystem fs(test_files, sizeof(test_files) / sizeof(FileSystemEntry));

  ProgMemStream* stream = fs.open("/notfound.txt");
  EXPECT_EQ(stream, nullptr);
}

TEST(ProgMemFileSystemTest, OpenMultipleFiles) {
  ProgMemFileSystem fs(test_files, sizeof(test_files) / sizeof(FileSystemEntry));

  ProgMemStream* stream1 = fs.open("/test1.txt");
  ProgMemStream* stream2 = fs.open("/dir/test2.txt");

  ASSERT_NE(stream1, nullptr);
  ASSERT_NE(stream2, nullptr);

  EXPECT_EQ(stream1->size(), 13);
  EXPECT_EQ(stream2->size(), 19);

  delete stream1;
  delete stream2;
}

TEST(ProgMemFileSystemTest, ReadMultilineFile) {
  ProgMemFileSystem fs(test_files, sizeof(test_files) / sizeof(FileSystemEntry));

  ProgMemStream* stream = fs.open("/test3.txt");
  ASSERT_NE(stream, nullptr);

  char buffer[50];
  size_t read = stream->readBytes(buffer, sizeof(buffer) - 1);
  buffer[read] = '\0';

  EXPECT_STREQ(buffer, "Third file\nwith multiple\nlines");

  delete stream;
}

TEST(ProgMemFileSystemTest, EmptyFileSystem) {
  FileSystemEntry empty_files[] = {};
  ProgMemFileSystem fs(empty_files, 0);

  EXPECT_EQ(fs.count(), 0);
  EXPECT_FALSE(fs.exists("/any.txt"));
  EXPECT_EQ(fs.open("/any.txt"), nullptr);
}

TEST(ProgMemFileSystemTest, BinarySearchExists) {
  ProgMemFileSystem fs(sorted_files, sizeof(sorted_files) / sizeof(FileSystemEntry), true);

  EXPECT_TRUE(fs.exists("/a.txt"));
  EXPECT_TRUE(fs.exists("/b.txt"));
  EXPECT_TRUE(fs.exists("/c.txt"));
  EXPECT_TRUE(fs.exists("/d.txt"));
  EXPECT_FALSE(fs.exists("/notfound.txt"));
  EXPECT_FALSE(fs.exists("/z.txt"));
}

TEST(ProgMemFileSystemTest, BinarySearchOpen) {
  ProgMemFileSystem fs(sorted_files, sizeof(sorted_files) / sizeof(FileSystemEntry), true);

  ProgMemStream* stream1 = fs.open("/a.txt");
  ProgMemStream* stream2 = fs.open("/c.txt");
  ProgMemStream* stream3 = fs.open("/notfound.txt");

  ASSERT_NE(stream1, nullptr);
  ASSERT_NE(stream2, nullptr);
  EXPECT_EQ(stream3, nullptr);

  EXPECT_EQ(stream1->size(), 13);
  EXPECT_EQ(stream2->size(), 30);

  delete stream1;
  delete stream2;
}

TEST(ProgMemFileSystemTest, BinarySearchEdgeCases) {
  ProgMemFileSystem fs(sorted_files, sizeof(sorted_files) / sizeof(FileSystemEntry), true);

  EXPECT_TRUE(fs.exists("/a.txt"));
  EXPECT_TRUE(fs.exists("/d.txt"));
  EXPECT_FALSE(fs.exists("/0.txt"));
  EXPECT_FALSE(fs.exists("/e.txt"));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
