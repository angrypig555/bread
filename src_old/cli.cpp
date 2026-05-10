#include<iostream>
#include "compiler.hpp"

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