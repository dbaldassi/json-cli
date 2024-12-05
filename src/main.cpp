#include <fmt/core.h>
#include <CLI/CLI.hpp>
#include <fstream>
#include <sstream>

#include "nlohmann/json.hpp"

using json = nlohmann::json;

std::string read_file(std::istream& ifs)
{
    constexpr auto size = 1024;

    std::string out;
    std::string buf(size, 0);

    while(ifs.read(&buf[0], size)) {
        out.append(buf, 0, ifs.gcount());
    }

    out.append(buf, 0, ifs.gcount());

    return out;
}

int main(int argc, char* argv[])
{
    CLI::App app;

    std::string file, field, out_file_name;
    bool dump = false, parse = false;

    // Setup cli
    auto file_opt = app.add_option("file", file, "Input file to read JSON from");
    app.add_flag("-p,--parse", parse, "Parse the input JSON dump and print it");
    app.add_flag("-d,--dump", dump, "Dump a JSON input");
    app.add_flag("-o,--out", out_file_name, "Redirect output to this file");
    auto field_opt = app.add_option("-g,--get", field, "Print this field's value");

    // PARSE command line args
    CLI11_PARSE(app, argc, argv);

    std::string input;

    // Load file
    if(*file_opt) {
        std::ifstream ifs(file);
        input = read_file(ifs);
    }
    else { // Check for standard input
        // Peek to see if there is any input
        if(std::cin.peek() != EOF) {
            input = read_file(std::cin);
        }
    }

    // Parse input
    json input_json = json::parse(input);

    if(*field_opt) fmt::print("{}\n", input_json[field].dump()); // Print requested field
    else if(dump)  fmt::print("{}\n", input_json.dump()); // print one liner
    else if(parse) std::cout << input_json.dump(4) << std::endl; // Print pretty json
    else fmt::print(stderr, "No action to be done, specify --parse or --dump");

    return 0;
}