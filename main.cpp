#include "generator.hpp"

#include <CLI/CLI.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <experimental/filesystem>


using namespace dragon;
namespace fs = std::experimental::filesystem;

int main(int argc, char **argv) {
    std::string APP_VERSION = "0.01";
    std::string headerFile = fs::current_path().parent_path() / "test" /"include"/ "header.h";
    std::string outputFile = "./test/OutputFile.ll";

    CLI::App app("This app generates llvm IR code for two functions: square(x) and sumsquares(x, y).\n");
    app.set_version_flag("--version",
                         APP_VERSION + "; using cli11 version "
                         + std::string(CLI11_VERSION) + ".");

    CLI11_PARSE(app, argc, argv);

    std::string headerStr =
    R"(
// This is auto-generated
#ifndef GEN_HEADER_H
#define GEN_HEADER_H
extern "C" {
 int square(int);
 int sumsquares(int, int);
 int fivefactorial();
 int fivefactorialnofold();
}
#endif // GEN_HEADER_H
    )";

    IRGenerator irGenerator;
//    auto outString = irGenerator.GetSquareFuncIR();
    auto outString = irGenerator.GetMainModuleIR();
    std::ofstream outputStream;
    outputStream.open(outputFile);
    if (outputStream && !outString.empty()) {
        outputStream << outString;
        outputStream.flush();
        std::cout << "The Module's IR written into: " << outputFile << "\n";

        outputStream.close();
        outputStream.close();
        outputStream.open(headerFile);
        outputStream << headerStr;
        outputStream.flush();
        std::cout << "The Header file is written into: " <<  headerFile<< "\n";

    } else std::cout << "No IR generated OR Failed to open the file " << outputFile << " for write\n";

    return 0;
}