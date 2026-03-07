#include<iostream>
#include<fstream>
#include<filesystem>
#include<vector>
#include<string>
#include<algorithm>

std::string filename;
std::filesystem::path tmp_dir = "/tmp/bread/";
std::filesystem::path cpp_dir = "/tmp/bread/temp.cpp";
std::string cppname;
std::vector<std::string> file_contents;
std::vector<std::string> integer_names;
std::vector<std::string> string_names;
std::vector<std::string> bool_names;


int compiler; // 1 = gcc, 2 = clang, if both, clang will be preferred due to faster compile time

int create_tmp_dir_if_not_exist() {
    try {
        if (!std::filesystem::is_directory(tmp_dir)) {
            std::filesystem::create_directory(tmp_dir);
            std::cout << "created tmp directory, first start?" << std::endl;
        }
    } catch (const std::filesystem::filesystem_error &e) {
        std::cout << "could not create tmp directory, " << e.what() << std::endl;
        throw 500;
        return 1;
    }
    return 0;
}

int compile_cpp(std::string filepath) {
    size_t prefix = filename.find(".bread");
    std::string new_filename = filename.substr(0, prefix);
    std::string clang = "clang++ " + filepath + " -o " + new_filename;
    std::string gcc = "g++ " + filepath + " -o " + new_filename;
    std::string copy_path = "cp " + std::string(tmp_dir) + new_filename + " .";
    std::cout << "beginning build..." << std::endl;
    if (compiler == 1) {
        std::system(gcc.c_str());
    } else if (compiler == 2) {
        std::system(clang.c_str());
    }
    std::cout << "build complete" << std::endl;
    return 0;
}

void trimwhtspc(std::string &string) {
    string.erase(string.begin(), std::find_if(string.begin(), string.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    string.erase(std::find_if(string.rbegin(), string.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), string.end());
}

int compile_to_cpp(std::string filename) {
    std::ifstream breadfile;
    std::ofstream cpp_file;

    breadfile.open(filename);
    if (!breadfile.is_open()) {
        std::cerr << "error opening file" << std::endl;
        throw 500;
        return 1;
    }
    std::string c;
    while (std::getline(breadfile, c)) {
        file_contents.push_back(c);
    } // save file into vector
    breadfile.close();
    cpp_file.open(cpp_dir);
    if (!cpp_file.is_open()) {
        std::cerr << "error creating temporary cpp file" << std::endl;
        throw 500;
        return 1;
    }
    //for (std::string content : file_contents) {
    //    if (content.find("shrtct/")) {
    //        content.erase(0,7);
    //    }
    //} todo: shortcuts
    cpp_file << "#include<iostream>\nint main() {\n";
    for (std::string content : file_contents) {
        trimwhtspc(content); // needed if the user uses tabs
        if (content.find("print/") == 0) {
            content.erase(0, 6);
            auto string_it = std::find(string_names.begin(), string_names.end(), content);
            auto int_it = std::find(integer_names.begin(), integer_names.end(), content);
            auto bool_it = std::find(bool_names.begin(), bool_names.end(), content);
            if (string_it != string_names.end() or int_it != integer_names.end() or bool_it != bool_names.end()) {
                cpp_file << "std::cout << " + content << " << std::endl;\n";
            } else {
                cpp_file << "std::cout << \"" << content << "\" << std::endl;\n";
            }
            
        } else if (content.find("int/") == 0 ) {
            content.erase(0, 4);
            size_t slash = content.find('/');
            if (slash != std::string::npos) {
                std::string int_name = content.substr(0, slash);
                std::string int_val = content.substr(slash + 1);
                int is_int = all_of(int_val.begin(), int_val.end(), ::isdigit);
                if (!is_int) {
                    std::cerr << "an integer can only be a number!" << std::endl;
                    throw 500;
                    return 1;
                }
                auto int_it = std::find(integer_names.begin(), integer_names.end(), content);
                if (int_it != integer_names.end()) {
                    cpp_file << int_name << " = " << int_val << ";\n"; 
                } else {
                    cpp_file << "int " << int_name << " = " << int_val << ";\n";
                    integer_names.push_back(int_name);
                }
            }
            
        } else if (content.find("bol/") == 0) {
            content.erase(0, 4);
            size_t slash = content.find('/');
            if (slash != std::string::npos) {
                std::string bol_name = content.substr(0, slash);
                std::string bol_val = content.substr(slash + 1);
                if (bol_val != "true" and bol_val != "false") {
                    std::cerr << "booleans can only be true or false!\nexpected: true / false\ngot: " << bol_val << "\nfor " << bol_name << std::endl;
                    throw 500;
                    return 1;
                }
                auto bol_it = std::find(bool_names.begin(), bool_names.end(), content);
                if (bol_it != bool_names.end()) {
                    cpp_file << bol_name << " = " << bol_val << ";\n";
                } else {
                    cpp_file << "bool " << bol_name << " = " << bol_val << ";\n";
                    bool_names.push_back(bol_name);
                }
            }
        } else if (content.find("str/") == 0) {
            content.erase(0, 4);
            size_t slash = content.find('/');
            if (slash != std::string::npos) {
                std::string str_name = content.substr(0, slash);
                std::string str_val = content.substr(slash + 1);
                auto str_it = std::find(string_names.begin(), string_names.end(), content);
                if (str_it != string_names.end()) {
                    cpp_file << str_name << " = \"" << str_val << "\";\n";
                } else {
                    cpp_file << "std::string " << str_name << " = \"" << str_val << "\";\n";
                    string_names.push_back(str_name);
                }
            }
        } else if (content.find("in/") == 0) {
            content.erase(0, 3);
            auto it = std::find(string_names.begin(), string_names.end(), content);
            if (it == string_names.end()) {
                cpp_file << "std::string " << content << ";\n";
                string_names.push_back(content);
            }
            cpp_file << "std::cin >> " << content << ";\n";
        } else if (content.find("exit/") == 0) {
            content.erase(0, 5);
            int is_int = all_of(content.begin(), content.end(), ::isdigit);
            if (!is_int) {
                    std::cerr << "an exit code can only be a number!" << std::endl;
                    throw 500;
                    return 1;
            }
            cpp_file << "return " << content << ";\n";
        } else if (content.find("if/") == 0) {
            content.erase(0, 3);
            size_t slash1 = content.find('/');
            size_t slash2 = content.find('/', slash1 + 1);
            std::string variable1 = content.substr(0, slash1);
            std::string condition = content.substr(slash1 + 1, slash2 - slash1 - 1);
            std::string variable2 = content.substr(slash2 + 1);
            if (condition != "equals" && condition != "notequals") {
                std::cerr << "an if condition can only take equals or notequals as operators!" << std::endl;
                throw 500;
                return 1;
            }
            if (condition == "equals") {
                condition = "==";
            } else if (condition == "notequals") {
                condition = "!=";
            }
            cpp_file << "if (" << variable1 << " "<< condition << " " << variable2 << ") {\n";
        } else if (content.find("endif/") == 0) {
            cpp_file << "}\n";
        }
    }
    cpp_file << "return 0;\n}\n";
    cpp_file.close();
    compile_cpp(cpp_dir);
    return 0;
}

// checks for compilers on the machine
int find_compiler() {
    std::cout << "scanning for compilers..." << std::endl;
    std::string gcc = "gcc -v > /dev/null 2>&1";
    std::string clang = "clang++ -v > /dev/null 2>&1";
    int gcc_result = std::system(gcc.c_str());
    int clang_result = std::system(clang.c_str());
    if (clang_result == 0 && gcc_result == 0) {
        std::cout << "2 compilers found, clang will be preferred" << std::endl;
        compiler = 2;
    } else if (gcc_result == 0) {
        std::cout << "gcc found" << std::endl;
        compiler = 1;
    } else if (clang_result == 0) {
        std::cout << "clang found" << std::endl;
        compiler = 2;
    } else {
        std::cerr << "no compilers found, please install gcc or clang" << std::endl;
        throw 500;
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "no arguments given, enter filename to compile .bread file" << std::endl;
        return 1;
    } else {
        filename = argv[1];
    }
    try {
        find_compiler();
    } catch (int e) {
        return 1;
    }
    try {
        create_tmp_dir_if_not_exist();
    } catch (int e) {
        return 1;
    }
    try {
        compile_to_cpp(filename);
    } catch (int ercode) {
        return 1;
    }
    return 0;
}