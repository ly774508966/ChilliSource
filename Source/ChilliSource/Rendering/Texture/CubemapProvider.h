//
//  CubemapProvider.h
//  Chilli Source
//  Created by Scott Downie on 17/04/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#ifndef _CHILLISOURCE_RENDERING_TEXTURE_CUBEMAPPROVIDER_H_
#define _CHILLISOURCE_RENDERING_TEXTURE_CUBEMAPPROVIDER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/ResourceProvider.h>

namespace ChilliSource
{
	namespace Rendering
	{
        //-------------------------------------------------------
        /// Factory loader for creating cubemap resources
        /// from files. This loader is responsible for loading the
        /// image data for the 6 faces and building the texture.
        ///
        /// @author S Downie
        //-------------------------------------------------------
		class CubemapProvider final : public Core::ResourceProvider
		{
		public:
            
            CS_DECLARE_NAMEDTYPE(CubemapProvider);
            
            //-------------------------------------------------------
            /// Factory method
            ///
            /// @author S Downie
            ///
            /// @return New provider with ownership transferred
            //-------------------------------------------------------
            static CubemapProviderUPtr Create();
			//-------------------------------------------------------------------------
			/// @author S Downie
			///
			/// @param Interface to compare
            ///
			/// @return Whether the object implements the given interface
			//-------------------------------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceId) const override;
			//----------------------------------------------------------------------------
			/// @author S Downie
			///
			/// @return The resource type this provider loads
			//----------------------------------------------------------------------------
			Core::InterfaceIDType GetResourceType() const override;
			//----------------------------------------------------------------------------
			/// @author S Downie
			///
			/// @param Extension to compare
            ///
			/// @return Whether the object can create a resource with the given extension
			//----------------------------------------------------------------------------
			bool CanCreateResourceWithFileExtension(const std::string& in_extension) const override;
            //----------------------------------------------------------------------------
			/// Loads the 6 face images and generate the cubemap via the output resource.
            /// Check the resource load state for success or failure.
            ///
            /// Note: The path given is the path to a theoretical image and the face
            /// indices are appended i.e. myCubemap.png is actually myCubemap1.png,
            /// myCubemap2.png, etc.
            ///
            /// @author S Downie
			///
			/// @param Location to load from
			/// @param Filename
			/// @param [Out] Resource object
			//----------------------------------------------------------------------------
			void CreateResourceFromFile(Core::StorageLocation in_location, const std::string& in_filePath, Core::ResourceSPtr& out_resource) override;
            //----------------------------------------------------------------------------
			/// Loads the 6 face images on a background thread and generate the cubemap via the output resource.
            /// Delegate is called on completion. Check the resource load state for success or failure
            ///
            /// Note: The path given is the path to a theoretical image and the face
            /// indices are appended i.e. myCubemap.png is actually myCubemap1.png,
            /// myCubemap2.png, etc.
            ///
            /// @author S Downie
			///
			/// @param Location to load from
			/// @param Filename
            /// @param Completion delegate
			/// @param [Out] Resource object
			//----------------------------------------------------------------------------
			void CreateResourceFromFileAsync(Core::StorageLocation in_location, const std::string& in_filePath, const Core::ResourceProvider::AsyncLoadDelegate& in_delegate, Core::ResourceSPtr& out_resource) override;
			
		private:
            //----------------------------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------
            CubemapProvider() = default;
            //----------------------------------------------------------------------------
            /// Called when the system is initialised. Retrieves the image providers
            /// to delegate image loading to
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------
            void OnInit() override;
            //----------------------------------------------------------------------------
            /// Called when the system is destroyed.
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------
            void OnDestroy() override;
            //----------------------------------------------------------------------------
			/// Does the heavy lifting for the 2 create methods. The building of the cubemap
            /// is always done on the main thread
            ///
            /// @author S Downie
			///
			/// @param Location to load from
			/// @param Filename
            /// @param Completion delegate
			/// @param [Out] Resource object
			//----------------------------------------------------------------------------
			void LoadCubemap(Core::StorageLocation in_location, const std::string& in_filePath, const Core::ResourceProvider::AsyncLoadDelegate& in_delegate, Core::ResourceSPtr& out_resource);
            
        private:
            
            std::vector<Core::ResourceProvider*> m_imageProviders;
		};
	}
}

#endif
