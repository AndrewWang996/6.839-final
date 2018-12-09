#pragma once
#include "mesh.hpp"
#include "tet_mesh.hpp"

#include <cinolib/meshes/meshes.h>
//#include <cinolib/vertex_mass.h>

namespace mesh{

    template<typename T>
    class HexMesh : public Mesh<T>{

    protected:

        std::shared_ptr<MatrixX<T>> vertices;
        std::shared_ptr<Eigen::MatrixXi> elements;
        std::shared_ptr<Eigen::MatrixXi> faces;
        const TetMesh<T> tet_mesh; //The underlying tetmesh representation

    public:

        HexMesh(const std::string& file_path) :
            elements(std::make_shared<Eigen::MatrixXi>()),
            faces(std::make_shared<Eigen::MatrixXi>()),
            vertices(std::make_shared<MatrixX<T>>()),
            tet_mesh(vertices, elements, faces, std::make_shared<cinolib::Hexmesh<>>(file_path.c_str()))

        {}

        HexMesh(std::shared_ptr<cinolib::Hexmesh<>> c_hex_mesh) :
                tet_mesh(vertices, elements, faces, c_hex_mesh)

        {}


        const std::shared_ptr<const MatrixX<T>> Vertices() const{
            return tet_mesh.Vertices();
        }

        const std::shared_ptr<const Eigen::MatrixXi> Elements() const{
            return std::const_pointer_cast<const Eigen::MatrixXi>(elements);
        }

        const std::shared_ptr<const Eigen::MatrixXi> Faces() const{
            return std::const_pointer_cast<const Eigen::MatrixXi>(faces);
        }


        const TetMesh<T>& TetMeshRep() const{
            return tet_mesh;
        }

        static HexMesh InitializeFromGrid(const uint nx, const uint ny, const uint nz,
                                          const double res_x, const double res_y, const double res_z,
                                          const double off_x = 0.0, const double off_y = 0.0, const double off_z = 0.0){


            std::vector<cinolib::vec3d> verts(nx * ny * nz);

            const uint idx = 0;
            for (uint x = 0; x < nx; ++x){
                for (uint y = 0; y < ny; ++y){
                    for (uint z = 0; z < nz; ++z){
                        verts[idx] = cinolib::vec3d(x * nx - off_x, y * ny - off_y, z * nz - off_z);
                    }
                }
            }

            std::vector<std::vector<uint>> polys;

            for (uint x = 0; x < nx - 1; ++x){
                for (uint y = 0; y < ny - 1; ++y) {
                    for (uint z = 0; z < nz - 1; ++z) {

                        std::vector<uint> indices = {z + nz * (y * nx + x),
                                                     z + nz * (y * nx + x + 1),
                                                     z + nz * ((y + 1) * nx + x),
                                                     z + nz * ((y + 1) * nx + x + 1),
                                                     z + 1 + nz * (y * nx + x),
                                                     z + 1 + nz * (y * nx + x + 1),
                                                     z + 1 + nz * ((y + 1) * nx + x),
                                                     z + 1 + nz * ((y + 1) * nx + x + 1)};

                    }
                }
            }






            std::shared_ptr<cinolib::Hexmesh<>> hm = std::make_shared<cinolib::Hexmesh<>>(verts, polys);

            return HexMesh(hm);
        }


    };
}

