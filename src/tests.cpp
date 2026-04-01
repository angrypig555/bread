// todo finish
#include<iostream>
#include<filesystem>
#include<fstream>
#include "compiler.hpp"

std::filesystem::path test_dir = "/tmp/bread/tests";

int main() {
    std::cout << "Bread automated testing framework" << std::endl;
    find_compiler();
    std::filesystem::create_directories("/tmp/bread/tests");
    std::cout << "Tests will be running in the directory /tmp/bread/tests" << std::endl;
    std::ofstream test1 ("/tmp/bread/tests/test1.bread");
    test1 <<  "print/hello from the bread project\nbol/foo/true\nprint/mathematics test\nint/foo/5\nprint/foo\nadd/foo/5\nprint/foo\nsub/foo/5\nprint/foo\nmul/foo/5\nprint/foo\ndiv/foo/5\nprint/foo\nmod/foo/3\nprint/foo\nprint/while loop test\nint/foo/0\nwhile/foo/less/5\n    print/you should see this 5 times\n    add/foo/1\nendwhile/\nshrtct/printshortcut\n    print/im a shortcut!\nendshrtct/\nprint/waiting 5 seconds\nwait/5\nprint/waited 5 seconds\n";
    test1.close();
    std::filesystem::remove("/tmp/bread/temp.cpp");
    std::cout << "[BUILD] Building to C++" << std::endl;
    compile_to_cpp("/tmp/bread/tests/test1.bread");
    std::cout << "[TEST] Analysing C++ file" << std::endl;

}