#pragma once
#include <Eigen/Dense>
#include <vector>

namespace geometry {
    template <typename T>
    using Vector3 = Eigen::Matrix<T, 3, 1>;

    // the plane is represented by (x - _p) /dot _normal = 0
    template <typename T>
    class Plane {
    public:
        Plane(Vector3<T> p, Vector3<T> normal) {
            _p = p;
            _normal = normal;
            _normal.normalize();
        }

        Vector3<T>& p() { return _p; }
        Vector3<T>& normal() { return _normal; }
        
        // return if the point is on plane
        // also fill parameter dist as the signed distance from point to plane
        bool onPlane(Vector3<T> point, T& dist) {
            dist = (point - _p).dot(_normal);
            if (std::fabs(dist) < 1e-6) {
                return true;
            } else {
                return false;
            }
        }

    private:
        Vector3<T> _p;
        Vector3<T> _normal;
    };

    template <typename T>
    class Triangle {
    public:
        Triangle(Vector3<T> v0, Vector3<T> v1, Vector3<T> v2) {
            _vertices[0] = v0;
            _vertices[1] = v1;
            _vertices[2] = v2;
        }

        Vector3<T>* vertices() { return _vertices; }
        Vector3<T>& vertices(int idx) { return _vertices[idx]; }

        // always return 3 pairs for corresponding intersections with three edges
        // the first value in pair:
        //     -1: no intersection
        //      0: intersection at vertex 0
        //      1: intersection at vertex 1
        //      2: intersection at vertex 2
        //      3: intersection with edge
        // the second value in pair:
        //      the intersection coordinates
        std::vector<std::pair<int, Vector3<T>>> IntersectPlane(Plane<T> p) {
            std::vector<std::pair<int, Vector3<T>>> intersections;
            intersections.clear();
            for (int i = 0;i < 3;++i) {
                int id0 = i;
                int id1 = (i + 1) % 3;
                T dist0, dist1;
                if (p.onPlane(_vertices[id0], dist0)) {
                    intersections.push_back(std::make_pair(id0, _vertices[id0]));
                } else if (p.onPlane(_vertices[id1], dist1)) {
                    intersections.push_back(std::make_pair(-1, Vector3<T>::Zero()));
                } else if (dist0 * dist1 < 0) {
                    T factor = dist0 / (dist0 - dist1);
                    Vector3<T> intersection = 
                        _vertices[id0] + factor * (_vertices[id1] - _vertices[id0]);
                    intersections.push_back(std::make_pair(3, intersection));
                } else {
                    intersections.push_back(std::make_pair(-1, Vector3<T>::Zero()));
                }
            }

            return intersections;
        }

        // Assignment 2: implement ray-triangle intersection.
        // The ray is defined as r(t) = origin + t * dir.
        // You should return a scalar t such that r(t) is the intersection point. Which value
        // to return for the case of no intersections is up to you. You can also change the
        // signature of this function as you see fit.
        const T IntersectRay(const Vector3<T>& origin, const Vector3<T>& dir) const {
            /* Assignment 2. */
            /* Implement your code here */
            return 0;
        }

    private:
        Vector3<T> _vertices[3];
    };
}
