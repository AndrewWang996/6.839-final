#include "linear_material.hpp"
#include "deformable_body.hpp"
#include <stdio.h>
#include <iostream>
#include "poly_mesh.hpp"
#include "hexahedral_mesh.hpp"
#include "hex_deformable_body.hpp"
#include "typedefs.hpp"
#include <Eigen/IterativeLinearSolvers>
#include <Eigen/SparseCholesky>




#include <stdio.h>
#include <iostream>
#include <Eigen/IterativeLinearSolvers>
#include <Eigen/SparseCholesky>
#include <fstream>
#include <set>



void write_facet(std::ofstream& file, Eigen::Vector3d p0, Eigen::Vector3d p1, Eigen::Vector3d p2) {
    file << "facet normal 0 0 0\n";
    file << "outer loop\n";
    file << "vertex " + std::to_string(p0.x()) + " " + std::to_string(p0.y()) + " " + std::to_string(p0.z()) + "\n";
    file << "vertex " + std::to_string(p1.x()) + " " + std::to_string(p1.y()) + " " + std::to_string(p1.z()) + "\n";
    file << "vertex " + std::to_string(p2.x()) + " " + std::to_string(p2.y()) + " " + std::to_string(p2.z()) + "\n";
    file << "endloop\n";
    file << "endfacet\n";
}

void write_voxel_grid(const std::string& filename, const Eigen::Matrix<double, 3, Eigen::Dynamic>& vertices, const Eigen::Matrix<int, 8, Eigen::Dynamic>& elements) {


    std::ofstream file;
    file.open(filename, std::ios::out);

    file << "solid voxel_grid \n";

    for (size_t i = 0; i < elements.cols(); ++i) {

        write_facet(file, vertices.col(elements(0, i)), vertices.col(elements(1, i)), vertices.col(elements(2, i)));
        write_facet(file, vertices.col(elements(1, i)), vertices.col(elements(3, i)), vertices.col(elements(2, i)));



        write_facet(file, vertices.col(elements(4, i)), vertices.col(elements(5, i)), vertices.col(elements(0, i)));
        write_facet(file, vertices.col(elements(5, i)), vertices.col(elements(1, i)), vertices.col(elements(0, i)));


        write_facet(file, vertices.col(elements(2, i)), vertices.col(elements(3, i)), vertices.col(elements(6, i)));
        write_facet(file, vertices.col(elements(3, i)), vertices.col(elements(7, i)), vertices.col(elements(6, i)));


        write_facet(file, vertices.col(elements(6, i)), vertices.col(elements(7, i)), vertices.col(elements(4, i)));
        write_facet(file, vertices.col(elements(7, i)), vertices.col(elements(5, i)), vertices.col(elements(4, i)));

        write_facet(file, vertices.col(elements(1, i)), vertices.col(elements(3, i)), vertices.col(elements(5, i)));
        write_facet(file, vertices.col(elements(3, i)), vertices.col(elements(7, i)), vertices.col(elements(5, i)));



        write_facet(file, vertices.col(elements(0, i)), vertices.col(elements(2, i)), vertices.col(elements(4, i)));
        write_facet(file, vertices.col(elements(2, i)), vertices.col(elements(6, i)), vertices.col(elements(4, i)));

    }
    file << "endsolid";

    file.close();

}


int main(int argc, char *argv[])
{

    const int dim = 3;

    //youngs =  10^7, poisson ratio = 0.45 - similar to that of silicone rubber
    materials::LinearElasticityMaterial<dim, double> linear_elasticity_material(10000000, 0.45);

    const size_t num_x_vertices = 5; //number of nodes (vertices) in x dimension of our mesh
    const size_t num_y_vertices = 3; //number of nodes (vertices) in y dimension of our mesh
    const size_t num_z_vertices = 2; //number of nodes (vertices) in z dimension of our mesh

    const size_t num_vertices = num_x_vertices * num_y_vertices * num_z_vertices;

    const double spacing = 0.05; //size of each edge, 5 cm
    materials::HexahedralMesh<double> hex_mesh =
            materials::HexahedralMeshCuboid<double>(Eigen::Vector3d::Zero(),
                    Eigen::Vector3i(num_x_vertices, num_y_vertices, num_z_vertices), spacing);

    //What does our undeformed mesh look like?
    write_voxel_grid("test.stl", hex_mesh.vertex(), hex_mesh.element());



    materials::HexDeformableBody<double> hex_def_body(linear_elasticity_material, hex_mesh.vertex(), 0.4, hex_mesh);

    //TODO: Students take it from here!
    
    std::cout << "Done with OpenFab!  Have a squishy day." << std::endl;




}
