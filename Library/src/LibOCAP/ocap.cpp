#include "ocap.hpp"

#include <fstream>
#include <iostream>

#include "HelperFunctions.hpp"

void ocap::compile(std::string assetsPath, const std::string &outputFile,
                   void (*onAdd)(const std::string &filename,
                                 uint32_t fileIndex, uint32_t fileCount),
                   void (*onAdded)(const std::string &),
                   void (*onWriteData)()) {
  // Remove trailing slash
  if (assetsPath[assetsPath.length() - 1] == '/' ||
      assetsPath[assetsPath.length() - 1] == '\\')
    assetsPath.pop_back();

  // Keep track of where the assets data begins
  uint32_t dataBegin = 0;

  // Open the output file
  std::ofstream outputFileStream(outputFile, std::ios::binary);

  // Write the file header
  outputFileStream.write(FILE_HEADER, strlen(FILE_HEADER));
  outputFileStream.put(0x00);
  // Move the "pointer" to the start of assets data by the size of the header
  dataBegin += strlen(FILE_HEADER) + 1;

  // Open the assets directory
  std::filesystem::path assetsDir(assetsPath);

  // Create a temporary file to write the data to
  std::ofstream tempDataFileOutStream(outputFile + ".tmp", std::ios::binary);

  // Get the number of files in the assets directory
  uint32_t fileCount = getFileCountInDirectory(assetsDir);

  // Loop through the directory
  uint32_t fileIndex = 0;
  for (auto const &entry :
       std::filesystem::recursive_directory_iterator(assetsDir)) {

    // If the current entry is not a file (aka if it is a directory)
    if (!entry.is_regular_file()) continue;

    // Get the path relative to the assets directory
    std::string pathString = entry.path().string();
    std::string relativePath = pathString.substr(assetsPath.length() + 1);
    replaceAll(relativePath, '\\', '/');

    onAdd(relativePath, fileIndex, fileCount);

    outputFileStream.write(encrypt(relativePath).c_str(),
                           relativePath.length());
    outputFileStream.put(0x00);
    dataBegin += relativePath.length() + 1;

    // Write the location which points to the start of the data
    uint32_t location = tempDataFileOutStream.tellp();
    outputFileStream.write(uint32ToBytes(location).c_str(), sizeof(uint32_t));
    dataBegin += sizeof(uint32_t);

    // Write the data to the temp file and get the start and size of it
    // Then write it to the header indices
    uint32_t before = tempDataFileOutStream.tellp();
    bool encrypted = writeDataToBinary(entry.path(), tempDataFileOutStream);
    uint32_t after = tempDataFileOutStream.tellp();
    uint32_t difference = after - before;
    outputFileStream.write(uint32ToBytes(difference).c_str(), sizeof(uint32_t));
    dataBegin += sizeof(uint32_t);

    // Flags (if data is encrypted etc.)
    uint8_t flags = 0;
    if (encrypted) flags |= DATA_FLAGS_ENCRYPTED;
    outputFileStream.put(flags);
    dataBegin += 1;

    // std::this_thread::sleep_for(std::chrono::seconds(2));

    // Clear the two lines then print added
    onAdded(relativePath);

    fileIndex++;
  }

  dataBegin += sizeof(uint32_t);
  outputFileStream.write(uint32ToBytes(dataBegin).c_str(), sizeof(uint32_t));

  tempDataFileOutStream.close();

  // Open the temp file and write it to the output file
  onWriteData();
  std::ifstream tempDataFileInStream(outputFile + ".tmp", std::ios::binary);
  outputFileStream << tempDataFileInStream.rdbuf();
  tempDataFileInStream.close();

  // Close the output file
  outputFileStream.close();

  remove((outputFile + ".tmp").c_str());
}

std::string ocap::getVersionString() {
  return "Asset Pack Library v0.0";
}
