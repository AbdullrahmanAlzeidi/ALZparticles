#include <pch.h>

#include "Environment.h"
#include "EnvironmentObjectInterface.h"
#include "json/json.hpp"
#include "Sprite.h"
#include "ParticleSystem.h"
#include "editor/Background.h"

using json = nlohmann::json;

#define VEC4_TO_JSON_ARRAY(vec) { vec.x, vec.y, vec.z, vec.w }
#define VEC3_TO_JSON_ARRAY(vec) { vec.x, vec.y, vec.z }
#define VEC2_TO_JSON_ARRAY(vec) { vec.x, vec.y }

namespace Ainan {

	//forward declarations
	static void toJson(json& j, const ParticleSystem& ps, size_t objectOrder);
	static void toJson(json& j, const RadialLight& light, size_t objectOrder);
	static void toJson(json& j, const SpotLight& light, size_t objectOrder);
	static void toJson(json& j, const Sprite& sprite, size_t objectOrder);

	bool SaveEnvironment(const Environment& env, std::string path)
	{
		json data;

		data["EnvironmentName"] = env.Name;

		//serialize inspector objects count int
		data["objectCount"] = env.Objects.size();
		
		//serialize inspector objects
		for (size_t i = 0; i < env.Objects.size(); i++)
		{
			//save object depending on what type it is
			switch (env.Objects[i]->Type)
			{
			case ParticleSystemType:
				toJson(data, *(ParticleSystem*)env.Objects[i].get(), i);
				break;

			case RadialLightType:
				toJson(data, *(RadialLight*)env.Objects[i].get(), i);
				break;

			case SpotLightType:
				toJson(data, *(SpotLight*)env.Objects[i].get(), i);
				break;

			case SpriteType:
				toJson(data, *(Sprite*)env.Objects[i].get(), i);
				break;

			default: //this means we have a type that we haven't implemented how to save it
				assert(false);
				break;
			}
		}

		data["BlurEnabled"] = env.BlurEnabled;
		data["BlurRadius"] = env.BlurRadius;

		data["BackgroundColor"] = VEC3_TO_JSON_ARRAY(env.BackgroundColor);
		data["BackgroundBaseLight"] = env.BackgroundBaseLight;
		data["BackgroundConstant"] = env.BackgroundConstant;
		data["BackgroundLinear"] = env.BackgroundLinear;
		data["BackgroundQuadratic"] = env.BackgroundQuadratic;

		std::string jsonString = data.dump(4);

		FILE* file = (FILE*)1;
		auto error = fopen_s(&file, path.c_str(), "w");
		if (error == 0) {
			fwrite(jsonString.c_str(), 1, jsonString.size(), file);
			fclose(file);
		}
		else
			assert(false, "Error while trying to save environment");
		
		return true;
	}

	void toJson(json& j, const ParticleSystem& ps, size_t objectOrder)
	{
		std::string id = "obj" + std::to_string(objectOrder) + "_";

		j[id + "Type"] = EnvironmentObjectTypeToString(ParticleSystemType).c_str();
		j[id + "Name"] = ps.m_Name;
		j[id + "Mode"] = GetModeAsText(ps.Customizer.Mode);
		j[id + "ParticlesPerSecond"] = ps.Customizer.m_ParticlesPerSecond;
		j[id + "SpawnPosition"] = VEC2_TO_JSON_ARRAY(ps.Customizer.m_SpawnPosition);
		j[id + "LinePosition"] = VEC2_TO_JSON_ARRAY(ps.Customizer.m_LinePosition);
		j[id + "LineLength"] = ps.Customizer.m_LineLength;
		j[id + "LineAngle"] = ps.Customizer.m_LineAngle;
		j[id + "CirclePosition"] = VEC2_TO_JSON_ARRAY(ps.Customizer.m_CircleOutline.Position);
		j[id + "CircleRadius"] = ps.Customizer.m_CircleOutline.Radius;

		//Scale data
		j[id + "MinScale"] = ps.Customizer.m_ScaleCustomizer.m_MinScale;
		j[id + "MaxScale"] = ps.Customizer.m_ScaleCustomizer.m_MaxScale;
		j[id + "DefinedScale"] = ps.Customizer.m_ScaleCustomizer.m_DefinedScale;
		j[id + "EndScale"] = ps.Customizer.m_ScaleCustomizer.m_EndScale;
		j[id + "ScaleInterpolationType"] = InterpolationTypeToString(ps.Customizer.m_ScaleCustomizer.m_Interpolator.Type);

		//Color data
		j[id + "DefinedColor"] = VEC4_TO_JSON_ARRAY(ps.Customizer.m_ColorCustomizer.m_DefinedColor);
		j[id + "EndColor"] = VEC4_TO_JSON_ARRAY(ps.Customizer.m_ColorCustomizer.m_EndColor);
		j[id + "ColorInterpolationType"] = InterpolationTypeToString(ps.Customizer.m_ColorCustomizer.m_Interpolator.Type);

		//Lifetime data
		j[id + "IsLifetimeRandom"] = ps.Customizer.m_LifetimeCustomizer.m_RandomLifetime;
		j[id + "DefinedLifetime"] = ps.Customizer.m_LifetimeCustomizer.m_DefinedLifetime;
		j[id + "MinLifetime"] = ps.Customizer.m_LifetimeCustomizer.m_MinLifetime;
		j[id + "MaxLifetime"] = ps.Customizer.m_LifetimeCustomizer.m_MaxLifetime;

		//Velocity data
		j[id + "IsStartingVelocityRandom"] = ps.Customizer.m_VelocityCustomizer.m_RandomVelocity;
		j[id + "DefinedVelocity"] = VEC2_TO_JSON_ARRAY(ps.Customizer.m_VelocityCustomizer.m_DefinedVelocity);
		j[id + "MinVelocity"] = VEC2_TO_JSON_ARRAY(ps.Customizer.m_VelocityCustomizer.m_MinVelocity);
		j[id + "MaxVelocity"] = VEC2_TO_JSON_ARRAY(ps.Customizer.m_VelocityCustomizer.m_MaxVelocity);
		j[id + "VelocityLimitType"] = LimitTypeToString(ps.Customizer.m_VelocityCustomizer.CurrentVelocityLimitType);
		j[id + "MinNormalVelocityLimit"] = ps.Customizer.m_VelocityCustomizer.m_MinNormalVelocityLimit;
		j[id + "MaxNormalVelocityLimit"] = ps.Customizer.m_VelocityCustomizer.m_MaxNormalVelocityLimit;
		j[id + "MinPerAxisVelocityLimit"] = VEC2_TO_JSON_ARRAY(ps.Customizer.m_VelocityCustomizer.m_MinPerAxisVelocityLimit);
		j[id + "MaxPerAxisVelocityLimit"] = VEC2_TO_JSON_ARRAY(ps.Customizer.m_VelocityCustomizer.m_MaxPerAxisVelocityLimit);

		//Noise data
		j[id + "NoiseEnabled"] = ps.Customizer.m_NoiseCustomizer.m_NoiseEnabled;
		j[id + "NoiseStrength"] = ps.Customizer.m_NoiseCustomizer.m_NoiseStrength;

		//Texture data
		j[id + "UseDefaultTexture"] = ps.Customizer.m_TextureCustomizer.UseDefaultTexture;
		j[id + "TexturePath"] = ps.Customizer.m_TextureCustomizer.m_TexturePath;

		//Force data
		{
			size_t i = 0;
			j[id + "Force Count"] = ps.Customizer.m_ForceCustomizer.m_Forces.size();
			for (auto& force : ps.Customizer.m_ForceCustomizer.m_Forces)
			{
				j[id + "Force" + std::to_string(i).c_str() + "Key"] = force.first;
				j[id + "Force" + std::to_string(i).c_str() + "Enabled"] = force.second.Enabled;
				j[id + "Force" + std::to_string(i).c_str() + "Type"] = std::string(force.second.ForceTypeToString(force.second.Type));
				j[id + "Force" + std::to_string(i).c_str() + "DF_Value"] = VEC2_TO_JSON_ARRAY(force.second.DF_Value);
				j[id + "Force" + std::to_string(i).c_str() + "RF_Target"] = VEC2_TO_JSON_ARRAY(force.second.RF_Target);
				j[id + "Force" + std::to_string(i).c_str() + "RF_Strength"] = force.second.RF_Strength;

				i++;
			}
		}
	}

	void toJson(json& j, const RadialLight& light, size_t objectOrder)
	{
		std::string id = "obj" + std::to_string(objectOrder) + "_";

		j[id + "Type"] = EnvironmentObjectTypeToString(RadialLightType);
		j[id + "Name"] = light.m_Name;
		j[id + "Position"] = VEC2_TO_JSON_ARRAY(light.Position);
		j[id + "Color"] = VEC3_TO_JSON_ARRAY(light.Color);
		j[id + "Intensity"] = light.Intensity;
	}

	void toJson(json& j, const SpotLight& light, size_t objectOrder)
	{
		std::string id = "obj" + std::to_string(objectOrder) + "_";

		j[id + "Type"] = EnvironmentObjectTypeToString(SpotLightType);
		j[id + "Name"] = light.m_Name;
		j[id + "Position"] = VEC2_TO_JSON_ARRAY(light.Position);
		j[id + "Color"] = VEC3_TO_JSON_ARRAY(light.Color);
		j[id + "OuterCutoff"] = light.OuterCutoff;
		j[id + "InnerCutoff"] = light.InnerCutoff;
		j[id + "Intensity"] = light.Intensity;
	}

	void toJson(json& j, const Sprite& sprite, size_t objectOrder)
	{
		std::string id = "obj" + std::to_string(objectOrder) + "_";

		j[id + "Type"] = EnvironmentObjectTypeToString(SpriteType);
		j[id + "Name"] = sprite.m_Name;
		j[id + "Position"] = VEC2_TO_JSON_ARRAY(sprite.Position);
		j[id + "Scale"] = sprite.Scale;
		j[id + "Rotation"] = sprite.Rotation;
		j[id + "Tint"] = VEC4_TO_JSON_ARRAY(sprite.Tint);
		j[id + "TexturePath"] = sprite.m_TexturePath;
	}

}

#undef VEC4_TO_JSON_ARRAY
#undef VEC3_TO_JSON_ARRAY
#undef VEC2_TO_JSON_ARRAY