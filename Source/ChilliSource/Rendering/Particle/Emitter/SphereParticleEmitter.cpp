//
//  SphereParticleEmitter.cpp
//  Chilli Source
//  Created by Ian Copland on 19/10/2014.
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

#include <ChilliSource/Rendering/Particle/Emitter/SphereParticleEmitter.h>

#include <ChilliSource/Core/Math/Random.h>
#include <ChilliSource/Rendering/Particle/ParticleEffect.h>
#include <ChilliSource/Rendering/Particle/Emitter/SphereParticleEmitterDef.h>

namespace ChilliSource
{
	namespace Rendering
	{
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		SphereParticleEmitter::SphereParticleEmitter(const CSCore::Entity* in_entity, const ParticleEmitterDef* in_particleEmitter, Core::dynamic_array<Particle>* in_particleArray)
			: ParticleEmitter(in_entity, in_particleEmitter, in_particleArray)
		{
			//Only the sphere emitter def can create this, so this is safe.
			m_sphereParticleEmitterDef = static_cast<const SphereParticleEmitterDef*>(in_particleEmitter);
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		void SphereParticleEmitter::GenerateEmission(f32 in_emissionTime, CSCore::Vector3& out_position, CSCore::Vector3& out_direction)
		{
			const f32 normalisedEmissionTime = in_emissionTime / m_sphereParticleEmitterDef->GetParticleEffect()->GetDuration();
			f32 radius = m_sphereParticleEmitterDef->GetRadiusProperty()->GenerateValue(normalisedEmissionTime);

			//calculate the position.
			switch (m_sphereParticleEmitterDef->GetEmitFromType())
			{
			case SphereParticleEmitterDef::EmitFromType::k_inside:
				out_position = Random::GenerateDirection3D<f32>() * Random::GenerateReal(0.0f, radius);
				break;
			case SphereParticleEmitterDef::EmitFromType::k_surface:
				out_position = Random::GenerateDirection3D<f32>() * radius;
				break;
			default:
				CS_LOG_FATAL("Invalid 'Emit From' type.");
				break;
			}

			//calculate the direction.
			switch (m_sphereParticleEmitterDef->GetEmitDirectionType())
			{
			case SphereParticleEmitterDef::EmitDirectionType::k_random:
				out_direction = Random::GenerateDirection3D<f32>();
				break;
			case SphereParticleEmitterDef::EmitDirectionType::k_awayFromCentre:
				out_direction = CSCore::Vector3::Normalise(out_position);
				break;
			default:
				CS_LOG_FATAL("Invalid 'Emit Direction' type.");
				break;
			}
		}
	}
}