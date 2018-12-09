#pragma once



#include <cstring>
#include <iostream>
#include <vector>
#include <cppad/cg.hpp>
#include "hex_mesh.hpp"




namespace mesh {

class MeshImporter{


public:

    template<typename T>
    static mesh::HexMesh<T> LoadHexMesh(const std::string& file_path){
        return HexMesh<double>(file_path);
    }


    //method which reads a hex mesh from file
    //calls a converter method to convert to our format
    //can reparameterize it with CGs a







};
}
