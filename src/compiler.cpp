#include<iostream>

std::string filename;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "no arguments given, enter filename to compile .bread file" << std::endl;
        return 1;
    } else {
        filename = argv[1];
    }
    return 0;
}