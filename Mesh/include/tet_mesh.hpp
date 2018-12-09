#pragma once
#include "mesh.hpp"
#include <vector>
#include <cinolib/meshes/meshes.h>
#include <igl/median.h>


namespace mesh{



    template<typename T>
    class TetMesh : public Mesh<T>{



    protected:
        std::shared_ptr<cinolib::Hexmesh<>> c_hex_mesh;
        std::shared_ptr<MatrixX<T>> vertices;
        std::shared_ptr<Eigen::MatrixXi> faces;
        std::shared_ptr<Eigen::MatrixXi> elements;

        void PopulateTet(const uint element_num, const uint idx, const std::vector<uint>& verts, const std::vector<uint>& hex_indices){
            const uint tets_per_hex = 4;
            for (uint i = 0; i < hex_indices.size(); ++i){
                elements->coeffRef(tets_per_hex  * element_num + idx, i) = verts[hex_indices[i]];
            }
        }

        void PopulateElements(const uint element_num, std::shared_ptr<Eigen::MatrixXi> elements){

            //TODO: Not really sure, tbqh, if this is the correct order.  Need to verify this.
            std::vector<uint> verts = c_hex_mesh->poly_verts_id(element_num);

            //TODO: set the hex mesh.
            Eigen::Matrix<double, 3, 1> median;
            ComputeElementMedian(median, element_num);
            OrderRowElement(median, verts);

            for (uint i = 0; i < verts.size(); ++i){
                elements->coeffRef(element_num, i) = verts[i]; //TODO: remove this for loop
            }



            PopulateTet(element_num, 0, verts, std::vector<uint>{3, 4, 1, 6});
            PopulateTet(element_num, 1, verts, std::vector<uint>{0, 1, 3, 4});
            PopulateTet(element_num, 2, verts, std::vector<uint>{0, 1, 3, 4});
            PopulateTet(element_num, 3, verts, std::vector<uint>{1, 2, 3, 6});

        }

        void PopulateFaceFromFace(const uint face_num, const uint idx, const Eigen::VectorXi& verts, const std::vector<uint>& face_indices){
            const uint faces_per_face = 2;
            for (uint i = 0; i < face_indices.size(); ++i){
                faces->coeffRef(faces_per_face  * face_num + idx, i) = verts[face_indices[i]];
            }
        }

        void PopulateFacesFromFaces(const uint face_num, std::shared_ptr<Eigen::MatrixXi> faces){

            //TODO: Not really sure, tbqh, if this is the correct order.  Need to verify this.
            //TODO: Just going to create a few faces, eh?
            Eigen::VectorXi verts = faces->row(face_num);

            PopulateFaceFromFace(face_num, 0, verts, std::vector<uint>{0, 1, 2});
            PopulateFaceFromFace(face_num, 1, verts, std::vector<uint>{2, 3, 0});

        }

        void PopulateFaceFromElement(const uint element_num, const uint idx, const Eigen::VectorXi& verts, const std::vector<uint>& face_indices){
            const uint faces_per_element = 3;
            const uint num_external_faces = 2 * c_hex_mesh->num_faces();

            for (uint i = 0; i < face_indices.size(); ++i){
                faces->coeffRef(num_external_faces + faces_per_element  * element_num + idx, i) = verts(face_indices[i]);
            }
        }

        void PopulateFacesFromElements(const uint element_num, std::shared_ptr<Eigen::MatrixXi> elements){

            const Eigen::VectorXi verts = elements->row(element_num);

            PopulateFaceFromElement(element_num, 0, verts, std::vector<uint>{4, 3, 1});
            PopulateFaceFromElement(element_num, 1, verts, std::vector<uint>{6, 1, 3});
            PopulateFaceFromElement(element_num, 2, verts, std::vector<uint>{3, 4, 6});

        }

        void OrderRowElement(const Eigen::Matrix<T, 3, 1>& median, std::vector<uint>& elements){

            std::vector<uint> output_elements(elements.size()); //we're just going to do this out of place because it's easier.

            for (uint elem : elements){
                Eigen::Matrix<T, 3, 1> vertex = this->vertices->row(elem);



                //now we order them according to checks: https://www.colorado.edu/engineering/CAS/courses.d/AFEM.d/AFEM.Ch11.d/AFEM.Ch11.pdf

                if (vertex[0] > median(0) && vertex[1] < median(1) && vertex[2] < median(2)){
                    output_elements[0] = elem;
                }else if (vertex[0] > median(0) && vertex[1] > median(1) && vertex[2] < median(2)){
                    output_elements[1] = elem;
                }
                else if (vertex[0] < median(0) && vertex[1] > median(1) && vertex[2] < median(2)){
                    output_elements[2] = elem;
                }
                else if (vertex[0] < median(0) && vertex[1] < median(1) && vertex[2] < median(2)){
                    output_elements[3] = elem;
                }
                else if (vertex[0] > median(0) && vertex[1] < median(1) && vertex[2] > median(2)){
                    output_elements[4] = elem;
                }else if (vertex[0] > median(0) && vertex[1] > median(1) && vertex[2] > median(2)){
                    output_elements[5] = elem;
                }
                else if (vertex[0] < median(0) && vertex[1] > median(1) && vertex[2] > median(2)){
                    output_elements[6] = elem;
                }
                else if (vertex[0] < median(0) && vertex[1] < median(1) && vertex[2] > median(2)){
                    output_elements[7] = elem;
                }

            }


            elements = output_elements;
        }

        void ComputeElementMedian(Eigen::Matrix<T, 3, 1>& median, const uint element_num) const{

            std::vector<uint> verts = c_hex_mesh->poly_verts_id(element_num);



            Eigen::Matrix<T, 8, 3> mat;

            for (uint i = 0; i < verts.size(); ++i){
                mat.row(i) = this->vertices->row(verts[i]);
            }

            for (uint i = 0; i < 3; ++i){
                igl::median(mat.col(i), median(i));
            }

        }

        //TODO: change these to be of fixed size

        void GenerateFaces(std::shared_ptr<Eigen::MatrixXi> faces, std::shared_ptr<Eigen::MatrixXi> elements){

            for (uint i = 0; i < c_hex_mesh->num_faces(); ++i){
                //TODO: organize this properly
                        std::vector<uint> face_vertices = c_hex_mesh->face_verts_id(i);
                faces->row(i) << face_vertices[0], face_vertices[1], face_vertices[2], face_vertices[3];

            }
            /*
            for (uint i = 0; i < elements->rows(); ++i){
                Eigen::VectorXi element = elements->row(i);
                faces->row(i * 6 + 0) << element(0), element(1), element(3), element(4);
                faces->row(i * 6 + 1) << element(1), element(2), element(6), element(3);
                faces->row(i * 6 + 2) << element(2), element(3), element(7), element(6);
                faces->row(i * 6 + 3) << element(3), element(0), element(4), element(7);
                faces->row(i * 6 + 4) << element(0), element(3), element(2), element(1);
                faces->row(i * 6 + 5) << element(4), element(3), element(6), element(7);

            }
             */
        }



    public:

        const std::shared_ptr<const MatrixX<T>> Vertices() const{
            return std::const_pointer_cast<const MatrixX<T>>(vertices);
        }

        const std::shared_ptr<const Eigen::MatrixXi> Elements() const{
            return std::const_pointer_cast<const Eigen::MatrixXi>(elements);
        }

        const std::shared_ptr<const Eigen::MatrixXi> Faces() const{
            return std::const_pointer_cast<const Eigen::MatrixXi>(faces);
        }

        //TODO: need one for the CG version
        TetMesh<T>(std::shared_ptr<MatrixX<T>> vertices, std::shared_ptr<Eigen::MatrixXi> elements,
                std::shared_ptr<Eigen::MatrixXi> faces, std::shared_ptr<cinolib::Hexmesh<>> c_hex_mesh);

    };

    //specialized methods:

    template <>
    TetMesh<double>::TetMesh(std::shared_ptr<MatrixX<double>> vertices, std::shared_ptr<Eigen::MatrixXi> elements,
            std::shared_ptr<Eigen::MatrixXi> faces, std::shared_ptr<cinolib::Hexmesh<>> c_hex_mesh):
    c_hex_mesh(c_hex_mesh),
    vertices(vertices)
    {
        const uint num_vertices = c_hex_mesh->num_verts();
        const uint num_faces = c_hex_mesh->num_faces();
        const uint num_elements = c_hex_mesh->num_polys();


        this->vertices = std::make_shared<MatrixX<double>>(num_vertices, 3);
        this->elements = std::make_shared<Eigen::MatrixXi>(num_elements * 4, 4);
        this->faces = std::make_shared<Eigen::MatrixXi>(num_faces*2 + num_elements * 3, 3);


        elements->resize(num_elements, 8);
        faces->resize(num_faces, 4);


        for (uint i = 0; i < num_vertices; ++i){
            //TODO: handle CGs here
            cinolib::vec3d& vec = c_hex_mesh->vert(i);
            this->vertices->coeffRef(i, 0) = vec.x();
            this->vertices->coeffRef(i, 1) = vec.y();
            this->vertices->coeffRef(i, 2) = vec.z();
        }


        for (uint i = 0; i < num_elements; ++i){
            PopulateElements(i, elements);
        }



        //now we've ordered the hex meshes, can we generate faces from here?
        GenerateFaces(faces, elements);

        for (uint i = 0; i < num_faces; ++i){
            PopulateFacesFromFaces(i, faces);
        }

        std::cout << *(this->faces) << std::endl;



        for (uint i = 0; i < num_elements; ++i){
            PopulateFacesFromElements(i, elements);
        }


    }



}

