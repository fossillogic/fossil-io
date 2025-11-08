/**
 * -----------------------------------------------------------------------------
 * Project: Fossil Logic
 *
 * This file is part of the Fossil Logic project, which aims to develop
 * high-performance, cross-platform applications and libraries. The code
 * contained herein is licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 * Author: Michael Gene Brockus (Dreamer)
 * Date: 04/05/2014
 *
 * Copyright (C) 2014-2025 Fossil Logic. All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include <fossil/pizza/framework.h>

#include "fossil/io/framework.h"


// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Utilites
// * * * * * * * * * * * * * * * * * * * * * * * *
// Setup steps for things like test fixtures and
// mock objects are set here.
// * * * * * * * * * * * * * * * * * * * * * * * *

// Define the test suite and add test cases
FOSSIL_SUITE(cpp_archive_suite);

// Setup function for the test suite
FOSSIL_SETUP(cpp_archive_suite) {
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(cpp_archive_suite) {
    // Teardown code here
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

// Test C++ Archive class functionality
FOSSIL_TEST(cpp_test_archive_class_create) {
    const std::string archive_path = "test_cpp_create.zip";
    
    // Create archive using C++ class
    auto archive = fossil::io::Archive::create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_ITS_TRUE(archive.is_valid());
    
    fossil::io::Stream::remove(archive_path);
}

FOSSIL_TEST(cpp_test_archive_class_open) {
    const std::string archive_path = "test_cpp_open.zip";
    
    // Create archive first
    auto create_archive = fossil::io::Archive::create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_ITS_TRUE(create_archive.is_valid());
    create_archive = fossil::io::Archive("", FOSSIL_IO_ARCHIVE_UNKNOWN, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    
    // Open existing archive
    fossil::io::Archive open_archive(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_ITS_TRUE(open_archive.is_valid());
    
    fossil::io::Stream::remove(archive_path);
}

FOSSIL_TEST(cpp_test_archive_class_move_constructor) {
    const std::string archive_path = "test_cpp_move.zip";
    
    // Create archive
    auto archive1 = fossil::io::Archive::create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_ITS_TRUE(archive1.is_valid());
    
    // Move construct
    fossil::io::Archive archive2 = std::move(archive1);
    ASSUME_ITS_TRUE(archive2.is_valid());
    
    fossil::io::Stream::remove(archive_path);
}

FOSSIL_TEST(cpp_test_archive_class_move_assignment) {
    const std::string archive_path1 = "test_cpp_move1.zip";
    const std::string archive_path2 = "test_cpp_move2.zip";
    
    // Create two archives
    auto archive1 = fossil::io::Archive::create(archive_path1, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    auto archive2 = fossil::io::Archive::create(archive_path2, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_ITS_TRUE(archive1.is_valid());
    ASSUME_ITS_TRUE(archive2.is_valid());
    
    // Move assign
    archive2 = std::move(archive1);
    ASSUME_ITS_TRUE(archive2.is_valid());
    
    fossil::io::Stream::remove(archive_path1);
    fossil::io::Stream::remove(archive_path2);
}

FOSSIL_TEST(cpp_test_archive_class_get_type) {
    const std::string zip_path = "test_cpp_type.zip";
    
    // Create a ZIP file with header
    fossil_fstream_t stream;
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&stream, zip_path, "wb"));
    unsigned char zip_header[] = {0x50, 0x4B, 0x03, 0x04};
    fossil::io::Stream::write(&stream, zip_header, sizeof(zip_header), 1);
    fossil::io::Stream::close(&stream);
    
    // Test type detection using C++ class
    fossil::io::Archive archive(zip_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    fossil_io_archive_type_t detected_type = archive.get_type(zip_path);
    ASSUME_ITS_EQUAL_I32(FOSSIL_IO_ARCHIVE_ZIP, detected_type);
    
    fossil::io::Stream::remove(zip_path);
}

FOSSIL_TEST(cpp_test_archive_class_list) {
    const std::string archive_path = "test_cpp_list.zip";
    const std::string test_file1 = "temp_cpp_list1.txt";
    const std::string test_file2 = "temp_cpp_list2.txt";
    
    // Create test files
    fossil_fstream_t temp_stream;
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&temp_stream, test_file1, "w"));
    fossil::io::Stream::write(&temp_stream, "Content1", 8, 1);
    fossil::io::Stream::close(&temp_stream);
    
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&temp_stream, test_file2, "w"));
    fossil::io::Stream::write(&temp_stream, "Content2", 8, 1);
    fossil::io::Stream::close(&temp_stream);
    
    // Create archive and add files using C++ class
    auto archive = fossil::io::Archive::create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_ITS_TRUE(archive.is_valid());
    ASSUME_ITS_TRUE(archive.add_file(test_file1, "file1.txt"));
    ASSUME_ITS_TRUE(archive.add_file(test_file2, "file2.txt"));
    
    // List entries using STL vector
    auto entries = archive.list();
    ASSUME_ITS_EQUAL_SIZE(2, entries.size());
    
    // Verify entries
    bool found_file1 = false, found_file2 = false;
    for (const auto& entry : entries) {
        if (std::string(entry.name) == "file1.txt") {
            found_file1 = true;
        } else if (std::string(entry.name) == "file2.txt") {
            found_file2 = true;
        }
    }
    ASSUME_ITS_TRUE(found_file1);
    ASSUME_ITS_TRUE(found_file2);
    
    fossil::io::Stream::remove(test_file1);
    fossil::io::Stream::remove(test_file2);
    fossil::io::Stream::remove(archive_path);
}

FOSSIL_TEST(cpp_test_archive_class_extract_file) {
    const std::string archive_path = "test_cpp_extract.zip";
    const std::string test_file = "temp_cpp_source.txt";
    const std::string extracted_file = "temp_cpp_extracted.txt";
    const std::string content = "C++ extract test content";
    
    // Create test file
    fossil_fstream_t temp_stream;
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&temp_stream, test_file, "w"));
    fossil::io::Stream::write(&temp_stream, content.c_str(), content.length(), 1);
    fossil::io::Stream::close(&temp_stream);
    
    // Create archive and add file
    auto archive = fossil::io::Archive::create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_ITS_TRUE(archive.is_valid());
    ASSUME_ITS_TRUE(archive.add_file(test_file, "source.txt"));
    archive = fossil::io::Archive("", FOSSIL_IO_ARCHIVE_UNKNOWN, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    
    // Open for reading and extract using C++ class
    fossil::io::Archive read_archive(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_ITS_TRUE(read_archive.is_valid());
    ASSUME_ITS_TRUE(read_archive.extract_file("source.txt", extracted_file));
    
    // Verify extraction
    ASSUME_ITS_TRUE(fossil::io::Stream::file_exists(extracted_file) == FOSSIL_ERROR_OK);
    
    fossil::io::Stream::remove(test_file);
    fossil::io::Stream::remove(extracted_file);
    fossil::io::Stream::remove(archive_path);
}

FOSSIL_TEST(cpp_test_archive_class_extract_all) {
    const std::string archive_path = "test_cpp_extract_all.zip";
    const std::string extract_dir = "temp_cpp_extract_dir";
    const std::string test_file = "temp_cpp_all.txt";
    
    // Create test file
    fossil_fstream_t temp_stream;
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&temp_stream, test_file, "w"));
    fossil::io::Stream::write(&temp_stream, "C++ extract all", 15, 1);
    fossil::io::Stream::close(&temp_stream);
    
    // Create archive and add file
    auto archive = fossil::io::Archive::create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_ITS_TRUE(archive.is_valid());
    ASSUME_ITS_TRUE(archive.add_file(test_file, "all.txt"));
    archive = fossil::io::Archive("", FOSSIL_IO_ARCHIVE_UNKNOWN, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    
    // Open for reading and extract all
    fossil::io::Archive read_archive(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_ITS_TRUE(read_archive.is_valid());
    ASSUME_ITS_TRUE(read_archive.extract_all(extract_dir));
    
    fossil::io::Stream::remove(test_file);
    fossil::io::Stream::remove(archive_path);
}

FOSSIL_TEST(cpp_test_archive_class_add_directory) {
    const std::string archive_path = "test_cpp_add_dir.zip";
    const std::string test_dir = "temp_cpp_test_dir";
    
    // Create archive using C++ class
    auto archive = fossil::io::Archive::create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_ITS_TRUE(archive.is_valid());
    
    // Test adding directory
    archive.add_directory(test_dir, "testdir");
    
    fossil::io::Stream::remove(archive_path);
}

FOSSIL_TEST(cpp_test_archive_class_exists) {
    const std::string archive_path = "test_cpp_exists.zip";
    const std::string test_file = "temp_cpp_exists.txt";
    
    // Create test file
    fossil_fstream_t temp_stream;
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&temp_stream, test_file, "w"));
    fossil::io::Stream::write(&temp_stream, "C++ exists test", 15, 1);
    fossil::io::Stream::close(&temp_stream);
    
    // Create archive and test existence using C++ class
    auto archive = fossil::io::Archive::create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_ITS_TRUE(archive.is_valid());
    ASSUME_ITS_TRUE(archive.add_file(test_file, "exists.txt"));
    
    // Test exists method
    ASSUME_ITS_TRUE(archive.exists("exists.txt"));
    ASSUME_ITS_FALSE(archive.exists("nonexistent.txt"));
    
    fossil::io::Stream::remove(test_file);
    fossil::io::Stream::remove(archive_path);
}

FOSSIL_TEST(cpp_test_archive_class_entry_size) {
    const std::string archive_path = "test_cpp_size.zip";
    const std::string test_file = "temp_cpp_size.txt";
    const std::string content = "C++ entry size test content";
    
    // Create test file
    fossil_fstream_t temp_stream;
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&temp_stream, test_file, "w"));
    fossil::io::Stream::write(&temp_stream, content.c_str(), content.length(), 1);
    fossil::io::Stream::close(&temp_stream);
    
    // Create archive and test entry size using C++ class
    auto archive = fossil::io::Archive::create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_ITS_TRUE(archive.is_valid());
    ASSUME_ITS_TRUE(archive.add_file(test_file, "size_test.txt"));
    
    // Test entry_size method
    ssize_t size = archive.entry_size("size_test.txt");
    ASSUME_ITS_EQUAL_I32(content.length(), size);
    
    // Test non-existent entry
    ssize_t bad_size = archive.entry_size("nonexistent.txt");
    ASSUME_ITS_EQUAL_I32(-1, bad_size);
    
    fossil::io::Stream::remove(test_file);
    fossil::io::Stream::remove(archive_path);
}

FOSSIL_TEST(cpp_test_archive_class_remove) {
    const std::string archive_path = "test_cpp_remove.zip";
    const std::string test_file = "temp_cpp_remove.txt";
    
    // Create test file
    fossil_fstream_t temp_stream;
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&temp_stream, test_file, "w"));
    fossil::io::Stream::write(&temp_stream, "C++ remove test", 15, 1);
    fossil::io::Stream::close(&temp_stream);
    
    // Create archive and test removal using C++ class
    auto archive = fossil::io::Archive::create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_ITS_TRUE(archive.is_valid());
    ASSUME_ITS_TRUE(archive.add_file(test_file, "to_remove.txt"));
    
    // Verify exists before removal
    ASSUME_ITS_TRUE(archive.exists("to_remove.txt"));
    
    // Remove and verify
    ASSUME_ITS_TRUE(archive.remove("to_remove.txt"));
    ASSUME_ITS_FALSE(archive.exists("to_remove.txt"));
    
    fossil::io::Stream::remove(test_file);
    fossil::io::Stream::remove(archive_path);
}

FOSSIL_TEST(cpp_test_archive_class_print) {
    const std::string archive_path = "test_cpp_print.zip";
    const std::string test_file = "temp_cpp_print.txt";
    
    // Create test file
    fossil_fstream_t temp_stream;
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&temp_stream, test_file, "w"));
    fossil::io::Stream::write(&temp_stream, "C++ print test", 14, 1);
    fossil::io::Stream::close(&temp_stream);
    
    // Create archive and test print using C++ class
    auto archive = fossil::io::Archive::create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_ITS_TRUE(archive.is_valid());
    ASSUME_ITS_TRUE(archive.add_file(test_file, "print.txt"));
    
    // Test print method (should not crash)
    archive.print();
    
    fossil::io::Stream::remove(test_file);
    fossil::io::Stream::remove(archive_path);
}

// Test archive type detection function
FOSSIL_TEST(cpp_test_archive_get_type_zip) {
    const char *zip_path = "test_type.zip";
    
    // Create a ZIP archive
    fossil_io_archive_t *archive = fossil_io_archive_create(zip_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_NOT_CNULL(archive);
    
    // Write ZIP signature (PK) to file
    fossil_fstream_t stream;
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&stream, zip_path, "wb"));
    unsigned char zip_header[] = {0x50, 0x4B, 0x03, 0x04};
    fossil::io::Stream::write(&stream, zip_header, sizeof(zip_header), 1);
    fossil::io::Stream::close(&stream);
    fossil_io_archive_close(archive);
    
    // Test type detection
    fossil_io_archive_type_t detected_type = fossil_io_archive_get_type(zip_path);
    ASSUME_ITS_EQUAL_I32(FOSSIL_IO_ARCHIVE_ZIP, detected_type);
    
    fossil::io::Stream::remove(zip_path);
}

FOSSIL_TEST(cpp_test_archive_get_type_gzip) {
    const char *gz_path = "test_type.gz";
    
    // Write GZIP signature
    fossil_fstream_t stream;
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&stream, gz_path, "wb"));
    unsigned char gz_header[] = {0x1F, 0x8B, 0x08, 0x00};
    fossil::io::Stream::write(&stream, gz_header, sizeof(gz_header), 1);
    fossil::io::Stream::close(&stream);
    
    fossil_io_archive_type_t detected_type = fossil_io_archive_get_type(gz_path);
    ASSUME_ITS_EQUAL_I32(FOSSIL_IO_ARCHIVE_GZ, detected_type);
    
    fossil::io::Stream::remove(gz_path);
}

FOSSIL_TEST(cpp_test_archive_get_type_bzip2) {
    const char *bz2_path = "test_type.bz2";
    
    // Write BZIP2 signature
    fossil_fstream_t stream;
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&stream, bz2_path, "wb"));
    unsigned char bz2_header[] = {0x42, 0x5A, 0x68, 0x39};
    fossil::io::Stream::write(&stream, bz2_header, sizeof(bz2_header), 1);
    fossil::io::Stream::close(&stream);
    
    fossil_io_archive_type_t detected_type = fossil_io_archive_get_type(bz2_path);
    ASSUME_ITS_EQUAL_I32(FOSSIL_IO_ARCHIVE_BZ2, detected_type);
    
    fossil::io::Stream::remove(bz2_path);
}

FOSSIL_TEST(cpp_test_archive_get_type_xz) {
    const char *xz_path = "test_type.xz";
    
    // Write XZ signature
    fossil_fstream_t stream;
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&stream, xz_path, "wb"));
    unsigned char xz_header[] = {0xFD, '7', 'z', 'X', 'Z', 0x00};
    fossil::io::Stream::write(&stream, xz_header, sizeof(xz_header), 1);
    fossil::io::Stream::close(&stream);
    
    fossil_io_archive_type_t detected_type = fossil_io_archive_get_type(xz_path);
    ASSUME_ITS_EQUAL_I32(FOSSIL_IO_ARCHIVE_XZ, detected_type);
    
    fossil::io::Stream::remove(xz_path);
}

FOSSIL_TEST(cpp_test_archive_get_type_lz4) {
    const char *lz4_path = "test_type.lz4";
    
    // Write LZ4 signature
    fossil_fstream_t stream;
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&stream, lz4_path, "wb"));
    unsigned char lz4_header[] = {0x04, 0x22, 0x4D, 0x18};
    fossil::io::Stream::write(&stream, lz4_header, sizeof(lz4_header), 1);
    fossil::io::Stream::close(&stream);
    
    fossil_io_archive_type_t detected_type = fossil_io_archive_get_type(lz4_path);
    ASSUME_ITS_EQUAL_I32(FOSSIL_IO_ARCHIVE_LZ4, detected_type);
    
    fossil::io::Stream::remove(lz4_path);
}

FOSSIL_TEST(cpp_test_archive_get_type_zstd) {
    const char *zst_path = "test_type.zst";
    
    // Write ZSTD signature
    fossil_fstream_t stream;
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&stream, zst_path, "wb"));
    unsigned char zst_header[] = {0x28, 0xB5, 0x2F, 0xFD};
    fossil::io::Stream::write(&stream, zst_header, sizeof(zst_header), 1);
    fossil::io::Stream::close(&stream);
    
    fossil_io_archive_type_t detected_type = fossil_io_archive_get_type(zst_path);
    ASSUME_ITS_EQUAL_I32(FOSSIL_IO_ARCHIVE_ZSTD, detected_type);
    
    fossil::io::Stream::remove(zst_path);
}

FOSSIL_TEST(cpp_test_archive_get_type_7z) {
    const char *sz_path = "test_type.7z";
    
    // Write 7Z signature
    fossil_fstream_t stream;
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&stream, sz_path, "wb"));
    unsigned char sz_header[] = {'7', 'z', 0xBC, 0xAF, 0x27, 0x1C};
    fossil::io::Stream::write(&stream, sz_header, sizeof(sz_header), 1);
    fossil::io::Stream::close(&stream);
    
    fossil_io_archive_type_t detected_type = fossil_io_archive_get_type(sz_path);
    ASSUME_ITS_EQUAL_I32(FOSSIL_IO_ARCHIVE_7Z, detected_type);
    
    fossil::io::Stream::remove(sz_path);
}

FOSSIL_TEST(cpp_test_archive_get_type_rar) {
    const char *rar_path = "test_type.rar";
    
    // Write RAR signature
    fossil_fstream_t stream;
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&stream, rar_path, "wb"));
    unsigned char rar_header[] = {'R', 'a', 'r', '!', 0x1A, 0x07, 0x00};
    fossil::io::Stream::write(&stream, rar_header, sizeof(rar_header), 1);
    fossil::io::Stream::close(&stream);
    
    fossil_io_archive_type_t detected_type = fossil_io_archive_get_type(rar_path);
    ASSUME_ITS_EQUAL_I32(FOSSIL_IO_ARCHIVE_RAR, detected_type);
    
    fossil::io::Stream::remove(rar_path);
}

FOSSIL_TEST(cpp_test_archive_get_type_rar5) {
    const char *rar5_path = "test_type.rar5";
    
    // Write RAR5 signature
    fossil_fstream_t stream;
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&stream, rar5_path, "wb"));
    unsigned char rar5_header[] = {'R', 'a', 'r', '!', 0x1A, 0x07, 0x01};
    fossil::io::Stream::write(&stream, rar5_header, sizeof(rar5_header), 1);
    fossil::io::Stream::close(&stream);
    
    fossil_io_archive_type_t detected_type = fossil_io_archive_get_type(rar5_path);
    ASSUME_ITS_EQUAL_I32(FOSSIL_IO_ARCHIVE_RAR5, detected_type);
    
    fossil::io::Stream::remove(rar5_path);
}

FOSSIL_TEST(cpp_test_archive_get_type_tar) {
    const char *tar_path = "test_type.tar";
    
    // Write TAR ustar signature at offset 257
    fossil_fstream_t stream;
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&stream, tar_path, "wb"));
    
    // Write 257 bytes of zeros first
    unsigned char zeros[257] = {0};
    fossil::io::Stream::write(&stream, zeros, sizeof(zeros), 1);
    
    // Write ustar signature
    const char *ustar = "ustar";
    fossil::io::Stream::write(&stream, ustar, 5, 1);
    fossil::io::Stream::close(&stream);
    
    fossil_io_archive_type_t detected_type = fossil_io_archive_get_type(tar_path);
    ASSUME_ITS_EQUAL_I32(FOSSIL_IO_ARCHIVE_TAR, detected_type);
    
    fossil::io::Stream::remove(tar_path);
}

FOSSIL_TEST(cpp_test_archive_get_type_targz) {
    const char *targz_path = "test_type.tar.gz";
    
    // Write GZIP signature for tar.gz detection
    fossil_fstream_t stream;
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&stream, targz_path, "wb"));
    unsigned char gz_header[] = {0x1F, 0x8B, 0x08, 0x00};
    fossil::io::Stream::write(&stream, gz_header, sizeof(gz_header), 1);
    fossil::io::Stream::close(&stream);
    
    fossil_io_archive_type_t detected_type = fossil_io_archive_get_type(targz_path);
    ASSUME_ITS_EQUAL_I32(FOSSIL_IO_ARCHIVE_TARGZ, detected_type);
    
    fossil::io::Stream::remove(targz_path);
}

FOSSIL_TEST(cpp_test_archive_get_type_cab) {
    const char *cab_path = "test_type.cab";
    
    // Write CAB signature
    fossil_fstream_t stream;
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&stream, cab_path, "wb"));
    unsigned char cab_header[] = {'M', 'S', 'C', 'F'};
    fossil::io::Stream::write(&stream, cab_header, sizeof(cab_header), 1);
    fossil::io::Stream::close(&stream);
    
    fossil_io_archive_type_t detected_type = fossil_io_archive_get_type(cab_path);
    ASSUME_ITS_EQUAL_I32(FOSSIL_IO_ARCHIVE_CAB, detected_type);
    
    fossil::io::Stream::remove(cab_path);
}

FOSSIL_TEST(cpp_test_archive_get_type_iso) {
    const char *iso_path = "test_type.iso";
    
    // Write ISO 9660 signature at offset 32769
    fossil_fstream_t stream;
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&stream, iso_path, "wb"));
    
    // Write padding to reach offset 32769
    unsigned char zeros[32769] = {0};
    fossil::io::Stream::write(&stream, zeros, sizeof(zeros), 1);
    
    // Write ISO signature
    const char *iso_sig = "CD001";
    fossil::io::Stream::write(&stream, iso_sig, 5, 1);
    fossil::io::Stream::close(&stream);
    
    fossil_io_archive_type_t detected_type = fossil_io_archive_get_type(iso_path);
    ASSUME_ITS_EQUAL_I32(FOSSIL_IO_ARCHIVE_ISO, detected_type);
    
    fossil::io::Stream::remove(iso_path);
}

FOSSIL_TEST(cpp_test_archive_get_type_unknown) {
    const char *unknown_path = "test_type.unknown";
    
    // Write random data that doesn't match any signature
    fossil_fstream_t stream;
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&stream, unknown_path, "wb"));
    unsigned char random_data[] = {0xDE, 0xAD, 0xBE, 0xEF};
    fossil::io::Stream::write(&stream, random_data, sizeof(random_data), 1);
    fossil::io::Stream::close(&stream);
    
    fossil_io_archive_type_t detected_type = fossil_io_archive_get_type(unknown_path);
    ASSUME_ITS_EQUAL_I32(FOSSIL_IO_ARCHIVE_UNKNOWN, detected_type);
    
    fossil::io::Stream::remove(unknown_path);
}

FOSSIL_TEST(cpp_test_archive_get_type_null_path) {
    fossil_io_archive_type_t detected_type = fossil_io_archive_get_type(NULL);
    ASSUME_ITS_EQUAL_I32(FOSSIL_IO_ARCHIVE_UNKNOWN, detected_type);
}

FOSSIL_TEST(cpp_test_archive_get_type_nonexistent) {
    fossil_io_archive_type_t detected_type = fossil_io_archive_get_type("nonexistent_file.xyz");
    ASSUME_ITS_EQUAL_I32(FOSSIL_IO_ARCHIVE_UNKNOWN, detected_type);
}

// Test archive initialization and cleanup
FOSSIL_TEST(cpp_test_archive_open_and_close) {
    const char *archive_path = "test_archive.zip";
    
    // Create a new archive
    fossil_io_archive_t *archive = fossil_io_archive_create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_NOT_CNULL(archive);
    fossil_io_archive_close(archive);
    
    // Open existing archive for reading
    archive = fossil_io_archive_open(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_NOT_CNULL(archive);
    
    fossil_io_archive_close(archive);
    fossil::io::Stream::remove(archive_path);
}

FOSSIL_TEST(cpp_test_archive_create) {
    const char *archive_path = "test_create.tar";
    
    // Create a TAR archive with fast compression
    fossil_io_archive_t *archive = fossil_io_archive_create(archive_path, FOSSIL_IO_ARCHIVE_TAR, FOSSIL_IO_COMPRESSION_FAST);
    ASSUME_NOT_CNULL(archive);
    
    fossil_io_archive_close(archive);
    fossil::io::Stream::remove(archive_path);
}

FOSSIL_TEST(cpp_test_archive_create_compression_levels) {
    const char *archive_path = "test_compression.zip";
    
    // Test different compression levels
    fossil_io_archive_compression_t levels[] = {
        FOSSIL_IO_COMPRESSION_NONE,
        FOSSIL_IO_COMPRESSION_FASTEST,
        FOSSIL_IO_COMPRESSION_FAST,
        FOSSIL_IO_COMPRESSION_NORMAL,
        FOSSIL_IO_COMPRESSION_MAXIMUM
    };
    
    for (size_t i = 0; i < sizeof(levels) / sizeof(levels[0]); i++) {
        fossil_io_archive_t *archive = fossil_io_archive_create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, levels[i]);
        ASSUME_NOT_CNULL(archive);
        fossil_io_archive_close(archive);
        fossil::io::Stream::remove(archive_path);
    }
}

FOSSIL_TEST(cpp_test_archive_create_null_params) {
    fossil_io_archive_t *archive;
    
    // Test null path
    archive = fossil_io_archive_create(NULL, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_ITS_CNULL(archive);
    
    // Test unknown type
    archive = fossil_io_archive_create("test.zip", FOSSIL_IO_ARCHIVE_UNKNOWN, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_ITS_CNULL(archive);
}

// Test archive inspection functions
FOSSIL_TEST(cpp_test_archive_get_stats) {
    const char *archive_path = "test_stats.zip";
    fossil_io_archive_stats_t stats;
    
    // Create archive and add test files
    fossil_io_archive_t *archive = fossil_io_archive_create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_NOT_CNULL(archive);
    
    // Create temporary test files
    const char *test_file1 = "temp_test_file1.txt";
    const char *test_file2 = "temp_test_file2.txt";
    const char *content1 = "Test content 1";
    const char *content2 = "Test content 2 is longer";
    
    fossil_fstream_t temp_stream;
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&temp_stream, test_file1, "w"));
    fossil::io::Stream::write(&temp_stream, content1, strlen(content1), 1);
    fossil::io::Stream::close(&temp_stream);
    
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&temp_stream, test_file2, "w"));
    fossil::io::Stream::write(&temp_stream, content2, strlen(content2), 1);
    fossil::io::Stream::close(&temp_stream);
    
    // Add files to archive
    ASSUME_ITS_TRUE(fossil_io_archive_add_file(archive, test_file1, "test1.txt"));
    ASSUME_ITS_TRUE(fossil_io_archive_add_file(archive, test_file2, "test2.txt"));
    
    // Get stats
    ASSUME_ITS_TRUE(fossil_io_archive_get_stats(archive, &stats));
    ASSUME_ITS_EQUAL_SIZE(2, stats.total_entries);
    ASSUME_ITS_EQUAL_SIZE(strlen(content1) + strlen(content2), stats.total_size);
    ASSUME_ITS_TRUE(stats.compression_ratio >= 0.0 && stats.compression_ratio <= 1.0);
    
    fossil_io_archive_close(archive);
    fossil::io::Stream::remove(test_file1);
    fossil::io::Stream::remove(test_file2);
    fossil::io::Stream::remove(archive_path);
}

FOSSIL_TEST(cpp_test_archive_get_stats_null_params) {
    fossil_io_archive_stats_t stats;
    
    // Test null archive
    ASSUME_ITS_FALSE(fossil_io_archive_get_stats(NULL, &stats));
    
    // Test null stats
    const char *archive_path = "test_stats_null.zip";
    fossil_io_archive_t *archive = fossil_io_archive_create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_NOT_CNULL(archive);
    ASSUME_ITS_FALSE(fossil_io_archive_get_stats(archive, NULL));
    
    fossil_io_archive_close(archive);
    fossil::io::Stream::remove(archive_path);
}

FOSSIL_TEST(cpp_test_archive_list_entries) {
    const char *archive_path = "test_list.zip";
    fossil_io_archive_entry_t *entries = NULL;
    
    // Create archive with test files
    fossil_io_archive_t *archive = fossil_io_archive_create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_NOT_CNULL(archive);
    
    // Create test files
    const char *test_file1 = "temp_test1.txt";
    const char *test_file2 = "temp_test2.txt";
    
    fossil_fstream_t temp_stream;
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&temp_stream, test_file1, "w"));
    fossil::io::Stream::write(&temp_stream, "Content 1", 9, 1);
    fossil::io::Stream::close(&temp_stream);
    
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&temp_stream, test_file2, "w"));
    fossil::io::Stream::write(&temp_stream, "Content 2", 9, 1);
    fossil::io::Stream::close(&temp_stream);
    
    // Add files to archive
    ASSUME_ITS_TRUE(fossil_io_archive_add_file(archive, test_file1, "file1.txt"));
    ASSUME_ITS_TRUE(fossil_io_archive_add_file(archive, test_file2, "file2.txt"));
    
    // List entries
    ssize_t count = fossil_io_archive_list(archive, &entries);
    ASSUME_ITS_EQUAL_I32(2, count);
    ASSUME_NOT_CNULL(entries);
    
    // Verify entries
    bool found_file1 = false, found_file2 = false;
    for (ssize_t i = 0; i < count; i++) {
        if (strcmp(entries[i].name, "file1.txt") == 0) {
            found_file1 = true;
            ASSUME_ITS_EQUAL_SIZE(9, entries[i].size);
        } else if (strcmp(entries[i].name, "file2.txt") == 0) {
            found_file2 = true;
            ASSUME_ITS_EQUAL_SIZE(9, entries[i].size);
        }
    }
    ASSUME_ITS_TRUE(found_file1);
    ASSUME_ITS_TRUE(found_file2);
    
    // Free entries
    fossil_io_archive_free_entries(entries, count);
    fossil_io_archive_close(archive);
    
    // Cleanup
    fossil::io::Stream::remove(test_file1);
    fossil::io::Stream::remove(test_file2);
    fossil::io::Stream::remove(archive_path);
}

FOSSIL_TEST(cpp_test_archive_list_empty) {
    const char *archive_path = "test_list_empty.zip";
    fossil_io_archive_entry_t *entries = NULL;
    
    fossil_io_archive_t *archive = fossil_io_archive_create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_NOT_CNULL(archive);
    
    ssize_t count = fossil_io_archive_list(archive, &entries);
    ASSUME_ITS_EQUAL_I32(0, count);
    ASSUME_ITS_CNULL(entries);
    
    fossil_io_archive_close(archive);
    fossil::io::Stream::remove(archive_path);
}

// Test extraction functions
FOSSIL_TEST(cpp_test_archive_extract_file) {
    const char *archive_path = "test_extract.zip";
    const char *test_file = "temp_source.txt";
    const char *extracted_file = "temp_extracted.txt";
    const char *test_content = "Extract test content";
    
    // Create source file
    fossil_fstream_t temp_stream;
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&temp_stream, test_file, "w"));
    fossil::io::Stream::write(&temp_stream, test_content, strlen(test_content), 1);
    fossil::io::Stream::close(&temp_stream);
    
    // Create archive and add file
    fossil_io_archive_t *archive = fossil_io_archive_create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_NOT_CNULL(archive);
    ASSUME_ITS_TRUE(fossil_io_archive_add_file(archive, test_file, "source.txt"));
    fossil_io_archive_close(archive);
    
    // Reopen for reading and extract
    archive = fossil_io_archive_open(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_NOT_CNULL(archive);
    ASSUME_ITS_TRUE(fossil_io_archive_extract_file(archive, "source.txt", extracted_file));
    fossil_io_archive_close(archive);
    
    // Verify file was extracted
    ASSUME_ITS_TRUE(fossil::io::Stream::file_exists(extracted_file) == FOSSIL_ERROR_OK);
    
    // Cleanup
    fossil::io::Stream::remove(test_file);
    fossil::io::Stream::remove(extracted_file);
    fossil::io::Stream::remove(archive_path);
}

FOSSIL_TEST(cpp_test_archive_extract_all) {
    const char *archive_path = "test_extract_all.zip";
    const char *extract_dir = "temp_extract_dir";
    
    // Create archive with multiple files
    fossil_io_archive_t *archive = fossil_io_archive_create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_NOT_CNULL(archive);
    
    // Create test files
    const char *test_file1 = "temp_all1.txt";
    const char *test_file2 = "temp_all2.txt";
    
    fossil_fstream_t temp_stream;
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&temp_stream, test_file1, "w"));
    fossil::io::Stream::write(&temp_stream, "All test 1", 10, 1);
    fossil::io::Stream::close(&temp_stream);
    
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&temp_stream, test_file2, "w"));
    fossil::io::Stream::write(&temp_stream, "All test 2", 10, 1);
    fossil::io::Stream::close(&temp_stream);
    
    ASSUME_ITS_TRUE(fossil_io_archive_add_file(archive, test_file1, "all1.txt"));
    ASSUME_ITS_TRUE(fossil_io_archive_add_file(archive, test_file2, "all2.txt"));
    fossil_io_archive_close(archive);
    
    // Reopen for reading and extract all
    archive = fossil_io_archive_open(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_NOT_CNULL(archive);
    ASSUME_ITS_TRUE(fossil_io_archive_extract_all(archive, extract_dir));
    fossil_io_archive_close(archive);
    
    // Cleanup
    fossil::io::Stream::remove(test_file1);
    fossil::io::Stream::remove(test_file2);
    fossil::io::Stream::remove(archive_path);
}

// Test modification functions
FOSSIL_TEST(cpp_test_archive_add_file) {
    const char *archive_path = "test_add.zip";
    const char *test_file = "temp_add_file.txt";
    
    // Create test file
    fossil_fstream_t temp_stream;
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&temp_stream, test_file, "w"));
    fossil::io::Stream::write(&temp_stream, "Add file test", 13, 1);
    fossil::io::Stream::close(&temp_stream);
    
    // Create archive and add file
    fossil_io_archive_t *archive = fossil_io_archive_create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_NOT_CNULL(archive);
    ASSUME_ITS_TRUE(fossil_io_archive_add_file(archive, test_file, "added.txt"));
    
    // Verify file exists in archive
    ASSUME_ITS_TRUE(fossil_io_archive_exists(archive, "added.txt"));
    
    fossil_io_archive_close(archive);
    fossil::io::Stream::remove(test_file);
    fossil::io::Stream::remove(archive_path);
}

FOSSIL_TEST(cpp_test_archive_add_directory) {
    const char *archive_path = "test_add_dir.zip";
    const char *test_dir = "temp_test_dir";
    
    // Create test directory and file (simplified - just test the API)
    fossil_io_archive_t *archive = fossil_io_archive_create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_NOT_CNULL(archive);
    
    // Test adding directory (assuming it exists)
    fossil_io_archive_add_directory(archive, test_dir, "testdir");
    
    fossil_io_archive_close(archive);
    fossil::io::Stream::remove(archive_path);
}

FOSSIL_TEST(cpp_test_archive_add_file_null_params) {
    const char *archive_path = "test_add_null.zip";
    const char *test_file = "temp_add_null.txt";
    
    fossil_fstream_t temp_stream;
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&temp_stream, test_file, "w"));
    fossil::io::Stream::write(&temp_stream, "test", 4, 1);
    fossil::io::Stream::close(&temp_stream);
    
    fossil_io_archive_t *archive = fossil_io_archive_create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_NOT_CNULL(archive);
    
    // Test null parameters
    ASSUME_ITS_FALSE(fossil_io_archive_add_file(NULL, test_file, "test.txt"));
    ASSUME_ITS_FALSE(fossil_io_archive_add_file(archive, NULL, "test.txt"));
    ASSUME_ITS_FALSE(fossil_io_archive_add_file(archive, test_file, NULL));
    
    fossil_io_archive_close(archive);
    fossil::io::Stream::remove(test_file);
    fossil::io::Stream::remove(archive_path);
}

FOSSIL_TEST(cpp_test_archive_remove_entry) {
    const char *archive_path = "test_remove.zip";
    const char *test_file = "temp_remove_file.txt";
    
    // Create test file
    fossil_fstream_t temp_stream;
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&temp_stream, test_file, "w"));
    fossil::io::Stream::write(&temp_stream, "Remove test", 11, 1);
    fossil::io::Stream::close(&temp_stream);
    
    // Create archive and add file
    fossil_io_archive_t *archive = fossil_io_archive_create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_NOT_CNULL(archive);
    ASSUME_ITS_TRUE(fossil_io_archive_add_file(archive, test_file, "to_remove.txt"));
    
    // Verify file exists before removal
    ASSUME_ITS_TRUE(fossil_io_archive_exists(archive, "to_remove.txt"));
    
    // Remove the entry
    ASSUME_ITS_TRUE(fossil_io_archive_remove(archive, "to_remove.txt"));
    ASSUME_ITS_FALSE(fossil_io_archive_exists(archive, "to_remove.txt"));
    
    fossil_io_archive_close(archive);
    fossil::io::Stream::remove(test_file);
    fossil::io::Stream::remove(archive_path);
}

// Test utility functions
FOSSIL_TEST(cpp_test_archive_exists) {
    const char *archive_path = "test_exists.zip";
    const char *test_file = "temp_exists_file.txt";
    
    // Create test file and archive
    fossil_fstream_t temp_stream;
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&temp_stream, test_file, "w"));
    fossil::io::Stream::write(&temp_stream, "Exists test", 11, 1);
    fossil::io::Stream::close(&temp_stream);
    
    fossil_io_archive_t *archive = fossil_io_archive_create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_NOT_CNULL(archive);
    ASSUME_ITS_TRUE(fossil_io_archive_add_file(archive, test_file, "exists.txt"));
    
    // Test existence
    ASSUME_ITS_TRUE(fossil_io_archive_exists(archive, "exists.txt"));
    ASSUME_ITS_FALSE(fossil_io_archive_exists(archive, "nonexistent.txt"));
    
    // Test null parameters
    ASSUME_ITS_FALSE(fossil_io_archive_exists(NULL, "exists.txt"));
    ASSUME_ITS_FALSE(fossil_io_archive_exists(archive, NULL));
    
    fossil_io_archive_close(archive);
    fossil::io::Stream::remove(test_file);
    fossil::io::Stream::remove(archive_path);
}

FOSSIL_TEST(cpp_test_archive_entry_size) {
    const char *archive_path = "test_size.zip";
    const char *test_file = "temp_size_file.txt";
    const char *test_content = "Size test content with known length";
    
    // Create test file
    fossil_fstream_t temp_stream;
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&temp_stream, test_file, "w"));
    fossil::io::Stream::write(&temp_stream, test_content, strlen(test_content), 1);
    fossil::io::Stream::close(&temp_stream);
    
    // Create archive and add file
    fossil_io_archive_t *archive = fossil_io_archive_create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_NOT_CNULL(archive);
    ASSUME_ITS_TRUE(fossil_io_archive_add_file(archive, test_file, "size_test.txt"));
    
    // Check entry size
    ssize_t size = fossil_io_archive_entry_size(archive, "size_test.txt");
    ASSUME_ITS_EQUAL_I32(strlen(test_content), size);
    
    // Test non-existent entry
    ssize_t bad_size = fossil_io_archive_entry_size(archive, "nonexistent.txt");
    ASSUME_ITS_EQUAL_I32(-1, bad_size);
    
    // Test null parameters
    ASSUME_ITS_EQUAL_I32(-1, fossil_io_archive_entry_size(NULL, "size_test.txt"));
    ASSUME_ITS_EQUAL_I32(-1, fossil_io_archive_entry_size(archive, NULL));
    
    fossil_io_archive_close(archive);
    fossil::io::Stream::remove(test_file);
    fossil::io::Stream::remove(archive_path);
}

FOSSIL_TEST(cpp_test_archive_print) {
    const char *archive_path = "test_print.zip";
    const char *test_file = "temp_print_file.txt";
    
    // Create test file and archive
    fossil_fstream_t temp_stream;
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&temp_stream, test_file, "w"));
    fossil::io::Stream::write(&temp_stream, "Print test", 10, 1);
    fossil::io::Stream::close(&temp_stream);
    
    fossil_io_archive_t *archive = fossil_io_archive_create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_NOT_CNULL(archive);
    ASSUME_ITS_TRUE(fossil_io_archive_add_file(archive, test_file, "print.txt"));
    
    // Test print function (just ensure it doesn't crash)
    fossil_io_archive_print(archive);
    
    // Test with null archive
    fossil_io_archive_print(NULL);
    
    fossil_io_archive_close(archive);
    fossil::io::Stream::remove(test_file);
    fossil::io::Stream::remove(archive_path);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(cpp_archive_tests) {
    // C++ Archive class functionality tests
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_class_create);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_class_open);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_class_move_constructor);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_class_move_assignment);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_class_get_type);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_class_get_stats);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_class_list);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_class_extract_file);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_class_extract_all);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_class_add_directory);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_class_exists);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_class_entry_size);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_class_remove);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_class_print);

    // Archive type detection tests
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_get_type_zip);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_get_type_gzip);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_get_type_bzip2);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_get_type_xz);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_get_type_lz4);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_get_type_zstd);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_get_type_7z);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_get_type_rar);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_get_type_rar5);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_get_type_tar);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_get_type_targz);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_get_type_cab);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_get_type_iso);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_get_type_unknown);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_get_type_null_path);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_get_type_nonexistent);

    // Archive initialization and cleanup tests
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_open_and_close);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_create);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_create_compression_levels);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_create_null_params);

    // Archive inspection tests
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_get_stats_null_params);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_list_entries);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_list_empty);

    // Archive extraction tests
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_extract_file);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_extract_all);

    // Archive modification tests
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_add_file);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_add_directory);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_add_file_null_params);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_remove_entry);

    // Archive utility tests
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_exists);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_entry_size);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_print);

    // Register the test suite
    FOSSIL_TEST_REGISTER(cpp_archive_suite);
}
