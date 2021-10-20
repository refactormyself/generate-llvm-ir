#include <CLI/CLI.hpp>
#include <iostream>
#include <string>

int main(int argc, char **argv) {
    std::string APP_VERSION = "0.00.0";

    // describe the app
    CLI::App app("This is a basic command line app. Nothing fancy."
                 "\nIt uses the cli11 library: https://cliutils.github.io/CLI11/");
    // version query
    app.set_version_flag("--version",
                         APP_VERSION + "; using cli11 version "
                         + std::string(CLI11_VERSION) + ".");

    // input file (positional)
    std::string inputFile;
    CLI::Option *inputOpt = app.add_option("-i,--in,file",
                                           inputFile, "Input File name");

    // output file
    std::string outputFile;
    CLI::Option *outputOpt = app.add_option("-o,--output",
                                            outputFile, "Output File name");

    // pass in numerical values
    int valueInt{0};
    CLI::Option *optInt = app.add_option("-z,--integer", valueInt, "Integer value");

    double valueDouble{0.0};  // = 3.14;
    app.add_option("-d,--double", valueDouble, "Floating Point Value");

    // general flags
    int flagValue{0};
    CLI::Option *flag = app.add_flag("--flag",
                                     flagValue, "Some flag that can be passed multiple times");

    CLI11_PARSE(app, argc, argv);

    if (!inputOpt->empty())
        std::cout << "I received the file: " << inputFile
                  << " as an input file. \nNum of times it was passed in: direct count: "
                  << app.count("--in")
                  << ", opt count: " << inputOpt->count() << std::endl;
    else std::cout << "No input file supplied! Option count: " << inputOpt->count() << std::endl;

    if (!outputOpt->empty())
        std::cout << "I received the file: " << outputFile
                  << " as an output file. \nNum of times it was passed in: direct count: "
                  << app.count("--output")
                  << ", opt count: " << outputOpt->count() << std::endl;
    else std::cout << "No output file supplied! Option count: " << outputOpt->count() << std::endl;

    std::cout << "Integer value: " << valueInt << std::endl;
    std::cout << "Floating Point value: " << valueDouble << std::endl;
    std::cout << "Received flag: " << flagValue << " (" << flag->count() << ") times\n";

    return 0;
}