// the bread compiler -- compiler.hpp
// licensed under the MIT license, see LICENSE for more information
#pragma once
#include<string>
#include<vector>
#include<filesystem>

extern std::string filename;
extern std::filesystem::path tmp_dir;
extern std::filesystem::path cpp_dir;
extern std::string cppname;
extern std::vector<std::string> file_contents;
extern std::vector<std::string> integer_names;
extern std::vector<std::string> string_names;
extern std::vector<std::string> bool_names;
extern std::vector<std::string> shortcut_names;
extern int compiler;

extern int line_count;

int create_tmp_dir_if_not_exist();
int compile_cpp(std::string filepath);
void trimwhtspc(std::string &string);
int compile_to_cpp(std::string filename);
int find_compiler();
