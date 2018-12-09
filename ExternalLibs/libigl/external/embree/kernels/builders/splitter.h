// ======================================================================== //
// Copyright 2009-2017 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#pragma once

#include "../common/scene.h"
#include "../common/primref.h"

namespace embree
{
  namespace isa
  {
    template<size_t N>
    __forceinline void splitPolygon(const BBox3fa& bounds, 
                                    const size_t dim, 
                                    const float pos, 
                                    const Vec3fa (&v)[N+1],
                                    const Vec3fa (&inv_length)[N],
                                    BBox3fa& left_o, 
                                    BBox3fa& right_o)
    {
      BBox3fa left = empty, right = empty;
      /* clip triangle to left and right box by processing all edges */
      for (size_t i=0; i<N; i++)
      {
        const Vec3fa &v0 = v[i]; 
        const Vec3fa &v1 = v[i+1]; 
        const float v0d = v0[dim];
        const float v1d = v1[dim];
        
        if (v0d <= pos) left. extend(v0); // this point is on left side
        if (v0d >= pos) right.extend(v0); // this point is on right side
        
        if ((v0d < pos && pos < v1d) || (v1d < pos && pos < v0d)) // the edge crosses the splitting location
        {
          assert((v1d-v0d) != 0.0f);
          const Vec3fa c = madd(Vec3fa((pos-v0d)*inv_length[i][dim]),v1-v0,v0);
          left.extend(c);
          right.extend(c);
        }
      }
      
      /* clip against current bounds */
      left_o  = intersect(left,bounds);
      right_o = intersect(right,bounds);
    }
    
    template<size_t N>
      __forceinline void splitPolygon(const PrimRef& prim, 
                                      const size_t dim, 
                                      const float pos, 
                                      const Vec3fa (&v)[N+1],
                                      PrimRef& left_o, 
                                      PrimRef& right_o)
    {
      BBox3fa left = empty, right = empty;
      for (size_t i=0; i<N; i++)
      {
        const Vec3fa &v0 = v[i]; 
        const Vec3fa &v1 = v[i+1]; 
        const float v0d = v0[dim];
        const float v1d = v1[dim];
        
        if (v0d <= pos) left. extend(v0); // this point is on left side
        if (v0d >= pos) right.extend(v0); // this point is on right side
        
        if ((v0d < pos && pos < v1d) || (v1d < pos && pos < v0d)) // the edge crosses the splitting location
        {
          assert((v1d-v0d) != 0.0f);
          const float inv_length = 1.0f/(v1d-v0d);
          const Vec3fa c = madd(Vec3fa((pos-v0d)*inv_length),v1-v0,v0);
          left.extend(c);
          right.extend(c);
        }
      }
      
      /* clip against current bounds */
      new (&left_o ) PrimRef(intersect(left ,prim.bounds()),prim.geomID(), prim.primID());
      new (&right_o) PrimRef(intersect(right,prim.bounds()),prim.geomID(), prim.primID());
    }
    
    struct TriangleSplitter
    {
      __forceinline TriangleSplitter(const Scene* scene, const PrimRef& prim)
      {
        const TriangleMesh* mesh = (const TriangleMesh*) scene->get(prim.geomID() & 0x00FFFFFF );  
        TriangleMesh::Triangle tri = mesh->triangle(prim.primID());
        v[0] = mesh->vertex(tri.v[0]);
        v[1] = mesh->vertex(tri.v[1]);
        v[2] = mesh->vertex(tri.v[2]);
        v[3] = mesh->vertex(tri.v[0]);
        inv_length[0] = Vec3fa(1.0f) / (v[1]-v[0]);
        inv_length[1] = Vec3fa(1.0f) / (v[2]-v[1]);
        inv_length[2] = Vec3fa(1.0f) / (v[0]-v[2]);
      }
      
      __forceinline void operator() (const PrimRef& prim, const size_t dim, const float pos, PrimRef& left_o, PrimRef& right_o) const {
        splitPolygon<3>(prim,dim,pos,v,left_o,right_o);
      }
      
      __forceinline void operator() (const BBox3fa& prim, const size_t dim, const float pos, BBox3fa& left_o, BBox3fa& right_o) const {
        splitPolygon<3>(prim,dim,pos,v,inv_length,left_o,right_o);
      }
      
    private:
      Vec3fa v[4];
      Vec3fa inv_length[3];
    };
    
    struct TriangleSplitterFactory
    {
      __forceinline TriangleSplitterFactory(const Scene* scene)
        : scene(scene) {}
      
      __forceinline TriangleSplitter operator() (const PrimRef& prim) const {
        return TriangleSplitter(scene,prim);
      }
      
    private:
      const Scene* scene;
    };
    
    struct QuadSplitter
    {
      __forceinline QuadSplitter(const Scene* scene, const PrimRef& prim)
      {
        const QuadMesh* mesh = (const QuadMesh*) scene->get(prim.geomID() & 0x00FFFFFF );  
        QuadMesh::Quad quad = mesh->quad(prim.primID());
        v[0] = mesh->vertex(quad.v[0]);
        v[1] = mesh->vertex(quad.v[1]);
        v[2] = mesh->vertex(quad.v[2]);
        v[3] = mesh->vertex(quad.v[3]);
        v[4] = mesh->vertex(quad.v[0]);
        inv_length[0] = Vec3fa(1.0f) / (v[1]-v[0]);
        inv_length[1] = Vec3fa(1.0f) / (v[2]-v[1]);
        inv_length[2] = Vec3fa(1.0f) / (v[3]-v[2]);
        inv_length[3] = Vec3fa(1.0f) / (v[0]-v[3]);
      }
      
      __forceinline void operator() (const PrimRef& prim, const size_t dim, const float pos, PrimRef& left_o, PrimRef& right_o) const {
        splitPolygon<4>(prim,dim,pos,v,left_o,right_o);
      }
      
      __forceinline void operator() (const BBox3fa& prim, const size_t dim, const float pos, BBox3fa& left_o, BBox3fa& right_o) const {
        splitPolygon<4>(prim,dim,pos,v,inv_length,left_o,right_o);
      }
      
    private:
      Vec3fa v[5];
      Vec3fa inv_length[4];
    };
    
    struct QuadSplitterFactory
    {
      __forceinline QuadSplitterFactory(const Scene* scene)
        : scene(scene) {}
      
      __forceinline QuadSplitter operator() (const PrimRef& prim) const {
        return QuadSplitter(scene,prim);
      }
      
    private:
      const Scene* scene;
    };
  }
}

