//
//  Mesh.h
//  Chilli Source
//  Created by Scott Downie on 06/10/2010.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#ifndef _CHILLISOURCE_RENDERING_MODEL_MESH_H_
#define _CHILLISOURCE_RENDERING_MODEL_MESH_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>
#include <ChilliSource/Core/Resource/Resource.h>
#include <ChilliSource/Rendering/Model/Skeleton.h>

namespace ChilliSource
{
    //---------------------------------------------------------------------
    /// Holds the sub meshes and skeleton that make up a mesh resource.
    /// Provides methods for building and rendering the mesh
    ///
    /// @author S Downie
    //---------------------------------------------------------------------
    class Mesh final : public Resource
    {
    public:
        
        CS_DECLARE_NAMEDTYPE(Mesh);
        
        //---------------------------------------------------------------------
        /// @author S Downie
        ///
        /// @param Interface ID type
        ///
        /// @return Whether this object is of given type
        //---------------------------------------------------------------------
        bool IsA(InterfaceIDType in_interfaceId) const override;
        //----------------------------------------------------------------------------
        /// Build the mesh from the given description. This will build the mesh buffers
        /// and sub meshes
        ///
        /// @author Ian Copland
        ///
        /// @param Mesh descriptor
        //----------------------------------------------------------------------------
        bool Build(const MeshDescriptor& in_meshDesc);
        //-----------------------------------------------------------------
        /// @author S Downie
        ///
        /// @return Axis-aligned bounding box
        //-----------------------------------------------------------------
        const AABB& GetAABB() const;
        //-----------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return Number of verts in this meshes sub-meshes.
        //-----------------------------------------------------------------
        u32 GetNumVerts() const;
        //-----------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return Number of indices in this this meshes sub-meshes.
        //-----------------------------------------------------------------
        u32 GetNumIndices() const;
        //-----------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return The skeleton
        //-----------------------------------------------------------------
        const Skeleton* GetSkeleton() const;
  
    private:
        friend class ResourcePool;
        //---------------------------------------------------------------------
        /// Factory method for creating mesh resource instance. Only accessed
        /// by the resource pool
        ///
        /// @author S Downie
        ///
        /// @return Ownership of new instance
        //---------------------------------------------------------------------
        static MeshUPtr Create();
        //-----------------------------------------------------------------
        /// Private constructor enforces the use of the factory method
        ///
        /// @author S Downie
        //-----------------------------------------------------------------
        Mesh();
        //-----------------------------------------------------------------
        /// Creates the AABB for this mesh from minimum and maximum bounds
        /// vectors
        ///
        /// @author Ian Copland
        ///
        /// @param the minimum bounds
        /// @param the maximum bounds.
        //-----------------------------------------------------------------
        void SetBounds(const Vector3& in_minBounds, const Vector3& in_maxBounds);
        //-----------------------------------------------------------------
        /// Calculates the internal vertex and index counts. This should be
        /// called after filling all of the submeshes with data.
        ///
        /// @author Ian Copland
        //-----------------------------------------------------------------
        void CalcVertexAndIndexCounts();
        
        //Only model loaders can alter the mesh construct
        friend class CSModelProvider;
        friend class MeshBatch;
        
    private:
        
        SkeletonUPtr m_skeleton;
        
        AABB m_aabb;
        u32 m_totalNumVerts;
        u32 m_totalNumIndices;
    };
}

#endif