#pragma once

#include <filesystem>
#include <format>
#include <algorithm>
#include <fstream>

#define DATA_FLAGS_ENCRYPTED 0b0000'0001
#define FILE_HEADER "OCAPv0.0"

enum class FileType {
  TEXT,
  BINARY
};

uint32_t getFileCountInDirectory(const std::filesystem::path &directory);

std::string uint32ToBytes(uint32_t t);

std::string encrypt(const std::string &input);

// Returns true if the file is encrypted with XOR
bool writeDataToBinary(const std::filesystem::path &in, std::ofstream &out);

void replaceAll(std::string &str, char original, char replaceWith);