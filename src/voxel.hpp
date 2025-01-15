#ifndef VOXEL_HPP_
#define VOXEL_HPP_
#include <cnpy.h>
#include <vector>
#include <sstream>
#include <tuple>
#include <array>
#include <iomanip>
#include <map>

namespace voxel_visualize {

    class Voxel {
    private:
        double mScale = 1;
        const size_t N, N2;
        std::vector<bool> mOccupy;
        std::vector<std::tuple<int, int, int>> mActiveVoxels;
        [[nodiscard]] size_t getIndex(int x, int y, int z) const;
        static bool checkNpyArray(const cnpy::NpyArray& array);
        [[nodiscard]] bool checkRange(int coord) const;

    public:
        Voxel(const size_t n, cnpy::NpyArray voxel);
        [[nodiscard]] std::string toObj() const;
    };

} // end namespace voxel_visualize
#endif