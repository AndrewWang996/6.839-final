// Reference Implementation by: Alexandre Kaspar <akaspar@mit.edu>
#pragma once

#include <algorithm>
#include <cassert>
#include <Eigen/Core>
#include <Eigen/Sparse>
#include <igl/min_quad_with_fixed.h>
#include <igl/parallel_for.h>
#include <igl/polar_svd3x3.h>

// edge indices
typedef typename Eigen::Matrix<int, 3, 2> Matrix32i;
const Matrix32i edges = (Matrix32i() << 1, 2, 2, 0, 0, 1).finished();

/**
 * Problem to minimize
 *
 *   sum_k sum_{ij in F(k)} c_ij | (v_i - v_j) - R_k (v_i' - v_j')|^2
 *
 */

/**
 * Compute entries of the cotangent matrix C
 * (c_ij) for i in F, and j in {0,1,2} = triangle edge index
 *
 * @param V (nx3) input vertices
 * @param F (mx3) input triangle faces
 * @param C (mx3) output cotangents
 */
template <typename TypeV, typename TypeF, typename TypeC>
void cotmatrix_entries(
  const TypeV & V,
  const TypeF & F,
  TypeC & C)
{
  typedef typename TypeV::Scalar Scalar;

  const int m = F.rows();
  C.resize(m, 3);
  // compute per face (m of them)
  igl::parallel_for( m, [&V, &F, &C](const int i){
    // squared edge lengths
    Scalar l[3], e[3];
    for(int j = 0; j < 3; ++j){
      const auto &v0 = V.row(F(i, edges(j, 0)));
      const auto &v1 = V.row(F(i, edges(j, 1)));
      Scalar l2 = (v0 - v1).squaredNorm();
      l[j] = std::sqrt(l2);
      e[j] = l2;
    }
    // face area using Heron's formula
    // @see https://en.wikipedia.org/wiki/Heron%27s_formula
    // improved stability using Karan's formula
    // @see Miscalculating Area and Angles of a Needle-like Triangle, Kahan 2014
    /* unstable version:
    Scalar s = (l[0] + l[1] + l[2]) * 0.5;
    Scalar dblA = s * (s - l[0]) * (s - l[1]) * (s - l[2]);
    */
    std::sort(l, l + 3); // ensures l[0] <= l[1] <= l[2]
    // stable version:
    Scalar arg  = (l[0] + (l[1] + l[2]))
                * (l[2] - (l[0] - l[1]))
                * (l[2] + (l[0] - l[1]))
                * (l[0] + (l[1] - l[2]));
    Scalar dblA = 2.0 * 0.25 * std::sqrt(arg);
    if(dblA != dblA){
      // NaN
      dblA = 0.0; // C(i, j) = infinity
    }
    
    // cotangent and diagonal entries
    // @see https://www.cs.toronto.edu/~jacobson/images/alec-jacobson-thesis-2013-compressed.pdf
    // @see 2.1.2 Cotangent weights
    // @see Appendix 4.8: A cotangent Laplacian for images as surfaces
    for(int j = 0; j < 3; ++j){
      Scalar e0 = e[edges(j, 0)];
      Scalar e1 = e[edges(j, 1)];
      Scalar e2 = e[3 - edges(j, 0) - edges(j, 1)];
      C(i, j) = (e0 + e1 - e2) / dblA / 4.0;
    }

  }, 1000);
}

/**
 * Compute cotangent laplacian sparse matrix L
 * (l_ii) = 1/2 sum_ik (cot \alpha_k + cot \beta_k)
 * (l_ij) = -1/2 (cot \alpha_j + cot \beta_j)
 * where ij and ik are edges.
 *
 * @param V (nx3) input vertices
 * @param F (mx3) input triangle faces
 * @param C (mx3) input cotangent weights
 * @param L (nxn) output laplacian matrix
 * @see https://graphics.stanford.edu/courses/cs468-13-spring/assets/lecture12-lu.pdf
 */
template <typename TypeV, typename TypeF, typename TypeC, typename TypeL>
void cotmatrix(
  const TypeV & V,
  const TypeF & F,
  const TypeC & C,
  TypeL & L)
{
  typedef typename TypeV::Scalar Scalar;
  typedef typename Eigen::Triplet<Scalar> Entry;

  // allocate laplacian
  const int n = V.rows();
  const int m = F.rows();
  L.resize(n, n);
  L.reserve(10 * n);

  // create sparse matrix entries
  std::vector<Entry> entries;
  entries.reserve(m * 3 * 4);
  //TODO: Fill in entries  
  L.setFromTriplets(entries.begin(), entries.end());
}

/**
 * Create the entries of a block of the ARAP K matrix
 *
 * @param V (nx3) input vertices
 * @param F (mx3) input faces
 * @param C (nxn) input cotangent weight matrix
 * @param d (1)   input dimension (0, 1, or 2) corresponding to (x, y or z)
 * @param fromRow input starting entry row offset
 * @param fromCol input starting entry column offset
 * @param entries output list of triplets to which new entries are appended
 * @param loop    input whether to use the loop implementation from IGL (slower)
 */
template <typename MatV, typename MatF, typename MatC, typename Scalar>
void arap_linear_block(
  const MatV & V,
  const MatF & F,
  const MatC & C,
  const int d,
  const int fromRow,
  const int fromCol,
  std::vector<Eigen::Triplet<Scalar> > &entries,
  bool loop = false)
{
  typedef typename Eigen::Triplet<Scalar> Entry;

  // spokes+rims energy implementation
  const int n = V.rows();
  const int m = F.rows(); // number of faces
  
  // workspace
  entries.reserve(entries.size() + 7 * n);

  // for all faces
  for(int i = 0; i < m; ++i){
    // for each half-edge
    for(int e = 0; e < 3; ++e){
      //TODO: fill in entries here.
    }
  }
}


/**
 * Precomputations for the ARAP quadratic problem using min_quad_with_fixed.
 *
 * min_quad_with_fixed minimizes
 *
 *    trace( 0.5*Z'*A*Z + Z'*B + constant )
 *
 * subject to
 *
 *    Z(known,:) = Y, and
 *    Aeq*Z = Beq
 * 
 * The call is:
 * 
 *    min_quad_with_fixed_precompute(A, known, Aeq, pd, data)
 * 
 * where pd = simplification when A is positive definite
 *
 *
 * @param V (nx3)   input vertices of rest shape
 * @param F (mx3)   input faces pointing to vertices
 * @param b (bx1)   input indices of handle vertices
 * @param data      output datastructure for min_quad precomputation
 * @param K (nx3n)  output K matrix
 */
template <typename TypeV, typename TypeF, typename Typeb, typename Scalar>
void arap_precompute(
  const TypeV & V,
  const TypeF & F,
  const Typeb & b,
  igl::min_quad_with_fixed_data<Scalar> & data,
  Eigen::SparseMatrix<Scalar> & K)
{
  using namespace Eigen;

  const int n = V.rows();
  // const int m = F.rows();

  // compute cotangent weight matrix c_ij
  Matrix<Scalar, Dynamic, Dynamic> C;
  cotmatrix_entries(V, F, C);

  // get Laplacian matrix
  SparseMatrix<Scalar> L;
  cotmatrix(V, F, C, L);

  // compute K = [Kx, Ky, Kz]
  std::vector<Triplet<Scalar> > entries;
  //TODO: Compute K (Hint: Use igl::min_quad_with_fixed_precompute)
}

/**
 * Find best rotation matrix R that minimizes
 * the following linear trace problem:
 *
 *   trace(C * R)
 *
 * where C = V^T K = [X Y Z]^T K = [Cx Cy Cz]^T
 * with  V in |R^{nx3}  and K in |R^{nx3n}
 *       C in |R^{3x3n} and R in |R^{3nx3} = [Rx Ry Rz]^T
 *
 * Notes:
 *   Rx, Ry, Rz in |R^{nx3}
 * with
 *   Rd = [Rd_1^T Rd_2^T ... Rd_n^T]^T
 * where
 *   Rd_i in |R^{1x3}
 * which can be assembled into individual rotations
 *   R_i = [Rx_i^T Ry_i^T Rz_i^T]^T in |R^{3x3}
 *
 * The sub-rotations R_i are all disconnected.
 * Thus they are solved in parallel independently using SVD.
 *
 * @param C (3x3n) input coefficient matrix encapsulating V and K
 * @param R (3nx3) output rotation matrix
 */
template<typename MatS, typename MatR>
void solve_rotations(
  const Eigen::PlainObjectBase<MatS> & C,
  Eigen::PlainObjectBase<MatR> & R)
{
  typedef typename MatR::Scalar Scalar;
  typedef Eigen::Matrix<Scalar, 3, 3> Mat3;
  
  const int dim = C.rows();
  assert(dim == 3 && "Only supporting 3D rotations");
  const int nr = C.cols() / dim;
  assert(nr * dim == C.cols() && "Dimension of C must use multiple of dimension");

  R.resize(3 * nr, 3);
  // for each rotation
  //TODO: compute R
}

template<typename Scalar, typename Typebc, typename TypeU>
void arap_single_iteration(
  const igl::min_quad_with_fixed_data<Scalar> & data,
  const Eigen::SparseMatrix<Scalar> & K,
  const Typebc & bc,
  TypeU & U)
{
  using namespace Eigen;
  
  // number of vertices
  const int n = U.rows();

  // enforce boundary conditions exactly
  for(int bi = 0; bi < bc.rows(); bi++)
    U.row(data.known(bi)) = bc.row(bi);

  // matrix of rotations
  Matrix<Scalar, Dynamic, Dynamic> R(n * 3, 3);

  // local solve: fix vertices, find rotations
local: {
    //TODO local solve.  Hint: use solve_rotations here
  }

  // global solve: fix rotations, find vertices
global: {
    typedef Matrix<Scalar, Dynamic, 1> Vector;
    //TODO: global solve.  Hint: use calls to igl::min_quad_with_fixed_solve here.
    }
}
