//
//  FMOD2DAudioComponent.cpp
//  moFloTest
//
//  Created by Scott Downie on 12/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Audio/Base/AudioSystem.h>

#include <ChilliSource/Backend/Audio/FMOD/3D/AudioComponent.h>

namespace ChilliSource
{
    namespace FMOD
    {
        //----------------------------------------------------
		/// FMOD - On Channel Audio Complete Callback
		///
		/// Triggered when a channel has finished playing
		/// audio. The resource instance is then pulled
		/// from the channel and the resource notified
		//----------------------------------------------------
		FMOD_RESULT F_CALLBACK OnChannelAudioCompleteCallback(FMOD_CHANNEL* inpChannel, FMOD_CHANNEL_CALLBACKTYPE type, void *commanddata1, void *commanddata2)
		{
			switch(type)
			{
				case FMOD_CHANNEL_CALLBACKTYPE_END:
				{
					//Convert from the dodgy signature to our actual channel object
					::FMOD::Channel* pChannel = (::FMOD::Channel*)inpChannel;
					
					//Grab the instance pointer so we can notify the correct resource 
					AudioComponent* pAudioResource = nullptr;
					pChannel->getUserData((void**)&pAudioResource);
					
					//Now trigger the real callback
                    if(pAudioResource)
                    {
                        pAudioResource->OnPlaybackComplete();
                    }
				}
					break;
				default:
					break;
			}
			
			return FMOD_OK;
		}
        //---------------------------------------------------------------------
		/// Constructor
		///
		/// Default
		//---------------------------------------------------------------------
		AudioComponent::AudioComponent(Audio::AudioSystem* inpOwningSystem) : Audio::AudioComponent(inpOwningSystem), mpFMODChannel(nullptr)
        {
        }
        //---------------------------------------------------------------------
		/// Play
		///
		/// Begin playback of the sound
		//---------------------------------------------------------------------
		void AudioComponent::Play()
		{
			//Check if we are simply resuming or whether we are starting from scratch
			if(mpFMODChannel)
			{
				bool bIsPaused = false;
				mpFMODChannel->getPaused(&bIsPaused);
				
				if(bIsPaused)
				{
					mpFMODChannel->setPaused(false);
					return;
				}
			}
			
			//We have yet to play this sound
			mpOwningSystem->PlaySound(this);
		}
		//---------------------------------------------------------------------
		/// Pause
		///
		/// Stop playback of the sound
		//---------------------------------------------------------------------
		void AudioComponent::Pause()
		{
			if(mpFMODChannel)
			{
				mpFMODChannel->setPaused(true);
			}
		}
		//---------------------------------------------------------------------
		/// Stop
		///
		/// Stop playback of the sound
		//---------------------------------------------------------------------
		void AudioComponent::Stop()
		{
			if (mpFMODChannel)
            {
				mpFMODChannel->stop();
			}
		}
		//---------------------------------------------------------------------
		/// Set Looping
		///
		/// @param Whether to loop the sound or not
		//---------------------------------------------------------------------
		void AudioComponent::SetLooping(bool inbShouldLoop)
		{
            mpAudioSource->SetLooping(inbShouldLoop);
		}
		//---------------------------------------------------------------------
		/// Set Channel
		///
		/// @param A handle to the channel that is playing us.
		//---------------------------------------------------------------------
		void AudioComponent::SetChannel(::FMOD::Channel* inpChannel)
		{
            if (mpFMODChannel != nullptr)
            {
                mpFMODChannel->setUserData(nullptr);
                mpFMODChannel->setCallback(nullptr);
            }
            
			mpFMODChannel = inpChannel;
			
            if (mpFMODChannel != nullptr)
            {
                //Set the initial volume
                SetVolume(Audio::kfDefaultAudioVolume);
                
                //Register for sound ending callbacks
                //As FMOD uses c-style callbacks we give the channel our instance pointer
                mpFMODChannel->setUserData(this);
                mpFMODChannel->setCallback(&OnChannelAudioCompleteCallback);
            }
		}
		//---------------------------------------------------------------------
		/// On Playback Complete
		///
		/// Triggered by FMOD when the channel has finished with us 
		//---------------------------------------------------------------------
		void AudioComponent::OnPlaybackComplete()
		{
            mOnPlaybackCompleteEvent.NotifyConnections(this);
			SetChannel(nullptr);
		}
		//---------------------------------------------------------------------
		/// Set Volume
		///
		/// @param Volume level in normalised range [0, 1]
		//---------------------------------------------------------------------
		void AudioComponent::SetVolume(f32 infVolume)
		{
			if(mpFMODChannel)
			{
                f32 fVolume = Core::MathUtils::Clamp(infVolume, 0.0f, 1.0f);
                mfCurrentVolume = fVolume;
				
				if(mpAudioSource && mpAudioSource->IsStreamed())
				{
					mpFMODChannel->setVolume(fVolume * mpOwningSystem->GetMasterStreamVolume());
				}
				else if(mpAudioSource)
				{
					mpFMODChannel->setVolume(fVolume * mpOwningSystem->GetMasterEffectVolume());
				}
            }
		}
        //---------------------------------------------------------------------
		/// Set Frequency
		///
		/// @param Frequency
		//---------------------------------------------------------------------
		void AudioComponent::SetFrequency(f32 infFreq)
		{
			if(mpFMODChannel)
			{
                mfCurrentFrequency = infFreq;
				
				if(mpAudioSource)
				{
					mpFMODChannel->setFrequency(mfCurrentFrequency);
				}
            }
		}
        //---------------------------------------------------------------------
        /// Get Frequency
        ///
        /// @return Frequency
        //---------------------------------------------------------------------
        f32 AudioComponent::GetFrequency() const
        {
            if (mpFMODChannel)
        	{
        		f32 fFreq;
				mpFMODChannel->getFrequency(&fFreq);
				return fFreq;
        	}
			return mfCurrentFrequency;
        }
		//---------------------------------------------------------------------
		/// Set 3D Location
		///
		/// Set the position of the audio source in 3D space
		/// @param 3D position vector
		/// @param 3D velocity vector
		//---------------------------------------------------------------------
		void AudioComponent::Set3DLocation(Core::Vector3& invPosition, Core::Vector3& invVelocity)
		{
			if(mpFMODChannel)
			{
				mpFMODChannel->set3DAttributes(reinterpret_cast<FMOD_VECTOR*>(&invPosition), reinterpret_cast<FMOD_VECTOR*>(&invVelocity));
			}
		}
		//---------------------------------------------------------------------
		/// Is Playing
		///
		/// @return Whether the sound is playing or not
		//---------------------------------------------------------------------
		bool AudioComponent::IsPlaying() const
		{
            bool bIsPlaying = false;
			//No channel. No sound
			if(mpFMODChannel)
			{
                mpFMODChannel->isPlaying(&bIsPlaying);
            }
            
            return bIsPlaying;
		}
		AudioComponent::~AudioComponent()
		{
            SetChannel(nullptr);
        }
    }
}