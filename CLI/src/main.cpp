#include <ocap.hpp>

#include "Helper.hpp"

#include <string>
#include <iostream>

void printUsage() {
  std::cout
      << "Usage: ocapcli.exe compile <Path to assets> <Output file> "
         "[--no-output]\n"
      << "           Compiles assets from <Path to assets> into <Output file> "
         "in ocap format.\n\n"
      << "       ocapcli.exe read <OCAP file> <File to read> [Output file]\n"
      << "           Reads the specified asset <File to read> from <OCAP file> "
         "to stdout (recommended for text assets)\n"
      << "           Or if [Output file] is specified, reads it to [Output "
         "file] instead (recommended for binary assets)\n\n"
      << "       ocapcli.exe usage\n"
      << "           Writes usage.\n\n"
      << "       ocapcli.exe version\n"
      << "           Outputs the version of the compiler and the version of "
         "the library."
      << std::endl;
}

void printVersion() {
  std::cout << "OptiCraft Asset Pack CLI v0.0" << std::endl;
  std::cout << ocap::getVersionString() << std::endl;
}

void compile(const std::string &assetsPath, const std::string &outputFile,
             bool noOutput) {
  if (noOutput) {
    ocap::compile(assetsPath, outputFile);
    return;
  }
  registerTopLeft();
  ocap::compile(
      assetsPath, outputFile,
      [](const std::string &filename, const uint32_t fileIndex,
         const uint32_t fileCount) {
        clearConsole();
        std::cout << "Adding " << filename << std::endl
                  << "  " << fileIndex + 1 << "/" << fileCount << " files";
      },
      [](const std::string &filename) {
        /*std::cout << "\33[2K\033[A\33[2K\r"
                  << "Added " << filename << std::endl;*/
      },
      []() {
        clearConsole();
        std::cout << "Writing data...";
      });

  clearConsole();
  std::cout << "Done!" << std::endl
            << "Outputted to " << outputFile << std::endl;
}

int main(int argc, char **argv) {
#ifndef DEBUG
  if (argc < 4 && argc != 2) {
    std::cerr << "Too few arguments! Use 'ocapcli.exe usage' to see usage."
              << std::endl;
    return 1;
  }

  if (strcmp(argv[1], "usage") == 0)
    printUsage();
  else if (strcmp(argv[1], "version") == 0)
    printVersion();
  else if (strcmp(argv[1], "compile") == 0) {
    bool noOutput = false;
    if (argc > 4)
      if (strcmp(argv[4], "--no-output") == 0) noOutput = true;

    compile(argv[2], argv[3], noOutput);
  } else if (strcmp(argv[1], "read") == 0)
    return -1;
  else {
    std::cerr << "Unknown command " << argv[1]
              << ". Use 'ocapcli.exe usage' to see usage";
    return 2;
  }

#endif // !DEBUG

#ifdef DEBUG
  compile("Assets", "out.ocap", true);
#endif // DEBUG

  return 0;
}
