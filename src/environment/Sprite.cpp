#include "Sprite.h"

namespace Ainan {

	Sprite::Sprite()
	{
		Type = SpriteType;
		m_Name = "Sprite";
		EditorOpen = false;

		Image img = Image::LoadFromFile("res/CheckerBoard.png");

		Image::GrayScaleToRGBA(img);

		m_Texture = Renderer::CreateTexture(img);
	}

	Sprite::~Sprite()
	{
		Renderer::DestroyTexture(m_Texture);
	}

	void Sprite::Update(const float deltaTime)
	{
	}

	void Sprite::Draw()
	{
		Renderer::DrawQuad(Model[3], Tint, Scale, -Rotation * PI / 180.0f, m_Texture);
	}

	void Sprite::DisplayGUI()
	{
		if (!EditorOpen)
			return;

		ImGui::PushID(this);

		ImGui::Begin((m_Name + "##" + std::to_string(ImGui::GetID(this))).c_str(), &EditorOpen, ImGuiWindowFlags_NoSavedSettings);

		ImGui::Text("Texture: ");
		ImGui::SameLine();
		ImGui::Image((void*)m_Texture.GetTextureID(), ImVec2(100, 100), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 1));

		if (ImGui::BeginCombo("##Texture: ", m_TexturePath == "" ? "None" : m_TexturePath.filename().u8string().c_str()))
		{
			auto textures = AssetManager::GetAll2DTextures();
			bool selected = false;
			if (ImGui::Selectable("None", &selected))
			{
				LoadTextureFromFile("res/CheckerBoard.png");
				m_TexturePath = "";
			}
			for (auto& tex : textures)
			{
				std::string textureFileName = std::filesystem::path(tex).filename().u8string();
				if (ImGui::Selectable(textureFileName.c_str(), &selected))
				{
					if (textureFileName != "Default")
					{
						LoadTextureFromFile(tex.u8string());
						m_TexturePath = tex.lexically_relative(AssetManager::s_EnvironmentDirectory).u8string();
					}
				}
			}

			ImGui::EndCombo();
		}


		ImGui::Spacing();

		ImGui::Text("Position: ");
		ImGui::SameLine();
		ImGui::DragFloat2("##Position: ", &Model[3][0], c_ObjectPositionDragControlSpeed);

		ImGui::Text("Scale: ");
		ImGui::SameLine();
		ImGui::DragFloat("##Scale: ", &Scale, c_ObjectScaleDragControlSpeed);

		ImGui::Text("Rotation: ");
		ImGui::SameLine();
		ImGui::DragFloat("##Rotation: ", &Rotation, c_ObjectRotationDragControlSpeed);
		Rotation = std::clamp(Rotation, 0.0f, 360.0f);

		ImGui::Text("Tint: ");
		ImGui::SameLine();
		ImGui::ColorEdit4("##Tint: ", &Tint.r);
		
		Renderer::RegisterWindowThatCanCoverViewport();
		ImGui::End();

		ImGui::PopID();
	}

	void Sprite::LoadTextureFromFile(const std::string& path)
	{
		Renderer::DestroyTexture(m_Texture);

		Image img = Image::LoadFromFile(path);

		if (img.Format == TextureFormat::R)
			Image::GrayScaleToRGB(img);

		m_Texture = Renderer::CreateTexture(img);
	}

}