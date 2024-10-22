#include <iostream>
#include <filesystem>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <fmt/core.h>
#include <fmt/color.h>

namespace fs = std::filesystem;

using std::cout, std::endl, std::cerr, std::string, std::ofstream, fmt::color,
        fmt::emphasis, fmt::print;

const string VER = "0.1";

int main(int argc, char** argv) {

    if (argc == 2) {
        string flag = argv[1];

        if (flag == "-h" || flag == "--help") {
            print(fg(fmt::color::sky_blue) | fmt::emphasis::bold,
                "\n    Usage\n");
            cout << "     " << endl;
            cout << "    Generate new project: " << endl;
            print(emphasis::bold,
            "        cgen new [-c | -cpp] [project name]\n");
            cout << "     " << endl;
            cout << "    Display this page: " << endl;
            print(emphasis::bold,
            "        cgen [-h | --help]\n");
            cout << "     " << endl;
            cout << "    Get version: " << endl;
            print(emphasis::bold,
            "        cgen [-v | --version]\n");
            cout << "     " << endl;

        } else if (flag == "-v" || flag == "--version") {
            print(fg(fmt::color::sky_blue) | fmt::emphasis::bold,
                "\n    Version ");
            print(fmt::emphasis::bold,
                VER + "\n");
        } else {
            print(fg(fmt::color::crimson) | fmt::emphasis::bold,
                "\n    [Error]\n");
                print(emphasis::bold, "    Unrecognized flag\n");
            return 1;
        }

    } else if (argc == 4) {
        string command = argv[1];
        string flag = argv[2];
        string project_name = argv[3];

        string proj_type_ext;
        string cxx;
        string cxxflags;

        string include;
        string using_line;
        string print_line;

        if (command == "new") {

            if (flag == "-c") {
                proj_type_ext = ".c";
                cxx = "CXX      = clang\n";
                cxxflags = "CXXFLAGS = -std=c2x -Wall\n";

                include = "#include <stdio.h>";
                using_line = "\n";
                print_line = "\tprintf(\"Hello World!\\n\");\n";
            } else if (flag == "-cpp") {
                proj_type_ext = ".cpp";
                cxx = "CXX      = clang++\n";
                cxxflags = "CXXFLAGS = -std=c++2b -Wall\n";

                include = "#include <iostream>\n";
                using_line = "using std::cout, std::endl;\n";
                print_line = "    cout << \"Hello World!\" << endl;\n";
            } else {
                cerr << "Invalid arguments." << endl;
            }

            fs::path directory = project_name;
            fs::path src = directory / "src";
            fs::path bin = directory / "bin";

            fs::path file = src / ("main" + proj_type_ext);
            fs::path make = directory / "Makefile";
            fs::path read = directory / "README.md";

            fs::path cwd = fs::current_path() / directory;
            string c = cwd.c_str();

            if(fs::exists(directory)) {
                print(fg(fmt::color::crimson) | fmt::emphasis::bold,
                "\n    [Error]\n");
                print(emphasis::bold, "    Directory already exists\n");
                return 0;
            }

            fs::create_directories(project_name);
            fs::create_directories(src);
            fs::create_directories(bin);

            ofstream codeFile(file);
            codeFile << include
                    << using_line
                    << '\n'
                    << "int main(int argc, char** argv) {\n"
                    << print_line
                    << '\n'
                    << "    return 0;\n"
                    << "}";

            codeFile.close();

            ofstream makeFile(make);
            makeFile << cxx
                    << cxxflags
                    << "SRC      = ./src/" << ("main" + proj_type_ext) << "\n"
                    << "OUT      = ./bin/out\n"
                    << "\n"
                    << "all: $(OUT)\n"
                    << "\n"
                    << "$(OUT): $(SRC)\n"
                    << "\t$(CXX) $(CXXFLAGS) -o $(OUT) $(SRC)\n"
                    << "\n"
                    << "clean:\n"
                    << "\trm -f $(OUT)\n"
                    << "\n"
                    << ".PHONY: all clean\n";
            makeFile.close();

            string author =  std::getenv("USER");;
            string date; 
            
            std::time_t currentTime = std::time(nullptr);
            std::tm* localTime = std::localtime(&currentTime);

            int day = localTime->tm_mday;
            int month = (localTime->tm_mon + 1);
            int year = (localTime->tm_year + 1900);

            int hour = localTime->tm_hour;
            int min = localTime->tm_min;

            ofstream readmeFile(read);
            readmeFile << "# " << project_name << "\n";
            readmeFile << "\n";
            readmeFile << "Created by " << author << 
                        " at " << hour << ":" << min <<
                        " on " << day << "/" << month << "/" << year << "\n";
            readmeFile.close();

            string command = "git init -q " + project_name;

            int result = system(command.c_str());

            if (result != 0) {
                print(fg(fmt::color::crimson) | fmt::emphasis::bold,
                "\n    [Error]\n");
                print(emphasis::bold, "    Failed to initialize git\n");
                return 0;
            }

            print(fg(fmt::color::green) | fmt::emphasis::bold,
                "\nSuccessfully");
            print(" created new project in the directory of: \n");
            print(fmt::emphasis::bold, "\t" + c + "\n\n");
        } else {
            print(fg(fmt::color::crimson) | fmt::emphasis::bold,
                "\n    [Error]\n");
            print(emphasis::bold, "    Unrecognized command\n");
        }

    } else {
        print(fg(fmt::color::crimson) | fmt::emphasis::bold,
                "\n    [Error]\n");
                print(emphasis::bold, "    Invalid number of arguments\n");
        return 1;
    }
}