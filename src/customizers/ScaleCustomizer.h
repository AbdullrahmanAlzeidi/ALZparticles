#pragma once

//for declaring friend to json serializer
#include "json/json_fwd.hpp"
namespace ALZ {
	class Environment;
	class ParticleSystem;
}

#include "misc/InterpolationSelector.h"
#include "misc/CurveEditor.h"

namespace ALZ {

	class ScaleCustomizer
	{
	public:
		ScaleCustomizer();
		void DisplayGUI();

		InterpolationSelector<float>& GetScaleInterpolator();
		CurveEditor m_Curve;

	private:
		//starting scale
		bool m_RandomScale = true;
		float m_DefinedScale = 2.0f;
		float m_MinScale = 20.0f;
		float m_MaxScale = 25.0f;

		//scale over time
		InterpolationSelector<float> m_Interpolator;
		float m_EndScale = m_DefinedScale;

		//random number generator
		std::mt19937 mt;

		friend void toJson(nlohmann::json& j, const ParticleSystem& ps, int objectOrder);
		friend void ParticleSystemFromJson(Environment* env, nlohmann::json& data, std::string id);
	};
}