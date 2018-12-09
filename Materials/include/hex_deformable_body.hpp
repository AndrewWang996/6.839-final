// Tao Du
// taodu@csail.mit.edu
// Oct 12, 2016
#pragma once
#include "deformable_body.hpp"
#include "hexahedral_mesh.hpp"
#include "typedefs.hpp"

namespace materials {

    template<typename T>
    class HexDeformableBody : public DeformableBody<3, T> {
    public:
        HexDeformableBody(const Material<3, T>& material,
                          const Matrix3X<T>& initial_vertex_position,
                          const T density, const HexahedralMesh<T>& undeformed_hex_mesh)
                          : DeformableBody<3, T>(material, initial_vertex_position, undeformed_hex_mesh),
                            hex_size_((undeformed_hex_mesh.vertex(undeformed_hex_mesh.element(0)(0)) - undeformed_hex_mesh.vertex(undeformed_hex_mesh.element(0)(1))).norm()) {}






        HexDeformableBody(const std::vector<std::reference_wrapper<const Material<3, T>>>& materials,
                          const std::vector<int>& material_id,
                          const Matrix3X<T>& initial_vertex_position,
                          const T density, const HexahedralMesh<T>& undeformed_hex_mesh)
                          : DeformableBody<3, T>(materials, material_id, initial_vertex_position, undeformed_hex_mesh),
                            hex_size_((undeformed_hex_mesh.vertex(undeformed_hex_mesh.element(0)(0)) - undeformed_hex_mesh.vertex(undeformed_hex_mesh.element(0)(1))).norm()) {}

        HexDeformableBody(const HexDeformableBody& hex_body) : DeformableBody<3, T>(hex_body),
                                                               hex_size_(hex_body.hex_size_) {}

        ~HexDeformableBody() {}


        //TODO: Students should fill this out
	//vertices is a matrix of the current vertex positions (3 x n)        
        const Eigen::SparseMatrix<T> ComputeStiffnessMatrix(
                const Matrix3X<T>& vertices) const{
            std::vector<Eigen::Triplet<T>> triplet_list;
            const int vertex_num = static_cast<int>(this->vertex_position_.size() / 3);
           
            Eigen::SparseMatrix<T> K(vertex_num * 3, vertex_num * 3);
            K.setFromTriplets(triplet_list.begin(), triplet_list.end());
            // Make sure K is symmetric.
            K = (K + Eigen::SparseMatrix<T>(K.transpose())) / 2.0;
            return K;
        }

        //return dphi (the deformation gradient) for a given voxel:
        //undeformed_vertex is a point in material space.
        //undeformed_cube are the vertices of an undeformed voxel
        //deformed_cube are the vertices of a the deformed voxel.
        static const Eigen::Matrix<T, 3, 3> DeformationGradient(
                const Vector3<T>& undeformed_vertex,
                const Eigen::Matrix<T, 3, 8>& undeformed_cube,
                const Eigen::Matrix<T, 3, 8>& deformed_cube){
            // Rename variables.
            const Vector3<T>& X = undeformed_vertex;
            const Eigen::Matrix<T, 3, 8>& X0 = undeformed_cube;
            const Eigen::Matrix<T, 3, 8>& x0 = deformed_cube;
            const T dx = X0(0, 4) - X0(0, 0);
            const T inv_dx = 1.0 / dx;
            const Vector3<T> offset = X - X0.col(0);
            const T rx = offset.x() / dx;
            const T ry = offset.y() / dx;
            const T rz = offset.z() / dx;
            Eigen::Matrix<T, 3, 3> F = Eigen::Matrix<T, 3, 3>::Zero();
            const T x_factor[2] = {1 - rx, rx};
            const T y_factor[2] = {1 - ry, ry};
            const T z_factor[2] = {1 - rz, rz};
            for (int i = 0; i < 2; ++i)
                for (int j = 0; j < 2; ++j)
                    for (int k = 0; k < 2; ++k) {
                        F.col(0) += x0.col(4 * i + 2 * j + k)
                                    * (i == 0 ? -inv_dx : inv_dx) * y_factor[j] * z_factor[k];
                        F.col(1) += x0.col(4 * i + 2 * j + k)
                                    * x_factor[i] * (j == 0 ? -inv_dx : inv_dx) * z_factor[k];
                        F.col(2) += x0.col(4 * i + 2 * j + k)
                                    * x_factor[i] * y_factor[j] * (k == 0 ? -inv_dx : inv_dx);
                    }
            return F;
        }




        //return dphi/dx for a given voxel:
        //undeformed_vertex is a point in material space.
        //undeformed_cube are the vertices of an undeformed voxel
        //deformed_cube are the vertices of a the deformed voxel.
        static const Eigen::Matrix<T, 9, 24> DeformationGradientPartialx(
                const Vector3<T>& undeformed_vertex,
                const Eigen::Matrix<T, 3, 8>& undeformed_cube,
                const Eigen::Matrix<T, 3, 8>& deformed_cube){

            const Vector3<T>& X = undeformed_vertex;
            const Eigen::Matrix<T, 3, 8>& X0 = undeformed_cube;
            Eigen::Matrix<T, 9, 24> Jacobian = MatrixX<T>::Zero(9, 24);
            const T dx = X0(0, 4) - X0(0, 0);
            const T inv_dx = 1.0 / dx;
            const Vector3<T> offset = X - X0.col(0);
            const T rx = offset.x() / dx;
            const T ry = offset.y() / dx;
            const T rz = offset.z() / dx;
            Eigen::Matrix<T, 3, 3> F = Eigen::Matrix<T, 3, 3>::Zero();
            const T x_factor[2] = { 1 - rx, rx };
            const T y_factor[2] = { 1 - ry, ry };
            const T z_factor[2] = { 1 - rz, rz };
            for (int i = 0; i < 2; ++i)
                for (int j = 0; j < 2; ++j)
                    for (int k = 0; k < 2; ++k) {
                        const int index = 4 * i + 2 * j + k;
                        const T scale_first_column = (i == 0 ? -inv_dx : inv_dx)
                                                          * y_factor[j] * z_factor[k];
                        Jacobian(0, 3 * index) += scale_first_column;
                        Jacobian(1, 3 * index + 1) += scale_first_column;
                        Jacobian(2, 3 * index + 2) += scale_first_column;
                        const T scale_second_column = x_factor[i]
                                                           * (j == 0 ? -inv_dx : inv_dx) * z_factor[k];
                        Jacobian(3, 3 * index) += scale_second_column;
                        Jacobian(4, 3 * index + 1) += scale_second_column;
                        Jacobian(5, 3 * index + 2) += scale_second_column;
                        const T scale_third_column = x_factor[i] * y_factor[j]
                                                          * (k == 0 ? -inv_dx : inv_dx);
                        Jacobian(6, 3 * index) += scale_third_column;
                        Jacobian(7, 3 * index + 1) += scale_third_column;
                        Jacobian(8, 3 * index + 2) += scale_third_column;
                    }
            return Jacobian;



        }

        virtual void getInitialNodes(Matrix3X<T>& initial_nodes){
            initial_nodes = this->undeformed_mesh_.vertex();
        }

    private:
        HexDeformableBody& operator=(const HexDeformableBody&);


        //TODO: Studnets fill this function out
        static const Eigen::Matrix<T, 8, 8> GaussIntegrationFactor() {
            // \int_{-1}^{1} f(x) dx \approx f(-1/sqrt(3)) + f(1/sqrt(3)).
            Eigen::Matrix<T, 8, 8> X0_coeff = Eigen::MatrixXd::Zero(8, 8);
            
            return X0_coeff;
        }



        const T hex_size_;
    };

}
