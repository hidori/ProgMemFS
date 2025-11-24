// test_progmemdirectory.cpp

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
#include "ProgMemDirectory.h"
#include "ProgMemFile.h"

const char file1_data[] PROGMEM = "Hello";
const char file2_data[] PROGMEM = "World";
const char file3_data[] PROGMEM = "Test";
const char file4_data[] PROGMEM = "Nested";
const char file5_data[] PROGMEM = "Deep";

const ProgMemFSEntry testFiles[] PROGMEM = {
  {"/hello.txt", file1_data, 5},
  {"/world.txt", file2_data, 5},
  {"/docs/readme.txt", file3_data, 4},
  {"/docs/nested/file.txt", file4_data, 6},
  {"/images/logo.png", file5_data, 4},
};

class ProgMemDirectoryTest : public ::testing::Test {
 protected:
  ProgMemFS fs{testFiles, 5, false};
};

TEST_F(ProgMemDirectoryTest, OpenDirectoryRoot) {
  ProgMemDirectory* dir = fs.openDirectory("/");
  ASSERT_NE(dir, nullptr);
  EXPECT_STREQ(dir->name(), "/");
  delete dir;
}

TEST_F(ProgMemDirectoryTest, OpenDirectorySubdir) {
  ProgMemDirectory* dir = fs.openDirectory("/docs/");
  ASSERT_NE(dir, nullptr);
  EXPECT_STREQ(dir->name(), "/docs/");
  delete dir;
}

TEST_F(ProgMemDirectoryTest, OpenDirectoryInvalidPath) {
  ProgMemDirectory* dir = fs.openDirectory("/nonexistent/");
  EXPECT_EQ(dir, nullptr);
}

TEST_F(ProgMemDirectoryTest, OpenDirectoryWithFilePath) {
  ProgMemDirectory* dir = fs.openDirectory("/hello.txt");
  EXPECT_EQ(dir, nullptr);
}

TEST_F(ProgMemDirectoryTest, OpenFileWithDirectoryPath) {
  ProgMemFile* file = fs.open("/docs/");
  EXPECT_EQ(file, nullptr);
}

TEST_F(ProgMemDirectoryTest, OpenNextFileFromRoot) {
  ProgMemDirectory* dir = fs.openDirectory("/");
  ASSERT_NE(dir, nullptr);

  ProgMemFile* file1 = dir->openNextFile();
  ASSERT_NE(file1, nullptr);
  delete file1;

  ProgMemFile* file2 = dir->openNextFile();
  ASSERT_NE(file2, nullptr);
  delete file2;

  ProgMemFile* file3 = dir->openNextFile();
  EXPECT_EQ(file3, nullptr);

  delete dir;
}

TEST_F(ProgMemDirectoryTest, OpenNextFileFromSubdir) {
  ProgMemDirectory* dir = fs.openDirectory("/docs/");
  ASSERT_NE(dir, nullptr);

  ProgMemFile* file = dir->openNextFile();
  ASSERT_NE(file, nullptr);
  delete file;

  ProgMemFile* file2 = dir->openNextFile();
  EXPECT_EQ(file2, nullptr);

  delete dir;
}

TEST_F(ProgMemDirectoryTest, OpenNextDirectoryFromRoot) {
  ProgMemDirectory* dir = fs.openDirectory("/");
  ASSERT_NE(dir, nullptr);

  ProgMemDirectory* subdir1 = dir->openNextDirectory();
  ASSERT_NE(subdir1, nullptr);
  delete subdir1;

  ProgMemDirectory* subdir2 = dir->openNextDirectory();
  ASSERT_NE(subdir2, nullptr);
  delete subdir2;

  ProgMemDirectory* subdir3 = dir->openNextDirectory();
  EXPECT_EQ(subdir3, nullptr);

  delete dir;
}

TEST_F(ProgMemDirectoryTest, OpenNextDirectoryFromSubdir) {
  ProgMemDirectory* dir = fs.openDirectory("/docs/");
  ASSERT_NE(dir, nullptr);

  ProgMemDirectory* subdir = dir->openNextDirectory();
  ASSERT_NE(subdir, nullptr);
  EXPECT_STREQ(subdir->name(), "/docs/nested/");
  delete subdir;

  ProgMemDirectory* subdir2 = dir->openNextDirectory();
  EXPECT_EQ(subdir2, nullptr);

  delete dir;
}

TEST_F(ProgMemDirectoryTest, Rewind) {
  ProgMemDirectory* dir = fs.openDirectory("/");
  ASSERT_NE(dir, nullptr);

  ProgMemFile* file1 = dir->openNextFile();
  ASSERT_NE(file1, nullptr);
  delete file1;

  ProgMemFile* file2 = dir->openNextFile();
  ASSERT_NE(file2, nullptr);
  delete file2;

  dir->rewind();

  ProgMemFile* file3 = dir->openNextFile();
  ASSERT_NE(file3, nullptr);
  delete file3;

  delete dir;
}

TEST_F(ProgMemDirectoryTest, IsDirectory) {
  EXPECT_TRUE(fs.isDirectory("/"));
  EXPECT_TRUE(fs.isDirectory("/docs/"));
  EXPECT_TRUE(fs.isDirectory("/images/"));
  EXPECT_FALSE(fs.isDirectory("/hello.txt"));
  EXPECT_FALSE(fs.isDirectory("/nonexistent/"));
}
