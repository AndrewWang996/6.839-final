#pragma once
#include <Eigen/Dense>
#include <vector>
#include <set>
#include <iostream>

namespace geometry {
    template <typename T>
    using Vector3 = Eigen::Matrix<T, 3, 1>;
    template <typename T>
    using Vector2 = Eigen::Matrix<T, 2, 1>;
    template <typename T>
    using VectorX = Eigen::Matrix<T, Eigen::Dynamic, 1>;

    // Q1: implement Graham Scan for 2D convex hull
    // The input is a vector of 2D points
    // The output should be the 2D points on the convex hull
    // Remove duplicates and sort your return vector
    // for comparing with the reference result
    template <typename T>
    std::vector<Vector2<T>> ConvexHull2D(const std::vector<Vector2<T>> &points) {
        return points;
    }

    // Q2: implement brute-force method for Nd Pareto front
    // The input is a vector of Nd points
    // The output should be the Nd points on the Pareto front
    // sort your return vector for comparing with 
    // the reference result
    template <typename T>
    std::vector<VectorX<T>> ParetoFrontNdNaive(const std::vector<VectorX<T>> &points) {
        return points;
    }

    // Q3: implement nlog(n) method for 2d Pareto front
    // The input is a vector of 2d points
    // The output should be the 2d points on the Pareto front
    // sort your return vector for comparing with 
    // the reference result
    template <typename T>
    std::vector<Vector2<T>> ParetoFront2D(const std::vector<Vector2<T>> &points) {
        return points;
    }

    // bonus question: implement nlog(n) method for 3d Pareto front
    // The input is a vector of 3d points
    // The output should be the 3d points on the Pareto front
    // sort your return vector for comparing with 
    // the reference result
    template <typename T>
    std::vector<Vector3<T>> ParetoFront3D(const std::vector<Vector3<T>> &points) {
        return points;
    }
}
