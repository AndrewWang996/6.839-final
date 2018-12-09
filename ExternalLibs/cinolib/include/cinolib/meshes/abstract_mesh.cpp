/*********************************************************************************
*  Copyright(C) 2016: Marco Livesu                                               *
*  All rights reserved.                                                          *
*                                                                                *
*  This file is part of CinoLib                                                  *
*                                                                                *
*  CinoLib is dual-licensed:                                                     *
*                                                                                *
*   - For non-commercial use you can redistribute it and/or modify it under the  *
*     terms of the GNU General Public License as published by the Free Software  *
*     Foundation; either version 3 of the License, or (at your option) any later *
*     version.                                                                   *
*                                                                                *
*   - If you wish to use it as part of a commercial software, a proper agreement *
*     with the Author(s) must be reached, based on a proper licensing contract.  *
*                                                                                *
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE       *
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.     *
*                                                                                *
*  Author(s):                                                                    *
*                                                                                *
*     Marco Livesu (marco.livesu@gmail.com)                                      *
*     http://pers.ge.imati.cnr.it/livesu/                                        *
*                                                                                *
*     Italian National Research Council (CNR)                                    *
*     Institute for Applied Mathematics and Information Technologies (IMATI)     *
*     Via de Marini, 6                                                           *
*     16149 Genoa,                                                               *
*     Italy                                                                      *
**********************************************************************************/
#include <cinolib/meshes/abstract_mesh.h>
#include <cinolib/stl_container_utilities.h>
#include <cinolib/inf.h>
#include <map>

namespace cinolib
{

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
bool AbstractMesh<M,V,E,P>::mesh_is_surface() const
{
    switch (mesh_type())
    {
        case TRIMESH       : return true;
        case QUADMESH      : return true;
        case POLYGONMESH   : return true;
        case TETMESH       : return false;
        case HEXMESH       : return false;
        case POLYHEDRALMESH: return false;
        default : assert(false);
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
bool AbstractMesh<M,V,E,P>::mesh_is_volumetric() const
{
    return !mesh_is_surface();
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
void AbstractMesh<M,V,E,P>::clear()
{
    bb.reset();
    //
    verts.clear();
    edges.clear();
    polys.clear();
    //
    M std_M_data;
    m_data = std_M_data;
    v_data.clear();
    e_data.clear();
    p_data.clear();
    //
    v2v.clear();
    v2e.clear();
    v2p.clear();
    e2p.clear();
    p2e.clear();
    p2p.clear();
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
vec3d AbstractMesh<M,V,E,P>::centroid() const
{
    vec3d bary(0,0,0);
    for(auto p : verts) bary += p;
    if (num_verts() > 0) bary/=static_cast<double>(num_verts());
    return bary;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
void AbstractMesh<M,V,E,P>::translate(const vec3d & delta)
{
    for(uint vid=0; vid<num_verts(); ++vid) vert(vid) += delta;
    update_bbox();
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
void AbstractMesh<M,V,E,P>::rotate(const vec3d & axis, const double angle)
{
    double R[3][3];
    bake_rotation_matrix(axis, angle, R);
    //
    vec3d c = centroid();
    //
    for(uint vid=0; vid<num_verts(); ++vid)
    {
        vert(vid) -= c;
        transform(vert(vid), R);
        vert(vid) += c;
    }
    //
    update_bbox();
    update_normals();
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
void AbstractMesh<M,V,E,P>::scale(const double scale_factor)
{
    vec3d c = centroid();
    translate(-c);
    for(uint vid=0; vid<num_verts(); ++vid) vert(vid) *= scale_factor;
    translate(c);
    update_bbox();
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
void AbstractMesh<M,V,E,P>::normalize_bbox()
{
    double s = 1.0/bbox().diag();
    for(uint vid=0; vid<num_verts(); ++vid) vert(vid) *= s;
    update_bbox();
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
void AbstractMesh<M,V,E,P>::update_bbox()
{
    bb.reset();
    for(uint vid=0; vid<num_verts(); ++vid)
    {
        vec3d v = vert(vid);
        bb.min = bb.min.min(v);
        bb.max = bb.max.max(v);
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
std::vector<Color> AbstractMesh<M,V,E,P>::vector_poly_colors() const
{
    std::vector<Color> colors;
    colors.reserve(num_polys());
    for(uint pid=0; pid<num_polys(); ++pid)
    {
        colors.push_back(poly_data(pid).color);
    }
    return colors;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
std::vector<int> AbstractMesh<M,V,E,P>::vector_poly_labels() const
{
    std::vector<int> labels;
    labels.reserve(num_polys());
    for(uint pid=0; pid<num_polys(); ++pid)
    {
        labels.push_back(poly_data(pid).label);
    }
    return labels;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
std::vector<double> AbstractMesh<M,V,E,P>::serialize_uvw(const int mode) const
{
    std::vector<double> uvw;
    uvw.reserve(num_verts());
    for(uint vid=0; vid<num_verts(); ++vid)
    {
        switch (mode)
        {
            case U_param  : uvw.push_back(vert_data(vid).uvw[0]); break;
            case V_param  : uvw.push_back(vert_data(vid).uvw[1]); break;
            case W_param  : uvw.push_back(vert_data(vid).uvw[2]); break;
            case UV_param : uvw.push_back(vert_data(vid).uvw[0]);
                            uvw.push_back(vert_data(vid).uvw[1]); break;
            case UW_param : uvw.push_back(vert_data(vid).uvw[0]);
                            uvw.push_back(vert_data(vid).uvw[2]); break;
            case VW_param : uvw.push_back(vert_data(vid).uvw[1]);
                            uvw.push_back(vert_data(vid).uvw[2]); break;
            case UVW_param: uvw.push_back(vert_data(vid).uvw[0]);
                            uvw.push_back(vert_data(vid).uvw[1]);
                            uvw.push_back(vert_data(vid).uvw[2]); break;
            default: assert(false);
        }
    }
    return uvw;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
void AbstractMesh<M,V,E,P>::deserialize_uvw(const std::vector<vec3d> & uvw)
{
    assert(uvw.size()==num_verts());
    for(uint vid=0; vid<num_verts(); ++vid)
    {
        vert_data(vid).uvw = uvw.at(vid);
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
void AbstractMesh<M,V,E,P>::copy_xyz_to_uvw(const int mode)
{
    for(uint vid=0; vid<num_verts(); ++vid)
    {
        switch (mode)
        {
            case U_param  : vert_data(vid).uvw[0] = vert(vid).x(); break;
            case V_param  : vert_data(vid).uvw[1] = vert(vid).y(); break;
            case W_param  : vert_data(vid).uvw[2] = vert(vid).z(); break;
            case UV_param : vert_data(vid).uvw[0] = vert(vid).x();
                            vert_data(vid).uvw[1] = vert(vid).y(); break;
            case UW_param : vert_data(vid).uvw[0] = vert(vid).x();
                            vert_data(vid).uvw[2] = vert(vid).z(); break;
            case VW_param : vert_data(vid).uvw[1] = vert(vid).y();
                            vert_data(vid).uvw[2] = vert(vid).z(); break;
            case UVW_param: vert_data(vid).uvw[0] = vert(vid).x();
                            vert_data(vid).uvw[1] = vert(vid).y();
                            vert_data(vid).uvw[2] = vert(vid).z(); break;
            default: assert(false);
        }
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
void AbstractMesh<M,V,E,P>::copy_uvw_to_xyz(const int mode)
{
    for(uint vid=0; vid<num_verts(); ++vid)
    {
        switch (mode)
        {
            case U_param  : vert(vid).x() = vert_data(vid).uvw[0]; break;
            case V_param  : vert(vid).y() = vert_data(vid).uvw[1]; break;
            case W_param  : vert(vid).z() = vert_data(vid).uvw[2]; break;
            case UV_param : vert(vid).x() = vert_data(vid).uvw[0];
                            vert(vid).y() = vert_data(vid).uvw[1]; break;
            case UW_param : vert(vid).x() = vert_data(vid).uvw[0];
                            vert(vid).z() = vert_data(vid).uvw[2]; break;
            case VW_param : vert(vid).y() = vert_data(vid).uvw[1];
                            vert(vid).z() = vert_data(vid).uvw[2]; break;
            case UVW_param: vert(vid).x() = vert_data(vid).uvw[0];
                            vert(vid).y() = vert_data(vid).uvw[1];
                            vert(vid).z() = vert_data(vid).uvw[2]; break;
            default: assert(false);
        }
    }
    update_bbox();
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
void AbstractMesh<M,V,E,P>::swap_xyz_uvw(const bool normals, const bool bbox)
{
    for(uint vid=0; vid<num_verts(); ++vid)
    {
        std::swap(vert(vid),vert_data(vid).uvw);
    }
    if (normals) update_normals();
    if (bbox)    update_bbox();
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
std::set<uint> AbstractMesh<M,V,E,P>::vert_n_ring(const uint vid, const uint n) const
{
    std::set<uint> active_set;
    std::set<uint> ring;

    active_set.insert(vid);
    for(uint i=0; i<n; ++i)
    {
        std::set<uint> next_active_set;

        for(uint curr : active_set)
        for(uint nbr  : adj_v2v(curr))
        {
            if (DOES_NOT_CONTAIN(ring,nbr) && nbr != vid) next_active_set.insert(nbr);
            ring.insert(nbr);
        }

        active_set = next_active_set;
    }
    return ring;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
bool AbstractMesh<M,V,E,P>::verts_are_adjacent(const uint vid0, const uint vid1) const
{
    for(uint nbr : adj_v2v(vid0)) if (vid1==nbr) return true;
    return false;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
uint AbstractMesh<M,V,E,P>::vert_opposite_to(const uint eid, const uint vid) const
{
    assert(this->edge_contains_vert(eid, vid));
    if (this->edge_vert_id(eid,0) != vid) return this->edge_vert_id(eid,0);
    else                                  return this->edge_vert_id(eid,1);
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
void AbstractMesh<M,V,E,P>::vert_weights(const uint vid, const int type, std::vector<std::pair<uint,double>> & wgts) const
{
    switch (type)
    {
        case UNIFORM : vert_weights_uniform(vid, wgts); return;
        default      : assert(false && "Vert weights not supported at this level of the hierarchy!");
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
void AbstractMesh<M,V,E,P>::vert_weights_uniform(const uint vid, std::vector<std::pair<uint,double>> & wgts) const
{
    wgts.clear();
    double w = 1.0; // / (double)nbrs.size(); // <= WARNING: makes the matrix non-symmetric!!!!!
    for(uint nbr : adj_v2v(vid))
    {
        wgts.push_back(std::make_pair(nbr,w));
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
bool AbstractMesh<M,V,E,P>::vert_is_local_min(const uint vid, const int tex_coord) const
{
    for(uint nbr : adj_v2v(vid))
    {
        switch (tex_coord)
        {
            case U_param : if (vert_data(nbr).uvw[0] < vert_data(vid).uvw[0]) return false; break;
            case V_param : if (vert_data(nbr).uvw[1] < vert_data(vid).uvw[1]) return false; break;
            case W_param : if (vert_data(nbr).uvw[2] < vert_data(vid).uvw[2]) return false; break;
            default: assert(false);
        }
    }
    return true;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
bool AbstractMesh<M,V,E,P>::vert_is_local_max(const uint vid, const int tex_coord) const
{
    for(uint nbr : adj_v2v(vid))
    {
        switch (tex_coord)
        {
            case U_param : if (vert_data(nbr).uvw[0] > vert_data(vid).uvw[0]) return false; break;
            case V_param : if (vert_data(nbr).uvw[1] > vert_data(vid).uvw[1]) return false; break;
            case W_param : if (vert_data(nbr).uvw[2] > vert_data(vid).uvw[2]) return false; break;
            default: assert(false);
        }
    }
    return true;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
uint AbstractMesh<M,V,E,P>::vert_valence(const uint vid) const
{
    return adj_v2v(vid).size();
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
uint AbstractMesh<M,V,E,P>::vert_shared(const uint eid0, const uint eid1) const
{
    uint e00 = edge_vert_id(eid0,0);
    uint e01 = edge_vert_id(eid0,1);
    uint e10 = edge_vert_id(eid1,0);
    uint e11 = edge_vert_id(eid1,1);
    if (e00 == e10 || e00 == e11) return e00;
    if (e01 == e10 || e01 == e11) return e01;
    assert(false);
    return 0; // warning killer
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
double AbstractMesh<M,V,E,P>::vert_min_uvw_value(const int tex_coord) const
{
    double min = inf_double;
    for(uint vid=0; vid<num_verts(); ++vid)
    {
        switch (tex_coord)
        {
            case U_param : min = std::min(min, vert_data(vid).uvw[0]); break;
            case V_param : min = std::min(min, vert_data(vid).uvw[1]); break;
            case W_param : min = std::min(min, vert_data(vid).uvw[2]); break;
            default: assert(false);
        }
    }
    return min;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
double AbstractMesh<M,V,E,P>::vert_max_uvw_value(const int tex_coord) const
{
    double max = -inf_double;
    for(uint vid=0; vid<num_verts(); ++vid)
    {
        switch (tex_coord)
        {
            case U_param : max = std::max(max, vert_data(vid).uvw[0]); break;
            case V_param : max = std::max(max, vert_data(vid).uvw[1]); break;
            case W_param : max = std::max(max, vert_data(vid).uvw[2]); break;
            default: assert(false);
        }
    }
    return max;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
void AbstractMesh<M,V,E,P>::vert_set_color(const Color & c)
{
    for(uint vid=0; vid<num_verts(); ++vid)
    {
        vert_data(vid).color = c;
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
void AbstractMesh<M,V,E,P>::vert_set_alpha(const float alpha)
{
    for(uint vid=0; vid<num_verts(); ++vid)
    {
        vert_data(vid).color.a = alpha;
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
void AbstractMesh<M,V,E,P>::vert_unmark_all()
{
    for(uint vid=0; vid<num_verts(); ++vid)
    {
        vert_data(vid).marked = false;
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
uint AbstractMesh<M,V,E,P>::edge_vert_id(const uint eid, const uint offset) const
{
    uint   eid_ptr = eid * 2;
    return edges.at(eid_ptr + offset);
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
int AbstractMesh<M,V,E,P>::edge_id(const uint vid0, const uint vid1) const
{
    assert(vid0 != vid1);
    for(uint eid : adj_v2e(vid0))
    {
        if (edge_contains_vert(eid, vid0) && edge_contains_vert(eid, vid1))
        {
            return eid;
        }
    }
    return -1;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
uint AbstractMesh<M,V,E,P>::edge_valence(const uint eid) const
{
    return this->adj_e2p(eid).size();
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
vec3d AbstractMesh<M,V,E,P>::edge_vert(const uint eid, const uint offset) const
{
    return vert(edge_vert_id(eid,offset));
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
bool AbstractMesh<M,V,E,P>::edge_contains_vert(const uint eid, const uint vid) const
{
    if (edge_vert_id(eid,0) == vid) return true;
    if (edge_vert_id(eid,1) == vid) return true;
    return false;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
bool AbstractMesh<M,V,E,P>::edges_are_adjacent(const uint eid0, const uint eid1) const
{
    if (edge_vert_id(eid0,0)==edge_vert_id(eid1,0)) return true;
    if (edge_vert_id(eid0,0)==edge_vert_id(eid1,1)) return true;
    if (edge_vert_id(eid0,1)==edge_vert_id(eid1,0)) return true;
    if (edge_vert_id(eid0,1)==edge_vert_id(eid1,1)) return true;
    return false;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
vec3d AbstractMesh<M,V,E,P>::edge_sample_at(const uint eid, const double lambda) const
{
    return ((1.0-lambda)*edge_vert(eid,0) + lambda*edge_vert(eid,1));
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
double AbstractMesh<M,V,E,P>::edge_length(const uint eid) const
{
    return edge_vec(eid).length();
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
vec3d AbstractMesh<M,V,E,P>::edge_vec(const uint eid, const bool normalized) const
{
    vec3d e(edge_vert(eid,1) - edge_vert(eid,0));
    if(normalized) e.normalize();
    return e;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
double AbstractMesh<M,V,E,P>::edge_avg_length() const
{
    double avg = 0;
    for(uint eid=0; eid<num_edges(); ++eid) avg += edge_length(eid);
    if (num_edges() > 0) avg/=static_cast<double>(num_edges());
    return avg;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
double AbstractMesh<M,V,E,P>::edge_max_length() const
{
    double max = 0;
    for(uint eid=0; eid<num_edges(); ++eid) max = std::max(max, edge_length(eid));
    return max;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
double AbstractMesh<M,V,E,P>::edge_min_length() const
{
    double min = 0;
    for(uint eid=0; eid<num_edges(); ++eid) min = std::min(min,edge_length(eid));
    return min;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
void AbstractMesh<M,V,E,P>::edge_set_color(const Color & c)
{
    for(uint eid=0; eid<num_edges(); ++eid)
    {
        edge_data(eid).color = c;
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
void AbstractMesh<M,V,E,P>::edge_unmark_all()
{
    for(uint eid=0; eid<num_edges(); ++eid)
    {
        edge_data(eid).marked = false;
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
void AbstractMesh<M,V,E,P>::edge_set_alpha(const float alpha)
{
    for(uint eid=0; eid<num_edges(); ++eid)
    {
        edge_data(eid).color.a = alpha;
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
uint AbstractMesh<M,V,E,P>::poly_vert_id(const uint pid, const uint offset) const
{
    return adj_p2v(pid).at(offset);
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
vec3d AbstractMesh<M,V,E,P>::poly_centroid(const uint pid) const
{
    vec3d c(0,0,0);
    for(uint vid : adj_p2v(pid)) c += vert(vid);
    c /= static_cast<double>(verts_per_poly(pid));
    return c;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
vec3d AbstractMesh<M,V,E,P>::poly_sample_at(const uint pid, const std::vector<double> & bary) const
{
    assert(bary.size() == verts_per_poly(pid));
    vec3d p(0,0,0);
    for(uint off=0; off<verts_per_poly(pid); ++off)
    {
        p += bary.at(off) * poly_vert(pid,off);
    }
    return p;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
double AbstractMesh<M,V,E,P>::poly_sample_param_at(const uint pid, const std::vector<double> & bary, const int tex_coord) const
{
    assert(bary.size() == verts_per_poly(pid));
    double val = 0;
    for(uint off=0; off<verts_per_poly(pid); ++off)
    {
        switch(tex_coord)
        {
            case U_param : val += bary.at(off) * this->vert_data(this->poly_vert_id(pid,off)).uvw[0]; break;
            case V_param : val += bary.at(off) * this->vert_data(this->poly_vert_id(pid,off)).uvw[1]; break;
            case W_param : val += bary.at(off) * this->vert_data(this->poly_vert_id(pid,off)).uvw[2]; break;
            default: assert(false);
        }
    }
    return val;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
vec3d AbstractMesh<M,V,E,P>::poly_vert(const uint pid, const uint offset) const
{
    return vert(poly_vert_id(pid,offset));
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
std::vector<vec3d> AbstractMesh<M,V,E,P>::poly_verts(const uint pid) const
{
    uint nv = this->verts_per_poly(pid);
    std::vector<vec3d> p_list(nv);
    for(uint off=0; off<nv; ++off) p_list.at(off) = this->poly_vert(pid,off);
    return p_list;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
std::vector<uint> AbstractMesh<M,V,E,P>::poly_verts_id(const uint pid, const bool sort_by_vid) const
{
    if(sort_by_vid)
    {
        std::vector<uint> v_list = this->adj_p2v(pid);
        SORT_VEC(v_list);
        return v_list;
    }
    return this->adj_p2v(pid);
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
std::vector<uint> AbstractMesh<M,V,E,P>::poly_v2v(const uint pid, const uint vid) const
{
    assert(this->poly_contains_vert(pid,vid));
    std::vector<uint> verts;
    for(uint nbr : this->adj_v2v(vid))
    {
        if(this->poly_contains_vert(pid,nbr)) verts.push_back(nbr);
    }
    return verts;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
std::vector<uint> AbstractMesh<M,V,E,P>::poly_v2e(const uint pid, const uint vid) const
{
    assert(this->poly_contains_vert(pid,vid));
    std::vector<uint> edges;
    for(uint eid : this->adj_v2e(vid))
    {
        if(this->poly_contains_edge(pid,eid)) edges.push_back(eid);
    }
    return edges;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
uint AbstractMesh<M,V,E,P>::poly_vert_valence(const uint pid, const uint vid) const
{
    return this->poly_v2v(pid,vid).size();
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
uint AbstractMesh<M,V,E,P>::poly_edge_id(const uint fid, const uint vid0, const uint vid1) const
{
    assert(poly_contains_vert(fid,vid0));
    assert(poly_contains_vert(fid,vid1));

    for(uint eid : adj_p2e(fid))
    {
        if (edge_contains_vert(eid,vid0) && edge_contains_vert(eid,vid1)) return eid;
    }

    assert(false);
    return 0; // warning killer
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
bool AbstractMesh<M,V,E,P>::poly_contains_edge(const uint pid, const uint eid) const
{
    for(uint e : adj_p2e(pid)) if (e == eid) return true;
    return false;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
bool AbstractMesh<M,V,E,P>::poly_contains_edge(const uint pid, const uint vid0, const uint vid1) const
{
    for(uint eid : adj_p2e(pid))
    {
        if (edge_contains_vert(eid, vid0) &&
            edge_contains_vert(eid, vid1))
        {
            return true;
        }
    }
    return false;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
void AbstractMesh<M,V,E,P>::poly_show_all()
{
    for(uint pid=0; pid<num_polys(); ++pid)
    {
        poly_data(pid).visible = true;
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
void AbstractMesh<M,V,E,P>::center_bbox()
{
    vec3d center = bb.center();
    for(uint vid=0; vid<num_verts(); ++vid) vert(vid) -= center;
    bb.min -= center;
    bb.max -= center;
    update_bbox();
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
void AbstractMesh<M,V,E,P>::poly_set_color(const Color & c)
{
    for(uint pid=0; pid<num_polys(); ++pid)
    {
        poly_data(pid).color = c;
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
void AbstractMesh<M,V,E,P>::poly_set_alpha(const float alpha)
{
    for(uint pid=0; pid<num_polys(); ++pid)
    {
        poly_data(pid).color.a = alpha;
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
void AbstractMesh<M,V,E,P>::poly_color_wrt_label(const float s, const float v) // s => saturation, v => value in HSV color space
{
    std::map<int,uint> l_map;
    for(uint pid=0; pid<this->num_polys(); ++pid)
    {
        int l = this->poly_data(pid).label;
        if (DOES_NOT_CONTAIN(l_map,l)) l_map[l] = l_map.size();
    }
    uint n_labels = l_map.size();
    for(uint pid=0; pid<this->num_polys(); ++pid)
    {
        this->poly_data(pid).color = Color::scatter(n_labels,l_map.at(this->poly_data(pid).label), s, v);
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
void AbstractMesh<M,V,E,P>::poly_label_wrt_color()
{
    std::map<Color,int> colormap;
    for(uint pid=0; pid<this->num_polys(); ++pid)
    {
        const Color & c = this->poly_data(pid).color;
        if (DOES_NOT_CONTAIN(colormap,c)) colormap[c] = colormap.size();
    }
    for(uint pid=0; pid<this->num_polys(); ++pid)
    {
        this->poly_data(pid).label = colormap.at(this->poly_data(pid).color);
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
bool AbstractMesh<M,V,E,P>::poly_contains_vert(const uint pid, const uint vid) const
{
    for(uint v : adj_p2v(pid)) if(v == vid) return true;
    return false;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
void AbstractMesh<M,V,E,P>::poly_unmark_all()
{
    for(uint pid=0; pid<num_polys(); ++pid)
    {
        poly_data(pid).marked = false;
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
bool AbstractMesh<M,V,E,P>::polys_are_colored() const
{
    std::set<Color> colors;
    for(uint pid=0; pid<num_polys(); ++pid)
    {
        colors.insert(poly_data(pid).color);
    }
    if (colors.size()>1) return true;
    return false;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
bool AbstractMesh<M,V,E,P>::polys_are_labeled() const
{
    std::set<int> labels;
    for(uint pid=0; pid<num_polys(); ++pid)
    {
        labels.insert(poly_data(pid).label);
    }
    if (labels.size()>1) return true;
    return false;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
void AbstractMesh<M,V,E,P>::poly_apply_labels(const std::vector<int> & labels)
{
    assert(labels.size() == this->num_polys());
    for(uint pid=0; pid<num_polys(); ++pid)
    {
        poly_data(pid).label = labels.at(pid);
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
int AbstractMesh<M,V,E,P>::poly_id(const std::vector<uint> & vids) const
{
    std::vector<uint> query = SORT_VEC(vids);

    uint vid = vids.front();
    for(uint pid : this->adj_v2p(vid))
    {
        if(this->poly_verts_id(pid,true)==query) return pid;
    }
    return -1;
}

}
