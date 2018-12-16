#include "GCodeConverter.hpp"
#include "FabSlicer.hpp"
#include "generate_commands.hpp"
#include "server.hpp"
#include "BasicGeometry.hpp"
#include "args.hxx"
#include <stdio.h>
#include <Eigen/Dense>
#include <vector>
#include <algorithm>
#include <random>
#include <limits>

template <typename T>
using Vector3 = Eigen::Matrix<T,3,1>;

template <typename T>
using Vector2 = Eigen::Matrix<T,2,1>;

bool ransacGenerateSpine(
        std::vector<std::vector<Vector3<double>>> contours,
        Vector3<double>& spineLine,
        double materialThickness,
        std::vector<bool>& inlierMask,
        int ransacIters = 1000,
        int randSeed = -1) {
    std::mt19937 rand_gen(randSeed);
    int bestInlierCount = 0;
    double bestSumSqMargin = 0.0;
    Vector3<double> bestLine;
    std::vector<bool> bestInlierMask;

    // Find the centroid of every contour
    std::vector<Vector2<double>> centroids;
    for (const auto& contour : contours) {
       int pointCount = 0;
       Vector3<double> pointSum = Vector3<double>::Zero();
       for (const auto& point : contour) {
           pointSum += point;
           pointCount++;
       }
       Vector3<double> pointMean = pointSum / pointCount;
       // Ignore the Z coordinate
       centroids.push_back(Vector2<double>(pointMean(0), pointMean(1)));
    }

    if (centroids.size() < 2) return false;

    std::vector<size_t> sampleIndices(centroids.size());
    for (int i = 0; i < centroids.size(); i++) {
        sampleIndices[i] = i;
    }

    for (int iter = 0; iter < ransacIters; iter++) {
        // Sample 2 centroids
        std::shuffle(sampleIndices.begin(), sampleIndices.end(), rand_gen);
        Eigen::Matrix<double, 2, 2> P;
        P.row(0) = centroids[sampleIndices[0]];
        P.row(1) = centroids[sampleIndices[1]];
        Vector2<double> c = Vector2<double>::Ones();
        Vector2<double> sampleLineNH = P.colPivHouseholderQr().solve(-c);
        Vector3<double> sampleLine;
        sampleLine << sampleLineNH(0), sampleLineNH(1), 1.0;
        sampleLine /= sampleLineNH.norm();

        // Count inliers (number of contours that lie on both sides of the line)
        int inlierCount = 0;
        double sampleSumSqMargin = 0.0;
        std::vector<bool> sampleInlierMask(contours.size());
        int contourIndex = 0;
        for (const auto& contour : contours) {
            double maxDist = -std::numeric_limits<double>::max();
            double minDist = std::numeric_limits<double>::max();
            for (const auto& point : contour) {
                Vector3<double> pointH;
                pointH << point(0), point(1), 1.0;
                double signed_dist = pointH.dot(sampleLine);
                maxDist = std::max(maxDist, signed_dist);
                minDist = std::min(minDist, signed_dist);
            }
            bool isInlier = (maxDist > (0.5 * materialThickness)) &&
                            (minDist < (-0.5 * materialThickness));
            sampleInlierMask[contourIndex] = isInlier;
            if (isInlier) {
                // Contour has material on both sides of the line
                inlierCount++;
                double margin = std::min(maxDist - 0.5 * materialThickness,
                                         -0.5 * materialThickness - minDist);
                sampleSumSqMargin += margin * margin;
            }
            contourIndex++;
        }

        // Keep line with best inlier count and best sum of squared margins
        if (inlierCount >= bestInlierCount && sampleSumSqMargin > bestSumSqMargin) {
            bestInlierCount = inlierCount;
            bestSumSqMargin = sampleSumSqMargin;
            bestLine = sampleLine;
            bestInlierMask = std::move(sampleInlierMask);
        }
    }

    if (bestInlierCount == 0) return false;

    //// Refine line using all inliers
    //Eigen::MatrixXd P_inliers(bestInlierCount, 2);
    //int j = 0;
    //for (int i = 0; i < centroids.size(); i++) {
    //    if (bestInlierMask[i]) {
    //        P_inliers.row(j) = centroids[i];
    //        j++;
    //    }
    //}
    //spineLine = P_inliers.colPivHouseholderQr().solve(-Eigen::VectorXd::Ones(bestInlierCount));

    spineLine = bestLine;
    inlierMask = std::move(bestInlierMask);
    return true;
}

int main(int argc, char* argv[]) {
    args::ArgumentParser parser("Convert a mesh into outlines for laser cutting a woodcraft kit.");
    args::HelpFlag help(parser, "help", "Display this help message", {'h', "help"});
    args::ValueFlag<double> thickness(parser, "thickness", "Thickness of the wood (mm)", {'t', "thickness"}, 3.17, args::Options::Single);
    args::ValueFlag<double> scale(parser, "scale", "Scale (mm per unit)", {'s', "scale"}, 0.01, args::Options::Single);
    args::ValueFlag<std::string> contour_file(parser, "contour_file", "File to output slicer contours to", {'c', "contour_file"}, args::Options::Single);
    args::Positional<std::string> mesh_file(parser, "mesh_file", "Mesh in STL format", args::Options::Required);

    // Don't show the (overly verbose) message about the '--' flag
    parser.helpParams.showTerminator = false;

    // Include default values in help text
    parser.helpParams.addDefault = true;

    // Parse options and handle errors
    try {
        parser.ParseCLI(argc, argv);
    } catch (const args::Completion& e) {
        std::cout << e.what();
        return 0;
    } catch (const args::Help&) {
        std::cout << parser;
        return 0;
    } catch (const args::ParseError& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    } catch (const args::RequiredError& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }
    
    // load a stl file and convert to a connected mesh (obj)
    mesh::TriMesh<double> tri_mesh(args::get(mesh_file), args::get(scale));

    int best_disconnected_piece_count = INT_MAX;
    std::vector<std::vector<std::vector<Eigen::Vector3d>>> best_contours;
    int best_axis = -1;
    for (int axis = 0; axis < 3; axis++) {
        // Create rotated vertices
        std::vector<Vector3<double>> rotated_vertices;
        rotated_vertices.reserve(tri_mesh._vertices.size());
        for (const auto& vertex : tri_mesh._vertices) {
            rotated_vertices.push_back(Vector3<double>(
                    vertex(axis),
                    vertex((axis + 1) % 3),
                    vertex((axis + 2) % 3)
                ));
        }

        mesh::TriMesh<double> rotated_mesh;
        rotated_mesh._vertices = std::move(rotated_vertices);
        rotated_mesh._elements = tri_mesh._elements;
        rotated_mesh._edges = tri_mesh._edges;
        rotated_mesh._normals = tri_mesh._normals;
        rotated_mesh._num_edges = tri_mesh._num_edges;

        // create a FabSlicer instance
        // passing in bottom > top will use the exact bounds of the mesh
        fab_translation::FabSlicer<double> fab(rotated_mesh, 1.0, -1.0, args::get(thickness), 0.05);

        std::vector<std::vector<std::vector<Eigen::Vector3d>>> contours;
        std::vector<std::vector<std::pair<Eigen::Vector3d, Eigen::Vector3d>>> infill_edges;

        fab.RunTranslation(contours, infill_edges);

        int disconnected_piece_count = 0;

        // Count disconnected pieces
        for (const auto& layer : contours) {
            disconnected_piece_count += std::max((int)layer.size() - 1, 0);
        }

        std::cout << "Axis " << axis << " DPC: " << disconnected_piece_count << std::endl;

        if (disconnected_piece_count < best_disconnected_piece_count) {
            best_disconnected_piece_count = disconnected_piece_count;
            best_contours = std::move(contours);
            best_axis = axis;
        }
    }

    std::cout << "Best axis: " << best_axis << std::endl;
    std::cout << "Best DPC: " << best_disconnected_piece_count << std::endl;

    // Flatten contours list
    std::vector<std::vector<Vector3<double>>> contoursFlat;
    for (const auto& layer : best_contours) {
        for (const auto& contour : layer) {
            contoursFlat.push_back(contour);
        }
    }

    // Generate spine
    Vector3<double> spineLine;
    std::vector<bool> inlierMask;
    ransacGenerateSpine(contoursFlat, spineLine, args::get(thickness), inlierMask);
    // Transform mesh into spine reference frame for slicing
    Vector3<double> spineNormal(spineLine(0), spineLine(1), 0.0);
    Vector3<double> spineV(0.0, 0.0, 1.0);
    Vector3<double> spineU = spineV.cross(spineNormal);
    Eigen::Matrix3d spineR;
    spineR << spineU, spineV, spineNormal;
    Vector3<double> spineT(spineLine(0) * -spineLine(2),
                           spineLine(1) * -spineLine(2),
                           0.0);
    
    // Create rotated vertices
    std::vector<Vector3<double>> rotated_vertices;
    rotated_vertices.reserve(tri_mesh._vertices.size());
    for (const auto& vertex : tri_mesh._vertices) {
        rotated_vertices.push_back(Vector3<double>(
                vertex(best_axis),
                vertex((best_axis + 1) % 3),
                vertex((best_axis + 2) % 3)
            ));
    }

    std::vector<Vector3<double>> transformed_vertices;
    transformed_vertices.reserve(rotated_vertices.size());
    for (const auto& vertex : rotated_vertices) {
        transformed_vertices.push_back(spineR.transpose() * (vertex - spineT));
    }

    mesh::TriMesh<double> transformed_mesh;
    transformed_mesh._vertices = std::move(transformed_vertices);
    transformed_mesh._elements = tri_mesh._elements;
    transformed_mesh._edges = tri_mesh._edges;
    transformed_mesh._normals = tri_mesh._normals;
    transformed_mesh._num_edges = tri_mesh._num_edges;

    // Slice the mesh, transformed into the spine's coordinates
    fab_translation::FabSlicer<double> spine_fab(transformed_mesh, 0.0, 0.5, 1.0, 0.05);
    std::vector<std::vector<std::vector<Eigen::Vector3d>>> spine_contours;
    std::vector<std::vector<std::pair<Eigen::Vector3d, Eigen::Vector3d>>> spine_infill_edges;
    spine_fab.RunTranslation(spine_contours, spine_infill_edges);

    // Transform the spine contours back into world coordinates
    // There should be exactly one layer
    std::vector<std::vector<Vector3<double>>> world_spine_contours;
    for (const auto& contour : spine_contours[0]) {
        std::vector<Vector3<double>> world_contour;
        world_contour.reserve(contour.size());
        for (const auto& point : contour) {
            world_contour.push_back(spineR * point + spineT);
        }
        world_spine_contours.push_back(world_contour);
    }

    best_contours.push_back(world_spine_contours);

    std::cout << "Spine equation: " << spineLine << std::endl;

    // visualize your results with ply format
    if (!args::get(contour_file).empty()) {
        fab_translation::FabSlicer<double>::VisualizeContour(args::get(contour_file), 0.1, best_contours);
    }

    return 0;
}

