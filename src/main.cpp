#include <iostream>
#include <filesystem>
#include <cnpy.h>
#include <fstream>
#include "voxel.hpp"
namespace fs = std::filesystem;
namespace vv = voxel_visualize;

int main(int argc, char* argv[]){
    if(argc != 3){
        std::cerr << "usage: " << argv[0] << " ${input_voxel_npy} ${output_obj}\n";
        return 1;
    }
    fs::path voxel_npy = argv[1];
    fs::path output_obj = argv[2];
    if(!fs::is_regular_file(voxel_npy)){
        std::cerr << "path '" << voxel_npy << "' is not regular file\n";
        return 1;
    }
    auto voxel = vv::Voxel(64, cnpy::npy_load(voxel_npy));
    std::ofstream f(output_obj, std::ios::out);
    if(!f.good()){
        std::cerr << "";
        return 1;
    }
    f << voxel.toObj();
    return 0;
}