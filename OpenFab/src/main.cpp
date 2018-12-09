/*
#include <unsupported/Eigen/CXX11/Tensor>
#include <autodiff_engine.hpp>
#include <igl/opengl/glfw/Viewer.h>
int main(int argc, char *argv[])
{
    // Inline mesh of a cube
    const Eigen::MatrixXd V= (Eigen::MatrixXd(8,3)<<
                                                  0.0,0.0,0.0,
            0.0,0.0,1.0,
            0.0,1.0,0.0,
            0.0,1.0,1.0,
            1.0,0.0,0.0,
            1.0,0.0,1.0,
            1.0,1.0,0.0,
            1.0,1.0,1.0).finished();
    const Eigen::MatrixXi F = (Eigen::MatrixXi(12,4)<<
                                                    1,7,5,
            1,3,7,
            1,4,3,
            1,2,4,
            3,8,7,
            3,4,8,
            5,7,8,
            5,8,6,
            1,5,6,
            1,6,2,
            2,6,8,
            2,8,4).finished().array()-1;
    // Plot the mesh
    igl::opengl::glfw::Viewer viewer;
    viewer.data().set_mesh(V, F);
    viewer.data().set_face_based(true);
    viewer.launch();
}
 */

/* This is a base application for cinolib (https://github.com/maxicino/cinolib).
* It will show a GL canvas (and associated control panel) to interact
* with a hexahedral mesh.
*
* In case you don't need a GUI, you can drop the "Drawable" prefix from the mesh data type.
* What you will get is a lighter yet fully operational mesh data structure, just
* without the burden of OpenGL code necessary for rendering!
*
* Enjoy!
*/

//#define CINOLIB_USES_OPENGL
//#define CINOLIB_USES_QT

//#include <cinolib/meshes/meshes.h>
//#include <cinolib/gui/qt/qt_gui_tools.h>

//demo 2
/*
#include "mesh_importer.hpp"
#include "hex_mesh.hpp"
#include <igl/opengl/glfw/Viewer.h>
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
int main(int argc, char **argv) {
    std::string s = "/home/aespielberg/ResearchCode/FossFab/ExternalLibs/cinolib/examples/data/rockerarm.mesh";
    mesh::HexMesh<double> hex_mesh = mesh::MeshImporter::LoadHexMesh<double>(s);
    auto elements = hex_mesh.Elements();
    auto vertices = hex_mesh.Vertices();
    auto faces = hex_mesh.TetMeshRep().Faces();
    for (int i = 0; i < elements->rows(); ++i){
        for (int j = 0; j < elements->cols(); ++j){
            std::cout << vertices->row(elements->coeffRef(i, j)) << " | ";
        }
        std::cout << std::endl;
    }
    std::cout << *faces << std::endl;
    igl::opengl::glfw::Viewer viewer;
    viewer.data().set_mesh(*vertices, *faces);
    viewer.launch();
    std::cout << "Done with OpenFab.  Have a nice day!" << std::endl;
}
*/


// // Test triangle mesh intersection
// #include "BasicGeometry.hpp"
// #include <iostream>

// int main() {
//     geometry::Vector3<float> p0(0, 0, 0);
//     geometry::Vector3<float> p1(0, 1, 0);
//     geometry::Vector3<float> p2(0.3, 0, 1.1);
//     geometry::Triangle<float> triangle(p0, p1, p2);
//     geometry::Vector3<float> p(0, 0, 1.0);
//     geometry::Vector3<float> normal(0, 0, 1);
//     geometry::Plane<float> plane(p, normal);

//     auto intersections = triangle.IntersectPlane(plane);
//     for (int i = 0;i < 3;++i) {
//         std::cerr << intersections[i].first << " : " << intersections[i].second.transpose() << std::endl;
//     }
// }

// // Test interval tree
// #include "tri_mesh.hpp"
// #include "FabTranslation.hpp"
// #include <iostream>
// #include <ctime>

// int main() {
//     std::vector<fab_translation::IntervalEntry<float>> intervals;
//     intervals.clear();
//     std::srand(2000);
//     for (int i = 0;i < 2000;++i) {
//         float l = (float)(std::rand() % 10000) / 9.0;
//         float r = (float)(std::rand() % 10000) / 9.0;
//         if (l > r) 
//             std::swap(l, r);
//         intervals.push_back(fab_translation::IntervalEntry<float>(l, r, -1));
//     }

//     printf("start build\n");
//     fab_translation::IntervalTree<float> tree(0.0, 1200.0);
//     tree.build(0, 0.0, 1200.0, intervals);
//     printf("build done\n");
//     for (int i = 0;i < 2000;++i) {
//         float x = (float)(std::rand() % 10000) / 9.0;
//         int ans0 = 0;
//         int ans1 = 0;
//         for (int j = 0;j < 2000;++j)
//             if (intervals[j].l < x + 1e-7 && intervals[j].r > x - 1e-7)
//                 ++ ans0;
//         std::vector<fab_translation::IntervalEntry<float>> results;
//         tree.query(0, 0.0, 1200.0, x, results);
//         ans1 = results.size();
//         if (ans0 != ans1)
//             printf("wrong!\n");
//         else {
//             printf("%d %d pass\n", ans0, ans1);
//         }
//     }

//     return 0;
// }

// Test stl loader
// Test bruteforce slicing

// Test accelerated slicing
// Test contour creation
// Test infill algorithm
// #include "tri_mesh.hpp"
// #include "FabTranslation.hpp"
// #include <iostream>
// #include <ctime>


// int main() {
//     int t0 = std::clock();
//     // mesh::TriMesh<double> tri_mesh("/home/eanswer/Projects/data/bunny_watertight.stl");
//     // mesh::TriMesh<double> tri_mesh("/home/eanswer/Projects/data/dragon.stl");
//     mesh::TriMesh<double> tri_mesh("/home/eanswer/Projects/data/cube.stl");

//     int t1 = std::clock();
//     printf("load mesh success... %d\n", t1 - t0);

//     // fab_translation::FabTranslation<double> fab(tri_mesh, 0.0, 200.0, 3.0, 5.0);
//     // fab_translation::FabTranslation<double> fab(tri_mesh, 0.0, 10.0, 1.0, 0.5);
//     fab_translation::FabTranslation<double> fab(tri_mesh, 0.0, 1.0, 0.1, 0.1);
//     std::vector<std::vector<fab_translation::IntersectionEdge<double>>> intersection_edges;

//     // fab.Slicing_bruteforce(tri_mesh, 35.0, 40.0, 5, intersection_edges);
//     fab.Slicing_accelerated(tri_mesh, intersection_edges);

//     int t2 = std::clock();
//     printf("slicing mesh success... %d\n", t2 - t1);
//     std::vector<std::vector<std::vector<Eigen::Vector3d>>> contour;
//     fab.CreateContour(tri_mesh, intersection_edges, contour);

//     int t3 = std::clock();
//     printf("creating contour success... %d\n", t3 - t2);

//     std::vector<std::vector<std::pair<Eigen::Vector3d, Eigen::Vector3d>>> infill_edges;
//     fab.Infill(contour, infill_edges);

//     int t4 = std::clock();
//     printf("infill success... %d\n", t4 - t3);

//     std::string slicing_file = "/home/eanswer/Projects/data/cube-slicing.ply";
//     fab.VisualizeSlicing(slicing_file, 0.005, intersection_edges);

//     std::string contour_file = "/home/eanswer/Projects/data/cube-contour.ply";
//     fab.VisualizeContour(contour_file, 0.005, contour);

//     std::string infill_file = "/home/eanswer/Projects/data/cube-infill.ply";
//     fab.VisualizeInfill(infill_file, 0.005, infill_edges);

//     return 0;
// }


// #include "generate_commands.hpp"
// #include <Eigen/Eigen>
// #include <iostream>
// #include "server.hpp"

// int main(){
//     /*
//     FossFab::Command command;
//     std::cout << command.kCommandCodeFieldNumber << std::endl;
//     std::cout << "hello world" << std::endl;
//      */


//     std::vector<Eigen::Vector3d> poses;

//     // for (int i = 0; i < 1; ++i) {
//     //     poses.push_back(Eigen::Vector3d::UnitZ() * 0.2);
//     // }

//     network_communication::GenerateCommands::ResetCommand();
//     poses.push_back(Eigen::Vector3d(0, 0, 1));
//     network_communication::GenerateCommands::MoveCommand(poses, false);
//     poses.clear();
//     poses.push_back(Eigen::Vector3d(0, 0, 0));
//     network_communication::GenerateCommands::MoveCommand(poses, true);
//     poses.clear();
//     poses.push_back(Eigen::Vector3d(1, 0, 0));
//     network_communication::GenerateCommands::MoveCommand(poses, true);
//     poses.clear();
//     poses.push_back(Eigen::Vector3d(1, 0, 1));
//     network_communication::GenerateCommands::MoveCommand(poses, true);
//     poses.clear();
//     poses.push_back(Eigen::Vector3d(0, 0, 1));
//     network_communication::GenerateCommands::MoveCommand(poses, true);
// }


// #include "GCodeConverter.hpp"
// #include "FabSlicer.hpp"
// #include "generate_commands.hpp"
// #include "server.hpp"
// 
// int main() {
//     int t0 = std::clock();
//     // mesh::TriMesh<double> tri_mesh("/home/aespielberg/ResearchCode/OpenFab/data/cube.stl");
//     // mesh::TriMesh<double> tri_mesh("/home/eanswer/Projects/data/bunny_watertight.stl", 0.01);
//     mesh::TriMesh<double> tri_mesh(std::string(PROJECT_SOURCE_DIR) + "/data/bunny_watertight.stl", 0.01);
//     int t1 = std::clock();
//     printf("load mesh success... %.6lf seconds\n", (double)(t1 - t0) / 1000000.0);
// 
//     tri_mesh.WriteToObj(std::string(PROJECT_SOURCE_DIR) + "/data/bunny_watertight.obj");
//     fab_translation::FabSlicer<double> fab(tri_mesh, 0.0, 2.0, 0.03, 0.05);
// 
//     std::vector<std::vector<std::vector<Eigen::Vector3d>>> contour;
//     std::vector<std::vector<std::pair<Eigen::Vector3d, Eigen::Vector3d>>> infill_edges;
// 
//     fab.RunTranslation(contour, infill_edges);
// 
//     // std::string contour_file = "/home/aespielberg/ResearchCode/OpenFab/data/cube-contour.ply";
//     std::string contour_file = std::string(PROJECT_SOURCE_DIR) + "/data/bunny-contour.ply";
//     fab.VisualizeContour(contour_file, 0.001, contour);
// 
//     // std::string infill_file = "/home/aespielberg/ResearchCode/OpenFab/data/cube-infill.ply";
//     std::string infill_file = std::string(PROJECT_SOURCE_DIR) + "/data/bunny-infill.ply";
//     fab.VisualizeInfill(infill_file, 0.001, infill_edges);
// 
//     std::vector<std::vector<Eigen::Vector3d>> paths;
//     paths.clear();
//     network_communication::GenerateCommands::ResetCommand();
//     // translate to gcode
//     for (int i = 0;i < contour.size();++i) { // for each layer
//         // first print contour
//         // printf("print contour");
//         paths.clear();
//         for (int j = 0;j < contour[i].size();++j) {
//             // first, move to the first contour point without extrude
//             std::vector<Eigen::Vector3d> path;
//             path.clear();
//             path.push_back(contour[i][j][0]);
//             // then print contour
//             path.clear();
//             for (int k = 0;k < contour[i][j].size();++k)
//                 path.push_back(contour[i][j][k]);
//             path.push_back(contour[i][j][0]);
//             for (int k = 0;k < path.size();++k)
//                 std::swap(path[k](1), path[k](2));
//             paths.push_back(path);
//         }
//         fab_translation::GCodeConverter::ConvertToGCode(paths, nullptr);
//         paths.clear();
//         // then print infill
//         for (int j = 0;j < infill_edges[i].size();++j) {
//             // first, move to the end of segment
//             std::vector<Eigen::Vector3d> path;
//             path.clear();
//             path.push_back(infill_edges[i][j].first);
//             path.push_back(infill_edges[i][j].second);
//             for (int k = 0;k < path.size();++k)
//                 std::swap(path[k](1), path[k](2));
//             paths.push_back(path);
//         }
//         fab_translation::GCodeConverter::ConvertToGCode(paths, nullptr);
//     }
// 
//     //std::string * gcode_filename = new std::string("/home/eanswer/Projects/data/gcode_bunny.gcode");
//     //fab_translation::GCodeConverter::ConvertToGCode(paths, gcode_filename);
// 
//     return 0;
// }

// #include "GCodeConverter.hpp"
// #include <Eigen/Eigen>

// int main(){



//      std::vector<std::vector<Eigen::Vector3d>> paths;

//      for (int i = 0; i < 10; ++i) {
//          std::vector<Eigen::Vector3d> path;
//          for (int j = 0; j < 10; ++j){
//              path.push_back(Eigen::Vector3d(cos(j), sin(j), i));
//          }
//          paths.push_back(path);
//      }
    
//     for (int i = 0; i < 10; ++i) {
//          std::vector<Eigen::Vector3d> path;
//          for (int j = 0; j < 10; ++j){
//              path.push_back(Eigen::Vector3d(cos(j) + 2, sin(j) + 2, i));
//          }
//          paths.push_back(path);
//      }

//      std::string * gcode_filename = new std::string("~/generated.gcode");
//      fab_translation::GCodeConverter::ConvertToGCode(paths, nullptr);

//      std::cout << "DONE!  Thank you for choosing OpenFab.  Have a nice day." << std::endl;

//  }

#include "voxelizer.hpp"
#include "marching_cube.hpp"
#include <iostream>
#include <ctime>
#include <string>

int main(int argc, char* argv[]) {
    // Usage:
    // Slow version:
    // ./OpenFab bunny.stl 2.0
    // ./OpenFab fandisk.stl 0.05
    // ./OpenFab spot.stl 0.125
    // ./OpenFab dragon.stl 0.05
    // Fast version:
    // ./OpenFab bunny.stl 2.0 fast
    // ./OpenFab fandisk.stl 0.05 fast
    // ./OpenFab spot.stl 0.125 fast
    // ./OpenFab dragon.stl 0.05 fast
    // Approximation:
    // ./OpenFab bunny_with_hole.stl 2.0 approx
    // ./OpenFab spot_with_whole.stl 0.125 approx
    // Marching cube version:
    // ./OpenFab bunny_voxel_info.txt
    // ./OpenFab dragon_voxel_info.txt
    // ./OpenFab fandisk_voxel_info.txt
    // ./OpenFab spot_voxel_info.txt
    if (argc == 2) {
        // Marching cube version.
        const std::string info_file(argv[1]);
        mesh::MarchingCube<double> mc(std::string(PROJECT_SOURCE_DIR) + "/data/assignment2/" + info_file);
        mc.BuildMesh();
        const std::string name = info_file.substr(0, info_file.size() - std::string("_voxel_info.txt").size());
        mc.ExportMeshToFile(name + "_mc.stl");
        return 0;
    }

    int t0 = std::clock();
    const std::string stl_name(argv[1]);
    const double dx = std::stod(argv[2]);
    mesh::Voxelizer<double> voxelizer(std::string(PROJECT_SOURCE_DIR) + "/data/assignment2/" + stl_name, dx);
    int t1 = std::clock();
    std::cout << "load mesh success... " << (double)(t1 - t0) / 1000000.0 << " seconds." << std::endl;
    std::cout << "Bounding box: " << voxelizer.pmin().transpose() << ", " << voxelizer.pmax().transpose() << std::endl;
    std::cout << "Number of voxels: " << voxelizer.voxel_num().transpose() << std::endl;
    if (argc == 3) {
        voxelizer.BasicVoxelization();
    } else {
        const std::string flag(argv[3]);
        if (flag == "fast")
            voxelizer.AdvancedVoxelization();
        else if (flag == "approx")
            voxelizer.AdvancedVoxelizationWithApproximation();
        else {
            std::cout << "ERROR: unexpected flag" << std::endl;
            exit(0);
        }
    }
    std::cout << "Voxelization done..." << std::endl;
    // Export results to mesh.
    const std::string stl_prefix = stl_name.substr(0, stl_name.size() - 4);
    const std::string voxel_file_name = stl_prefix + "_voxel.stl";
    std::cout << "Saving results to " << voxel_file_name << std::endl;
    voxelizer.WriteVoxelToMesh(voxel_file_name);
    const std::string voxel_info_file_name = stl_prefix + "_voxel_info.txt";
    std::cout << "Saving voxel info to " << voxel_info_file_name << std::endl;
    voxelizer.WriteVoxelToFile(voxel_info_file_name);
    std::cout << "Results saved..." << std::endl; 
    return 0;
}
