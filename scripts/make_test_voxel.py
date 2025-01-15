import numpy as np

if __name__ == "__main__":
    np.save("voxel1.npy", np.array([
        [0, 0, 0],
    ], dtype=np.int32))
    np.save("voxel2.npy", np.array([
        [0, 0, 0],
        [0, 0, 2],
        [0, 2, 0],
        [0, 2, 2],
        [2, 0, 0],
        [2, 0, 2],
        [2, 2, 0],
        [2, 2, 2],
        [1, 1, 1]
    ], dtype=np.int32))
