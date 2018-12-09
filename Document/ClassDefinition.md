#### Class Definitions

##### TriMesh class

> TriMesh defines a triangle mesh. It generates a triangle mesh with connectivity from STL (STL is just a edge soup without any connectivity information).

###### Member Variables

- `_vertices`: a vector of `Eigen::Vector3<T>` for all vertex positions, in order. 
- `_elements`: a vector of  `Eigen::Vector3i` for all triangle's three vertex indices. The indices of each triangle are stored in right hand order. 
- `_edges`: a vector of `Eigen::Vector3i` for all triangle's three edge indices. (all edges are indexed and no duplicated edges with different index).
- `_normals`: a vector `Eigen::Vector3<T>`for all triangle storing its normal (pointing outward).
- Each variable can be accessed by the corresponding functions named by removing the leading underline. (e.g. `_normals` can be accessed through function `normals()`).

###### Member Functions

- `TriMesh(string stl_file_name, T scale = 1.0)`: Construction. Input is a file name for stl model and a scale value. It will generate a connected model while the coordinates of each vertex will be scaled.
- `WriteToObj(string obj_file_name)`: Write the mesh in obj format to a file.



##### IntervalTree class

> IntervalTree defines an interval tree data structure

###### Structure

```C++
class IntervalTree {
public:
    /* TODO */
    void build();
    /* TODO */
    void query();
private:
    T _lower_bound, _upper_bound; 	// The interval represented by the whole tree
    vector<int> _left_child; 		// each node in tree has a unique id from 0, _left_child[i] is the id for node i's left child, -1 if node i doesn't have left child.
    vector<int> _right_child; 		// same as _left_child but for right child.
    int _num_nodes; 				// total number of the nodes
    T eps;							// error threshold
};
```

```C++
/* We also provide a class IntervalEntry for each individual interval, you can also implement your own struct for that */
class IntervalEntry {
public:
    T l, r;		// Left and right end of that interval
    int id;		// an id field for any needed use (e.g. to help find which triangle is this interval from)
};
```



##### FabSlicer class

> Main part of the code you need to complete is in this class

Structure

```C++
class FabSlicer {
public:
    
    /* Step-by-step visualization code */
    /* refer to code and comment for input paramters and details */
    void VisualizeSlicing();
    void VisualizeContour();
    void VisualizeInfill();
    
private:
	TriMesh<T> _tri_mesh;			// triangle mesh for the model to be printed
    T _bottom, _top;				// the z range of the model
    T _infill_dx;					// the grid length for infill pattern which is equal-length grid pattern
    IntervalTree<T> _interval_tree; // data structure for accelerated slicing algorithm
};
```

