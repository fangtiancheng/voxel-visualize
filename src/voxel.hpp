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
        const size_t N, N2, N3;
        std::vector<bool> mOccupy;
        std::vector<std::tuple<int, int, int>> mActiveVoxels;
        size_t getIndex(int x, int y, int z) const {
            return x + N * y + N * N * z;
        }
        static bool checkNpyArray(const cnpy::NpyArray& array) {
            if (array.shape.size() != 2) return false;
            if (array.shape[1] != 3) return false;
            if (array.word_size != 4) return false;
            return true;
        }
        bool checkRange(int coord) const {
            return coord >= 0 && coord <= N;
        }

    public:
        Voxel(const size_t n, cnpy::NpyArray voxel)
                : N(n), N2(n * n), N3(n * n * n), mOccupy(n * n * n, false) {
            if (!checkNpyArray(voxel)) {
                throw std::runtime_error("voxel format error, expect np.ndarray, shape = [N, 3], dtype = int");
            }
            const auto n_active_voxel = voxel.shape[0];
            mActiveVoxels.reserve(n_active_voxel);
            auto ptr = voxel.data<int>();
            for (uint64_t i = 0; i < voxel.shape[0]; ++i) {
                auto x = ptr[i * 3 + 0];
                auto y = ptr[i * 3 + 1];
                auto z = ptr[i * 3 + 2];
                if (checkRange(x) && checkRange(y) && checkRange(z)) {
                    auto idx = getIndex(x, y, z);
                    if (idx >= mOccupy.size()) {
                        throw std::runtime_error("voxel index out of bounds");
                    }
                    mOccupy[idx] = true;
                    mActiveVoxels.emplace_back(x, y, z);
                } else {
                    throw std::runtime_error("voxel out of range");
                }
            }
        }

        std::string toObj() const {
            static const std::array<std::tuple<int, int, int>, 3> dxyz = {
                    std::tuple<int, int, int>{1, 0, 0},   // +x
                    std::tuple<int, int, int>{0, 1, 0},   // +y
                    std::tuple<int, int, int>{0, 0, 1},   // +z
            };
            static const std::array<bool, 2> forwards = {true, false};

            std::vector<int> points_to_idx((N + 1) * (N + 1) * (N + 1), -1);
            std::stringstream vert, face;
            vert << std::setprecision(6);
            int vertex_count = 0;

            auto getPointIndex = [this](int x, int y, int z) -> size_t {
                return x + (N + 1) * y + (N + 1) * (N + 1) * z;
            };

            for (const auto& [x, y, z] : mActiveVoxels) {
                for (const auto& [dx, dy, dz] : dxyz) {
                    for(bool forward: forwards) {
                        int sgn = 2*forward-1;
                        int backward = 1-forward;
                        int nx = x + sgn*dx;
                        int ny = y + sgn*dy;
                        int nz = z + sgn*dz;
                        if (checkRange(nx) && checkRange(ny) && checkRange(nz) && mOccupy[getIndex(nx, ny, nz)]) {
                            continue;
                        }

                        auto vertices = std::array<std::tuple<int, int, int>, 4>{
                                std::tuple<int, int, int>{x+forward, y+forward, z+forward},
                                std::tuple<int, int, int>{x+(sgn*(dx+dy-dz)+1)/2, y+(sgn*(dz-dx+dy)+1)/2, z+(sgn*(dx-dy+dz)+1)/2},
                                std::tuple<int, int, int>{x+backward+sgn*dx, y+backward+sgn*dy, z+backward+sgn*dz},
                                std::tuple<int, int, int>{x+(sgn*(dx-dy+dz)+1)/2, y+(sgn*(dx-dz+dy)+1)/2, z+(sgn*(dy-dx+dz)+1)/2}
                        };

                        face << "f ";
                        for (auto [vx, vy, vz]: vertices) {
                            size_t idx = getPointIndex(vx, vy, vz);
                            if (points_to_idx[idx] == -1) {
                                vertex_count++;
                                points_to_idx[idx] = vertex_count;
                                vert << "v " << vx * mScale << " " << vy * mScale << " " << vz * mScale << "\n";
                            }
                            face << points_to_idx[idx] << ' ';
                        }
                        face << '\n';
                    }
                }
            }
            return vert.str() + face.str();
        }
    };

} // end namespace voxel_visualize
#endif