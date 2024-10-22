#include <iostream>
#include <filesystem>
#include <fstream>
#include <cstdlib>

namespace fs = std::filesystem;

using std::cout, std::endl, std::cerr, std::string, std::ofstream;

/*
    cgen new [flags] [project name]

        cgen new -c [project name] -> Generates new C project
        cgen new -cpp [project name] -> Generates new C++ project

    cgen --help
    cgen --version
*/

const string VER = "0.1";

int main(int argc, char** argv) {

    if (argc == 2) {
        string flag = argv[1];
        cout << "flag: " << flag << endl;

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

             if(fs::exists(directory)) {
                cerr << "Directory already exists \n" << endl;
                return 1;
            }

            cout << "\nNew projected created under the name of: \n" 
                 << "   " <<  project_name << "\n\n";

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

            string command = "git init " + project_name;

            int result = system(command.c_str());

            if (result == 0) {
                cout << "Git repository initialized successfully in " << directory << endl;
            } else {
                cerr << "Failed to initialize git repository in " << directory << endl;
            }

        } else {

        }

    } else {
        cerr << "Invalid number of arguments" << endl;
        return 1;
    }
}