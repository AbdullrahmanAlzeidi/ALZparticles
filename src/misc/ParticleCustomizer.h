#pragma once

#include "Window.h"
#include "Particle.h"
#include "customizers/TextureCustomizer.h"
#include "customizers/VelocityCustomizer.h"
#include "customizers/ColorCustomizer.h"
#include "customizers/ScaleCustomizer.h"
#include "customizers/LifetimeCustomizer.h"
#include "customizers/NoiseCustomizer.h"
#include "customizers/ForceCustomizer.h"
#include "misc/Line.h"
#include "misc/CircleOutline.h"

namespace ALZ {

	enum class SpawnMode {
		SpawnOnPoint,
		SpawnOnLine,
		SpawnOnCircle,
		SpawnInsideCircle
	};

	std::string GetModeAsText(const SpawnMode& mode);
	SpawnMode GetTextAsMode(const std::string& mode);

	class ParticleCustomizer
	{
	public:
		ParticleCustomizer();

		ParticleCustomizer(const ParticleCustomizer& customizer);
		ParticleCustomizer operator=(const ParticleCustomizer& customizer);

		void DisplayGUI(const std::string& windowName, bool& windowOpen);
		void Update();
		Particle& GetParticle();

		float GetTimeBetweenParticles() { return 1 / m_ParticlesPerSecond; }

	public:
		SpawnMode Mode = SpawnMode::SpawnOnPoint;

	public:
		float m_ParticlesPerSecond = 100.0f;

		VelocityCustomizer m_VelocityCustomizer;
		NoiseCustomizer m_NoiseCustomizer;
		LifetimeCustomizer m_LifetimeCustomizer;
		ScaleCustomizer m_ScaleCustomizer;
		ColorCustomizer m_ColorCustomizer;
		TextureCustomizer m_TextureCustomizer;
		ForceCustomizer m_ForceCustomizer;

		//this is on a scale from 0 to 1
		glm::vec2 m_SpawnPosition = { 0.5f, 0.5f };

		//the particle that is going to be spawned next
		Particle m_Particle;

		//spawn particle on line option
		glm::vec2 m_LinePosition = { 0.5f, 0.5f };
		float m_LineLength = 0.2f;
		float m_LineAngle = 0.0f; //in degrees
		Line m_Line;

		//spawn particle on circle option
		CircleOutline m_CircleOutline;

		//random number generator
		std::mt19937 mt;

		friend class ParticleSystem;
	};
}