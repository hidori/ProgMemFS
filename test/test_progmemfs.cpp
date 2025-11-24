// test_progmemfs.cpp

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
#include "ProgMemFS.h"

const char test_file1[] = "Hello, World!";
const char test_file2[] = "Second file content";
const char test_file3[] = "Third file\nwith multiple\nlines";

const char path1[] = "/test1.txt";
const char path2[] = "/dir/test2.txt";
const char path3[] = "/test3.txt";

const ProgMemFSEntry test_files[] = {
  {path1, test_file1, sizeof(test_file1) - 1},
  {path2, test_file2, sizeof(test_file2) - 1},
  {path3, test_file3, sizeof(test_file3) - 1}
};

const char sorted_path1[] = "/a.txt";
const char sorted_path2[] = "/b.txt";
const char sorted_path3[] = "/c.txt";
const char sorted_path4[] = "/d.txt";

const ProgMemFSEntry sorted_files[] = {
  {sorted_path1, test_file1, sizeof(test_file1) - 1},
  {sorted_path2, test_file2, sizeof(test_file2) - 1},
  {sorted_path3, test_file3, sizeof(test_file3) - 1},
  {sorted_path4, test_file1, sizeof(test_file1) - 1}
};

TEST(ProgMemFSTest, IsFile) {
  ProgMemFS fs(test_files, sizeof(test_files) / sizeof(ProgMemFSEntry));

  EXPECT_TRUE(fs.isFile("/test1.txt"));
  EXPECT_TRUE(fs.isFile("/dir/test2.txt"));
  EXPECT_TRUE(fs.isFile("/test3.txt"));
  EXPECT_FALSE(fs.isFile("/notfound.txt"));
  EXPECT_FALSE(fs.isFile("/test"));
}

TEST(ProgMemFSTest, OpenValidFile) {
  ProgMemFS fs(test_files, sizeof(test_files) / sizeof(ProgMemFSEntry));

  ProgMemFile* file = fs.open("/test1.txt");
  ASSERT_NE(file, nullptr);

  EXPECT_EQ(file->size(), 13);
  EXPECT_EQ(file->available(), 13);

  char buffer[14];
  size_t read = file->readBytes(buffer, 13);
  buffer[read] = '\0';

  EXPECT_EQ(read, 13);
  EXPECT_STREQ(buffer, "Hello, World!");

  delete file;
}

TEST(ProgMemFSTest, OpenInvalidFile) {
  ProgMemFS fs(test_files, sizeof(test_files) / sizeof(ProgMemFSEntry));

  ProgMemFile* file = fs.open("/notfound.txt");
  EXPECT_EQ(file, nullptr);
}

TEST(ProgMemFSTest, OpenMultipleFiles) {
  ProgMemFS fs(test_files, sizeof(test_files) / sizeof(ProgMemFSEntry));

  ProgMemFile* file1 = fs.open("/test1.txt");
  ProgMemFile* file2 = fs.open("/dir/test2.txt");

  ASSERT_NE(file1, nullptr);
  ASSERT_NE(file2, nullptr);

  EXPECT_EQ(file1->size(), 13);
  EXPECT_EQ(file2->size(), 19);

  delete file1;
  delete file2;
}

TEST(ProgMemFSTest, ReadMultilineFile) {
  ProgMemFS fs(test_files, sizeof(test_files) / sizeof(ProgMemFSEntry));

  ProgMemFile* file = fs.open("/test3.txt");
  ASSERT_NE(file, nullptr);

  char buffer[50];
  size_t read = file->readBytes(buffer, sizeof(buffer) - 1);
  buffer[read] = '\0';

  EXPECT_STREQ(buffer, "Third file\nwith multiple\nlines");

  delete file;
}

TEST(ProgMemFSTest, EmptyFileSystem) {
  ProgMemFSEntry empty_files[] = {};
  ProgMemFS fs(empty_files, 0);

  EXPECT_FALSE(fs.isFile("/any.txt"));
  EXPECT_EQ(fs.open("/any.txt"), nullptr);
}

TEST(ProgMemFSTest, BinarySearchIsFile) {
  ProgMemFS fs(sorted_files, sizeof(sorted_files) / sizeof(ProgMemFSEntry), true);

  EXPECT_TRUE(fs.isFile("/a.txt"));
  EXPECT_TRUE(fs.isFile("/b.txt"));
  EXPECT_TRUE(fs.isFile("/c.txt"));
  EXPECT_TRUE(fs.isFile("/d.txt"));
  EXPECT_FALSE(fs.isFile("/notfound.txt"));
  EXPECT_FALSE(fs.isFile("/z.txt"));
}

TEST(ProgMemFSTest, BinarySearchOpen) {
  ProgMemFS fs(sorted_files, sizeof(sorted_files) / sizeof(ProgMemFSEntry), true);

  ProgMemFile* file1 = fs.open("/a.txt");
  ProgMemFile* file2 = fs.open("/c.txt");
  ProgMemFile* file3 = fs.open("/notfound.txt");

  ASSERT_NE(file1, nullptr);
  ASSERT_NE(file2, nullptr);
  EXPECT_EQ(file3, nullptr);

  EXPECT_EQ(file1->size(), 13);
  EXPECT_EQ(file2->size(), 30);

  delete file1;
  delete file2;
}

TEST(ProgMemFSTest, BinarySearchEdgeCases) {
  ProgMemFS fs(sorted_files, sizeof(sorted_files) / sizeof(ProgMemFSEntry), true);

  EXPECT_TRUE(fs.isFile("/a.txt"));
  EXPECT_TRUE(fs.isFile("/d.txt"));
  EXPECT_FALSE(fs.isFile("/0.txt"));
  EXPECT_FALSE(fs.isFile("/e.txt"));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
