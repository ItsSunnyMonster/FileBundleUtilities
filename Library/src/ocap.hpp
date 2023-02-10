#pragma once

#include <string>

namespace ocap {
// This will probably not be called from OptiCraft
void compile(
    std::string assetsDirectory, const std::string &outputFile,
    void (*onAdd)(const std::string &filename, uint32_t fileIndex,
                  uint32_t fileCount) = [](const std::string &, uint32_t,
                                           uint32_t) {},
    void (*onAdded)(const std::string &) = [](const std::string &) {},
    void (*onWriteData)() = []() {});
std::string getVersionString();
} // namespace ocap