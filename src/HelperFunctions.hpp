#pragma once

#include <filesystem>
#include <format>
#include <algorithm>

#define DATA_FLAGS_ENCRYPTED 0b0000'0001

enum class FileType {
  TEXT,
  BINARY
};

void printUsage() {
  std::cout << "Usage: AssetPackCompiler.exe <Path to assets> <Output file>"
            << std::endl;
}

void printVersion() {
  std::cout << "AssetPackCompiler v0.0" << std::endl;
}

uint32_t getFileCountInDirectory(const std::filesystem::path &directory) {
  auto assetsIterator =
      std::filesystem::recursive_directory_iterator(directory);
  uint32_t fileCount =
      std::count_if(begin(assetsIterator), end(assetsIterator),
                    [](const auto &entry) { return entry.is_regular_file(); });
  return fileCount;
}

std::string uint32ToBytes(uint32_t t) {
  std::string bytes;

  bytes.push_back((t >> 24) & 0xFF);
  bytes.push_back((t >> 16) & 0xFF);
  bytes.push_back((t >> 8) & 0xFF);
  bytes.push_back(t & 0xFF);

  return bytes;
}

std::string encrypt(const std::string &input) {
  // choose a power of two => then compiler can replace "modulo x" by much
  // faster "and (x-1)"
  const size_t passwordLength = 16;
  // at least as long as passwordLength, can be longer, too ...
  static const char password[passwordLength] = "invalid pointer";
  // out = in XOR NOT(password)
  std::string result = input;
  for (size_t i = 1; i < input.length(); i++)
    result[i] ^= ~password[i % passwordLength];
  return result;
}

// Returns true if the file is encrypted with XOR
bool writeDataToBinary(const std::filesystem::path &in, std::ofstream &out) {
  FileType type;
  if (in.extension() == ".txt")
    type = FileType::TEXT;
  else
    type = FileType::BINARY;

  std::ifstream inputStream(in);

  switch (type) {
  case FileType::TEXT: {
    std::ostringstream oss;
    oss << inputStream.rdbuf();
    out << encrypt(oss.str());
    return true;
  }
  case FileType::BINARY:
    out << inputStream.rdbuf();
    return false;
  }

  inputStream.close();
}

void replaceAll(std::string &str, char original, char replaceWith) {
  std::replace(str.begin(), str.end(), original, replaceWith);
}