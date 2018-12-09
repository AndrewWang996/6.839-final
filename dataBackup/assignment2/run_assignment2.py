import os
import sys
import time

def run(command):
    t0 = time.time()
    os.system(command)
    t1 = time.time()
    print('Command: %s finished in %f seconds...' % (command, t1 - t0))

# Assignment 2.1: basic voxelization.
# run('./OpenFab/OpenFab bunny.stl 2.0')
# run('./OpenFab/OpenFab fandisk.stl 0.05')
run('./OpenFab/OpenFab spot.stl 0.125')
# run('./OpenFab/OpenFab dragon.stl 0.05')

'''
# Assignment 2.2: fast voxelization.
run('./OpenFab/OpenFab bunny.stl 2.0 fast')
run('./OpenFab/OpenFab fandisk.stl 0.05 fast')
run('./OpenFab/OpenFab spot.stl 0.125 fast')
run('./OpenFab/OpenFab dragon.stl 0.05 fast')

# Assignment 2.3: voxelization for non-watertight meshes.
run('./OpenFab/OpenFab bunny_with_hole.stl 2.0 approx')
run('./OpenFab/OpenFab spot_with_hole.stl 0.125 approx')

# Assignment 2.4: marching cubes.
run('./OpenFab/OpenFab bunny_voxel_info.txt')
run('./OpenFab/OpenFab fandisk_voxel_info.txt')
run('./OpenFab/OpenFab dragon_voxel_info.txt')
run('./OpenFab/OpenFab spot_voxel_info.txt')
'''
