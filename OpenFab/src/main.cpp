#include "GCodeConverter.hpp"
#include "FabSlicer.hpp"
#include "generate_commands.hpp"
#include "server.hpp"
#include "BasicGeometry.hpp"
#include <stdio.h>
#include <Eigen/Dense>
#include <vector>

template <typename T>
using Vector3 = Eigen::Matrix<T,3,1>;

// sample main code for FabSlicer, you should create your own main code
int main() {
    // load a stl file and convert to a connected mesh (obj)
    // mesh::TriMesh<double> tri_mesh(std::string(PROJECT_SOURCE_DIR) + "/data/cube.stl", 1);
    mesh::TriMesh<double> tri_mesh(std::string(PROJECT_SOURCE_DIR) + "/data/bunny_watertight.stl", 0.01);
    // mesh::TriMesh<double> tri_mesh(std::string(PROJECT_SOURCE_DIR) + "/data/dragon.stl", 0.2);
    

    // you can visualize your mesh as an obj file to check if it's loaded correctly
    // tri_mesh.WriteToObj(std::string(PROJECT_SOURCE_DIR) + "/data/bunny_watertight.obj");

    // create a FabSlicer instance
    fab_translation::FabSlicer<double> fab(tri_mesh, 0.0, 2.0, 0.03, 0.05);
    // fab_translation::FabSlicer<double> fab(tri_mesh, 0.0, 2.0, 0.03, 0.1);

    std::vector<std::vector<std::vector<Eigen::Vector3d>>> contour;
    std::vector<std::vector<std::pair<Eigen::Vector3d, Eigen::Vector3d>>> infill_edges;

    fab.RunTranslation(contour, infill_edges);

    // visualize your results with ply format
    std::string contour_file = std::string(PROJECT_SOURCE_DIR) + "/data/bunny-contour.ply";
    fab.VisualizeContour(contour_file, 0.001, contour);

    std::string infill_file = std::string(PROJECT_SOURCE_DIR) + "/data/bunny-infill.ply";
    fab.VisualizeInfill(infill_file, 0.001, infill_edges);

    return 0;
}

