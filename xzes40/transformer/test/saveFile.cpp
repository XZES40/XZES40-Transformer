#include <fstream>
#include <iostream>
#include <filesystem>
#include <string>
namespace fs = std::filesystem;

int main(int argc, char*argv[]) {
{
    //fs::create_directory("sandbox");
    //std::ofstream("sandbox/file1.txt").put('a');
    string src, det;
    src = argv[0];
    det = argv[1];
 
    fs::copy_file(src, det);
 
    return 0;
}
