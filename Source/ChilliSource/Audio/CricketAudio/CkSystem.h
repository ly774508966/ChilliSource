//
//  CkSystem.h
//  Chilli Source
//  Created by Ian Copland on 30/12/2014.
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

#ifndef _CHILLISOURCE_AUDIO_CRICKETAUDIO_CKSYSTEM_H_
#define _CHILLISOURCE_AUDIO_CRICKETAUDIO_CKSYSTEM_H_

#include <ChilliSource/ChilliSource.h>

#include <ChilliSource/Core/System/AppSystem.h>

namespace ChilliSource 
{
	namespace Audio
	{
		//------------------------------------------------------------------------------
		/// The Cricket Audio system. This manages the underlying cricket audio system,
		/// initialising it and destroying when needed. 
		///
		/// Cricket Technology has kindly allows us to include the Cricket Audio SDK 
		/// in the engine under the free license, which can be found at the following
		/// link: http://www.crickettechnology.com/free_license. To comply with the
		/// license there are two things you must do:
		///
		/// - You must display the following somewhere in your application:
		///   
		///                       Built with Cricket Audio
		///                       www.crickettechnology.com
		///
		/// - You must let Cricket Technology know when you release your game, so they
		///   can include you in their customer list.
		///
		/// This, and the other Cricket Audio systems, are not added to Application by 
		/// default. If you intend to use Cricket Audio you will need to add CkSystem,
		/// and CkBankProvider during the Application::AddSystems() life cycle event.
        /// You will also need to add CkAudioPlayer during the State::AddSystems() life
        /// cycle event for any states in which you wish to use it.
		///
		/// @author Ian Copland
		//------------------------------------------------------------------------------
		class CkSystem final : public Core::AppSystem
		{
		public:
			CS_DECLARE_NAMEDTYPE(CkSystem);
			//------------------------------------------------------------------------------
			/// Allows querying of whether or not this system implements the interface
			/// described by the given interface Id. Typically this is not called directly
			/// as the templated equivalent IsA<Interface>() is preferred.
			///
			/// @author Ian Copland
			///
			/// @param The interface Id.
			///
			/// @return Whether or not the interface is implmented.
			//------------------------------------------------------------------------------
			bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
		private:
			friend class CSCore::Application;
			friend class CkAudio;
			//------------------------------------------------------------------------------
			/// A factory method for creating new instances of the Cricket Audio System.
			///
			/// @author Ian Copland
			//------------------------------------------------------------------------------
			static CkSystemUPtr Create();
			//------------------------------------------------------------------------------
			/// Default constructor. Declared private to ensure this can only be created
			/// through Application::CreateSystem<CkSystem>().
			///
			/// @author Ian Copland
			//------------------------------------------------------------------------------
			CkSystem() = default;
			//------------------------------------------------------------------------------
			/// Registers a CkAudio with the system so that it receives update events.
			///
			/// @author Ian Copland
			///
			/// @param The pointer to the CkAudio.
			//------------------------------------------------------------------------------
			void Register(CkAudio* in_ckAudio);
			//------------------------------------------------------------------------------
			/// De-registers a CkAudio from the system so that it no longer receives update
			/// events.
			///
			/// @author Ian Copland
			///
			/// @param The pointer to the CkAudio.
			//------------------------------------------------------------------------------
			void Deregister(CkAudio* in_ckAudio);
			//------------------------------------------------------------------------------
			/// Initialises the Cricket Audio system.
			///
			/// @author Ian Copland
			//------------------------------------------------------------------------------
			void OnInit() override;
			//------------------------------------------------------------------------------
			/// Resumes the Cricket Audio system, resuming any audio that was paused during
			/// a suspend.
			///
			/// @author Ian Copland
			//------------------------------------------------------------------------------
			void OnResume() override;
			//------------------------------------------------------------------------------
			/// Updates the Cricket Audio system.
			///
			/// @author Ian Copland
			///
			/// @param The delta time.
			//------------------------------------------------------------------------------
			void OnUpdate(f32 in_deltaTime) override;
			//------------------------------------------------------------------------------
			/// Suspends the cricket audio system. This ensures that all audio is currectly
			/// paused while the app is not active.
			///
			/// @author Ian Copland
			//------------------------------------------------------------------------------
			void OnSuspend() override;
			//------------------------------------------------------------------------------
			/// Shuts down the cricket audio system.
			///
			/// @author Ian Copland
			///------------------------------------------------------------------------------
			void OnDestroy() override;

			std::vector<CkAudio*> m_ckAudioList;
		};
	}
}

#endif
