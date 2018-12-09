#pragma once
#include "tri_mesh.hpp"
#include "BasicGeometry.hpp"
#include "IntervalTree.hpp"
#include "cinolib/meshes/meshes.h"
#include <ctime>

namespace fab_translation {
    template <typename T>
    using Vector3 = Eigen::Matrix<T, 3, 1>;

    template <typename T>
    class IntersectionEdge {
    public:
        IntersectionEdge(int id0, int id1, Vector3<T> p0, Vector3<T> p1)
            : _id0(id0), _id1(id1), _p0(p0), _p1(p1) {
                if (_id0 > _id1) {
                    std::swap(_id0, _id1);
                    std::swap(_p0, _p1);
                }
            }
        const int id0() const { return _id0; }
        const int id1() const { return _id1; }
        Vector3<T> p0() { return _p0; }
        Vector3<T> p1() { return _p1; }

    private:
        int _id0, _id1;
        Vector3<T> _p0, _p1;
    };

    template <typename T>
    class FabSlicer {
        
    public:
        FabSlicer(mesh::TriMesh<T> tri_mesh, T bottom, T top, T dx,
            T infill_dx)
            : _tri_mesh(tri_mesh), _bottom(bottom), _top(top), _dx(dx), _infill_dx(infill_dx) {

            T lower_bound = tri_mesh.vertices(0)[2];
            T upper_bound = tri_mesh.vertices(0)[2];
            for (int i = 0;i < tri_mesh.vertices().size();++i) {
                lower_bound = std::min(lower_bound, tri_mesh.vertices(i)[2]);
                upper_bound = std::max(upper_bound, tri_mesh.vertices(i)[2]);
            }

            _interval_tree = data_structure::IntervalTree<T>(lower_bound - 0.1, upper_bound + 0.1);

            std::vector<data_structure::IntervalEntry<T>> triangle_intervals;
            triangle_intervals.clear();
            for (int i = 0;i < tri_mesh.elements().size();++i) {
                Eigen::Vector3i& element = tri_mesh.elements(i);
                T l = tri_mesh.vertices(element[0])[2];
                T r = tri_mesh.vertices(element[0])[2];
                for (int j = 1;j < 3;++j) {
                    l = std::min(l, tri_mesh.vertices(element[j])[2]);
                    r = std::max(r, tri_mesh.vertices(element[j])[2]);
                }
                data_structure::IntervalEntry<T> interval(l - 1e-4, r + 1e-4, i);
                triangle_intervals.push_back(interval);
            }

            _interval_tree.build(0,  _interval_tree.lower_bound(), _interval_tree.upper_bound(), triangle_intervals);

            /* compute infill range */
            _infill_x_lower_bound = 1000000.0; _infill_x_upper_bound = -1000000.0;
            _infill_y_lower_bound = 1000000.0; _infill_y_upper_bound = -1000000.0;
            for (int i = 0;i < tri_mesh.vertices().size();++i) {
                _infill_x_lower_bound = std::min(_infill_x_lower_bound, tri_mesh.vertices(i)[0]);
                _infill_x_upper_bound = std::max(_infill_x_upper_bound, tri_mesh.vertices(i)[0]);
                _infill_y_lower_bound = std::min(_infill_y_lower_bound, tri_mesh.vertices(i)[1]);
                _infill_y_upper_bound = std::max(_infill_y_upper_bound, tri_mesh.vertices(i)[1]);
            }

            _infill_x_lower_bound -= _infill_dx * 0.5;
            _infill_x_upper_bound += _infill_dx * 0.5;
            _infill_y_lower_bound -= _infill_dx * 0.5;
            _infill_y_upper_bound += _infill_dx * 0.5;
        }

        void RunTranslation(std::vector<std::vector<std::vector<Vector3<T>>>>& contour,
            std::vector<std::vector<std::pair<Vector3<T>, Vector3<T>>>>& infill_edges) {
            std::vector<std::vector<fab_translation::IntersectionEdge<T>>> intersection_edges;

            int t1 = std::clock();

            Slicing_accelerated(_tri_mesh, intersection_edges);

            int t2 = std::clock();
            printf("slicing mesh success... %.6lf seconds\n", (double)(t2 - t1) / 1000000.0);

            CreateContour(_tri_mesh, intersection_edges, contour);

            int t3 = std::clock();
            printf("creating contour success... %.6lf seconds\n", (double)(t3 - t2) / 1000000.0);

            Infill(contour, infill_edges);

            int t4 = std::clock();
            printf("infill success... %.6lf seconds", (double)(t4 - t3) / 1000000.0);
        }

        void Slicing_bruteforce(mesh::TriMesh<T>& tri_mesh, 
            std::vector<std::vector<IntersectionEdge<T>>> &intersection_edges) {

            std::vector<Eigen::Vector3i>& elements = tri_mesh.elements();
            std::vector<Vector3<T>>& vertices = tri_mesh.vertices();
            std::vector<Eigen::Vector3i>& edges = tri_mesh.edges();

            intersection_edges.clear();
            for (T h = _bottom; h <= _top; h += _dx) {
                std::vector<IntersectionEdge<T>> intersections_one_plane;
                intersections_one_plane.clear();

                geometry::Plane<T> plane(Vector3<T>(0, 0, h), Vector3<T>(0, 0, 1));
                for (int i = 0;i < elements.size();++i) {
                    geometry::Triangle<T> triangle(vertices[elements[i](0)], vertices[elements[i](1)], vertices[elements[i](2)]);
                    std::vector<std::pair<int, Vector3<T>>> intersections = triangle.IntersectPlane(plane);
                    int num_intersections = 0;
                    for (int j = 0;j < 3;++j) {
                        if ((intersections[j].first) != -1) {
                            ++ num_intersections;
                        }
                    }
                    if (num_intersections == 2) {
                        int cnt = 0;
                        int id[2];
                        Vector3<T> p[2];
                        for (int j = 0;j < 3;++j) {
                            if (intersections[j].first != -1) {
                                p[cnt] = intersections[j].second;
                                if (intersections[j].first != 3) {
                                    id[cnt] = elements[i][j];
                                } else {
                                    id[cnt] = vertices.size() + edges[i][j];
                                }
                                ++ cnt;
                            }
                        }
                        assert(cnt == 2);
                        intersections_one_plane.push_back(IntersectionEdge<T>(id[0], id[1], p[0], p[1]));        
                    }
                }

                intersection_edges.push_back(intersections_one_plane);
            }
        }

        void Slicing_accelerated(mesh::TriMesh<T>& tri_mesh,
            std::vector<std::vector<IntersectionEdge<T>>> &intersection_edges) {
            
            std::vector<Eigen::Vector3i>& elements = tri_mesh.elements();
            std::vector<Vector3<T>>& vertices = tri_mesh.vertices();
            std::vector<Eigen::Vector3i>& edges = tri_mesh.edges();

            intersection_edges.clear();
            for (T h = _bottom; h <= _top; h += _dx) {

                std::vector<data_structure::IntervalEntry<T>> candidates;
                _interval_tree.query(0, _interval_tree.lower_bound(), _interval_tree.upper_bound(), h, candidates);

                std::vector<IntersectionEdge<T>> intersections_one_plane;
                intersections_one_plane.clear();

                geometry::Plane<T> plane(Vector3<T>(0, 0, h), Vector3<T>(0, 0, 1));

                for (int i = 0;i < candidates.size();++i) {
                    int ii = candidates[i].id;
                    geometry::Triangle<T> triangle(vertices[elements[ii](0)], vertices[elements[ii](1)], vertices[elements[ii](2)]);
                    std::vector<std::pair<int, Vector3<T>>> intersections = triangle.IntersectPlane(plane);
                    int num_intersections = 0;
                    for (int j = 0;j < 3;++j) {
                        if ((intersections[j].first) != -1) {
                            ++ num_intersections;
                        }
                    }
                    if (num_intersections == 2) {
                        int cnt = 0;
                        int id[2];
                        Vector3<T> p[2];
                        for (int j = 0;j < 3;++j) {
                            if (intersections[j].first != -1) {
                                p[cnt] = intersections[j].second;
                                if (intersections[j].first != 3) {
                                    id[cnt] = elements[ii][j];
                                } else {
                                    id[cnt] = vertices.size() + edges[ii][j];
                                }
                                ++ cnt;
                            }
                        }
                        assert(cnt == 2);
                        intersections_one_plane.push_back(IntersectionEdge<T>(id[0], id[1], p[0], p[1]));    
                    }
                }

                intersection_edges.push_back(intersections_one_plane);
            }
        }

        void CreateContour(mesh::TriMesh<T>& tri_mesh,
            std::vector<std::vector<IntersectionEdge<T>>> &intersection_edges,
            std::vector<std::vector<std::vector<Vector3<T>>>>& contours) {
            
            contours.clear();
            std::vector<Vector3<T>> intersection_pos(tri_mesh.num_vertices() + tri_mesh.num_edges());
            std::vector<std::vector<int>> edges(tri_mesh.num_vertices() + tri_mesh.num_edges());;
            std::vector<int> added_to_graph(tri_mesh.num_vertices() + tri_mesh.num_edges(), -1);
            std::vector<int> visited(tri_mesh.num_vertices() + tri_mesh.num_edges(), -1);
            for (int i = 0;i < intersection_edges.size();++i) { // iterate each layer
                std::vector<std::vector<Vector3<T>>> contours_in_layer;
                contours_in_layer.clear();
                // construct edge graph
                for (int j = 0;j < intersection_edges[i].size();++j) {
                    int id0 = intersection_edges[i][j].id0();
                    int id1 = intersection_edges[i][j].id1();
                    if (added_to_graph[id0] < i) {
                        edges[id0].clear();
                        intersection_pos[id0] = intersection_edges[i][j].p0();
                        added_to_graph[id0] = i;
                    }
                    if (added_to_graph[id1] < i) {
                        edges[id1].clear();
                        intersection_pos[id1] = intersection_edges[i][j].p1();
                        added_to_graph[id1] = i;
                    }
                    edges[id0].push_back(id1);
                    edges[id1].push_back(id0);
                }

                // find contours
                for (int j = 0;j < intersection_edges[i].size();++j) {
                    int start_id = intersection_edges[i][j].id0();
                    if (visited[start_id] < i) { // a new contour
                        std::vector<int> contour_now;
                        contour_now.clear();
                        bool flag = false;
                        for (int now = start_id;;) {
                            visited[now] = i;
                            contour_now.push_back(now);
                            if (edges[now].size() < 2) {
                                break;
                            }
                            if (visited[edges[now][0]] < i) {
                                now = edges[now][0];
                            } else if (visited[edges[now][1]] < i) {
                                now = edges[now][1];
                            } else if (edges[now][0] == start_id || edges[now][1] == start_id) {
                                flag = true;
                                break;
                            } else {
                                break;
                            }
                        }
                        if (flag) {
                            std::vector<Vector3<T>> _contour_now;
                            _contour_now.clear();
                            for (int k = 0;k < contour_now.size();++k)
                                _contour_now.push_back(intersection_pos[contour_now[k]]);
                            contours_in_layer.push_back(_contour_now);
                        } else {
                            for (int k = 0;k < contour_now.size();++k)
                                visited[contour_now[k]] = -1;
                        }
                    }
                }

                contours.push_back(contours_in_layer);
            }
        }

        void Infill(std::vector<std::vector<std::vector<Vector3<T>>>>& contours,
            std::vector<std::vector<std::pair<Vector3<T>, Vector3<T>>>>& infill_edges) {
            
            infill_edges.clear();

            int num_dir_x = (int)((_infill_y_upper_bound - _infill_y_lower_bound + 1e-5) / _infill_dx) + 1;
            int num_dir_y = (int)((_infill_x_upper_bound - _infill_x_lower_bound + 1e-5) / _infill_dx) + 1;

            for (int i = 0;i < contours.size();++i) {
                std::vector<std::pair<Vector3<T>, Vector3<T>>> infill_edges_one_layer;
                infill_edges_one_layer.clear();

                // store all intersection points along each axis-parallel pattern
                std::vector<std::vector<T>> point_list_dir_x(num_dir_x, std::vector<T>());
                std::vector<std::vector<T>> point_list_dir_y(num_dir_y, std::vector<T>());
                for (int j = 0;j < contours[i].size();++j) {

                    for (int k = 0, k_plus_one = 1;k < contours[i][j].size();++k) {
                        
                        k_plus_one = k + 1;
                        if (k_plus_one == contours[i][j].size())
                            k_plus_one = 0;

                        Vector3<T> pos_0 = contours[i][j][k];
                        Vector3<T> pos_1 = contours[i][j][k_plus_one];

                        // first do for dir x
                        if (pos_0(1) > pos_1(1))
                            std::swap(pos_0, pos_1);
                        int id0 = (int)((pos_0(1) - _infill_y_lower_bound + 1e-6) / _infill_dx) + 1;
                        int id1 = (int)((pos_1(1) - _infill_y_lower_bound + 1e-6) / _infill_dx);

                        for (int kk = id0;kk <= id1;++kk) {
                            // intersect with x-direction pattern kk
                            T line_y = _infill_y_lower_bound + _infill_dx * kk;
                            T dist0 = pos_0(1) - line_y;
                            T dist1 = pos_1(1) - line_y;
                            point_list_dir_x[kk].push_back(pos_0(0) + (pos_1(0) - pos_0(0)) * dist0 / (dist0 - dist1));
                        }
                        
                        // then for dir y
                        if (pos_0(0) > pos_1(0))
                            std::swap(pos_0, pos_1);
                        id0 = (int)((pos_0(0) - _infill_x_lower_bound + 1e-6) / _infill_dx) + 1;
                        id1 = (int)((pos_1(0) - _infill_x_lower_bound + 1e-6) / _infill_dx);
                        for (int kk = id0;kk <= id1;++kk) {
                            // intersect with y-direction pattern kk
                            T line_x = _infill_x_lower_bound + _infill_dx * kk;
                            T dist0 = pos_0(0) - line_x;
                            T dist1 = pos_1(0) - line_x;
                            point_list_dir_y[kk].push_back(pos_0(1) + (pos_1(1) - pos_0(1)) * dist0 / (dist0 - dist1));
                        }
                    }
                }

                // generate infill edges
                T pos_z = _bottom + _dx * i;
                for (int j = 0;j < num_dir_x;++j) {
                    if (point_list_dir_x[j].size() == 0) continue;
                    std::sort(point_list_dir_x[j].begin(), point_list_dir_x[j].end());
                    T pos_y = _infill_y_lower_bound + _infill_dx * j;
                    for (int k = 0;k < point_list_dir_x[j].size() - 1;k += 2) {
                        infill_edges_one_layer.push_back(std::make_pair(Vector3<T>(point_list_dir_x[j][k], pos_y, pos_z), 
                            Vector3<T>(point_list_dir_x[j][k + 1], pos_y, pos_z)));
                    }
                }
                for (int j = 0;j < num_dir_y;++j) {
                    if (point_list_dir_y[j].size() == 0) continue;
                    std::sort(point_list_dir_y[j].begin(), point_list_dir_y[j].end());
                    T pos_x = _infill_x_lower_bound + _infill_dx * j;
                    for (int k = 0;k < point_list_dir_y[j].size() - 1;k += 2) {
                        infill_edges_one_layer.push_back(std::make_pair(Vector3<T>(pos_x, point_list_dir_y[j][k], pos_z),
                            Vector3<T>(pos_x, point_list_dir_y[j][k + 1], pos_z)));
                    }
                }

                infill_edges.push_back(infill_edges_one_layer);
            }
        }  

        void VisualizeSlicing(std::string file_name, 
            T point_density,
            std::vector<std::vector<IntersectionEdge<T>>> intersection_edges) {
            
            // generate point cloud for ply
            std::vector<Vector3<T>> points;
            points.clear();
            for (int i = 0;i < intersection_edges.size();++i)
                for (int j = 0;j < intersection_edges[i].size();++j) {
                    Vector3<T> s_pos = intersection_edges[i][j].p0();
                    Vector3<T> t_pos = intersection_edges[i][j].p1();
                    int num_steps = (int)((t_pos - s_pos).norm() / point_density) + 1;
                    for (int step = 0;step <= num_steps;++step) {
                        Vector3<T> pos = s_pos * ((T)step / num_steps) + t_pos * ((T)1.0 - (T)step / num_steps);
                        points.push_back(pos);
                    }
                }

            // output to ply
            FILE* fp = fopen(file_name.c_str(), "w");
            fprintf(fp, "ply\nformat ascii 1.0\n");
            fprintf(fp, "element vertex %d\n", (int)points.size());
            fprintf(fp, "property float32 x\nproperty float32 y\nproperty float32 z\n");
            fprintf(fp, "end_header\n");
            for (int i = 0;i < points.size();++i)
                if (std::is_same<T, float>::value)
                    fprintf(fp, "%.6f %.6f %.6f\n", points[i](0), points[i](1), points[i](2));
                else
                    fprintf(fp, "%.6lf %.6lf %.6lf\n", points[i](0), points[i](1), points[i](2));
            fclose(fp);
        }

        void VisualizeContour(std::string file_name,
            T point_density, 
            std::vector<std::vector<std::vector<Vector3<T>>>>& contour) {
            // generate point cloud for ply
            std::vector<Vector3<T>> points;
            points.clear();
            for (int i = 0;i < contour.size();++i)
                for (int j = 0;j < contour[i].size();++j) 
                    for (int k = 0;k < contour[i][j].size();++k) {
                        Vector3<T> s_pos = contour[i][j][k];
                        Vector3<T> t_pos = contour[i][j][(k + 1) % contour[i][j].size()];
                        int num_steps = (int)((t_pos - s_pos).norm() / point_density) + 1;
                        for (int step = 0;step <= num_steps;++step) {
                            Vector3<T> pos = s_pos * ((T)step / num_steps) + t_pos * ((T)1.0 - (T)step / num_steps);
                            points.push_back(pos);
                        }
                    }

            // output to ply
            FILE* fp = fopen(file_name.c_str(), "w");
            fprintf(fp, "ply\nformat ascii 1.0\n");
            fprintf(fp, "element vertex %d\n", (int)points.size());
            fprintf(fp, "property float32 x\nproperty float32 y\nproperty float32 z\n");
            fprintf(fp, "end_header\n");
            for (int i = 0;i < points.size();++i)
                if (std::is_same<T, float>::value)
                    fprintf(fp, "%.6f %.6f %.6f\n", points[i](0), points[i](1), points[i](2));
                else
                    fprintf(fp, "%.6lf %.6lf %.6lf\n", points[i](0), points[i](1), points[i](2));
            fclose(fp);
        }

        void VisualizeInfill(std::string file_name,
            T point_density,
            std::vector<std::vector<std::pair<Vector3<T>, Vector3<T>>>>& infill_edges) {
            // generate point cloud for ply
            std::vector<Vector3<T>> points;
            points.clear();
            for (int i = 0;i < infill_edges.size();++i)
                for (int j = 0;j < infill_edges[i].size();++j) {
                    int num_steps = (int)((infill_edges[i][j].first - infill_edges[i][j].second).norm() / point_density) + 1;
                    for (int k = 0;k <= num_steps;++k)
                        points.push_back(infill_edges[i][j].first + (infill_edges[i][j].second - infill_edges[i][j].first) * (T)k / (T)num_steps);
                }

            // output to ply
            FILE* fp = fopen(file_name.c_str(), "w");
            fprintf(fp, "ply\nformat ascii 1.0\n");
            fprintf(fp, "element vertex %d\n", (int)points.size());
            fprintf(fp, "property float32 x\nproperty float32 y\nproperty float32 z\n");
            fprintf(fp, "end_header\n");
            for (int i = 0;i < points.size();++i)
                if (std::is_same<T, float>::value)
                    fprintf(fp, "%.6f %.6f %.6f\n", points[i](0), points[i](1), points[i](2));
                else
                    fprintf(fp, "%.6lf %.6lf %.6lf\n", points[i](0), points[i](1), points[i](2));
            fclose(fp);
        }
    private:
        mesh::TriMesh<T> _tri_mesh;

        /* Variables for slicing */
        T _bottom, _top, _dx;

        /* Variables for infill algorithm */
        T _infill_dx; // infill pattern will be equal-length grid
        T _infill_x_lower_bound, _infill_x_upper_bound;
        T _infill_y_lower_bound, _infill_y_upper_bound;

        /* accelerated data structure */
        data_structure::IntervalTree<T> _interval_tree;
    };
}