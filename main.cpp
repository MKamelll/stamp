#include<iostream>
#include<filesystem>
#include<vector>
#include<string>
#include<fstream>

using namespace std;
using namespace std::string_literals;
namespace fs = filesystem;

class Directory
{
public:
    Directory(string name) : m_dir_name(name) {
        if (!fs::exists(name)) {
            fs::create_directory(name);
        }
    }

    bool add_file(string name)
    {
        if (!fs::exists(name)) {
            ofstream { name };
            return true;
        }

        return false;
    }

private:
    string m_dir_name;
};

class Cmake
{
public:
    Cmake(string name = "CMakeLists.txt") {
        if (!fs::exists(name)) {
            ofstream out(name);
            m_cmake = move(out);
        }
    }

    void min_version(string num) {
        m_cmake << "cmake_minimum_required" << "(" << "VERSION"
        << " " << num << ")" << "\n";
    }

    void cpp_standard(string num) {
        var("CMAKE_CXX_STANDARD", num);
        var("MAKE_CXX_STANDARD_REQUIRED", "ON");
    }

    void init_project() {
        m_cmake << "get_filename_component(ProjectId ${CMAKE_CURRENT_LIST_DIR} NAME)" << "\n"
        << "string(REPLACE \" \" \"_\" ProjectId ${ProjectId})" << "\n"
        << "project(${ProjectId} VERSION 1.0 LANGUAGES C CXX)" << "\n";

        m_cmake << "set(Src ${CMAKE_CURRENT_LIST_DIR}/src)" << "\n"
        << "add_executable(${ProjectId} ${Src}/main.cpp)" << "\n"
        << "target_include_directories(${ProjectId} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)" << "\n";
    }

private:
    void var(string name, string value) {
        m_cmake << "set" << "(" << name << " " << value << ")" << "\n";
    }
    
    ofstream m_cmake;
};

class ShScript
{
public:
    ShScript(string name = "build_and_run.sh") : m_script_name(name) {
        if (!fs::exists(name)) {
            ofstream shell { name };
            m_shell = move(shell);
        }
    }

    void set_debug_and_output_commands() {
        m_shell << "set" << " " << "-xe" << "\n";
    }

    void append_make() {
        m_shell << "make" << "\n";
    }

    void append_cmake() {
        change_dir("build");
        m_shell << "cmake" << " " << ".." << "\n";
    }

    void give_execution_permission() {
        fs::permissions(m_script_name, fs::perms::owner_exec | fs::perms::group_exec, fs::perm_options::add);
    }

    void append_run_main() {
        change_dir("..");
        m_shell << "./build/\"${PWD##*/}\"" << "\n";
    }

private:
    void change_dir(string name) {
        m_shell << "cd" << " " << name << "\n";
    }
    
    string m_script_name;
    ofstream m_shell;
};

class CppHelloFile
{
public:
    CppHelloFile(string name = "main") {
        ofstream hf {"src/"s + name + ".cpp"s};
        hf << "#include<iostream>" << "\n"
        << "\n"
        << "using namespace std" << ";"
        << "int main()" << "\n"
        << "{" << "\n"
        << "cout" << "<<" << "\"Hello, World!\"" << "<<"  << " " << "endl;" << "\n"
        << "return 0;" << "\n"
        << "}";
    }
};

int main()
{
    vector<string> names {"include", "src", "build"};
    vector<unique_ptr<Directory>> dirs;
    for (auto& name : names) {
        dirs.push_back(make_unique<Directory>(name));
    }

    auto cmake = make_unique<Cmake>();
    cmake->min_version("3.10");
    cmake->cpp_standard("17");
    cmake->init_project();

    auto script = make_unique<ShScript>();
    script->set_debug_and_output_commands();
    script->append_cmake();
    script->append_make();
    script->give_execution_permission();
    script->append_run_main();
    
    make_unique<CppHelloFile>();
    
    return 0;
}