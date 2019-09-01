#include <pch.h>

#include "Sprite.h"

namespace ALZ {

	Sprite::Sprite() :
		m_FileBrowser(FileManager::ApplicationFolder, "Load Texture")
	{
		Type = SpriteType;
		m_Name = "Sprite";
		EditorOpen = false;

		m_FileBrowser.Filter.push_back(".png");
		m_FileBrowser.Filter.push_back(".jpeg");
		m_FileBrowser.Filter.push_back(".jpg");
		m_FileBrowser.Filter.push_back(".bmp");

		m_VertexArray = Renderer::CreateVertexArray();
		m_VertexArray->Bind();

		glm::vec2 vertices[] = {
								//Positions				//Texture Coordinates
								glm::vec2(-1.0f, -1.0f), glm::vec2(0.0, 0.0),
								glm::vec2(-1.0f,  1.0f), glm::vec2(0.0, 1.0),
								glm::vec2( 1.0f,  1.0f), glm::vec2(1.0, 1.0),

								glm::vec2( 1.0f,  1.0f), glm::vec2(1.0, 1.0),
								glm::vec2( 1.0f, -1.0f), glm::vec2(1.0, 0.0),
								glm::vec2(-1.0f, -1.0f), glm::vec2(0.0, 0.0)
		};
		
		m_VertexBuffer = Renderer::CreateVertexBuffer(vertices, sizeof(vertices));
		m_VertexBuffer->Bind();
		m_VertexBuffer->SetLayout({ ShaderVariableType::Vec2, ShaderVariableType::Vec2 });

		m_ShaderProgram = Renderer::CreateShaderProgram("shaders/Sprite.vert", "shaders/Sprite.frag");

		m_Texture = Renderer::CreateTexture();

		Image img = Image::LoadFromFile("res/CheckerBoard.png");

		Image::GrayScaleToRGB(img);

		m_Texture->SetImage(img);

	}

	void Sprite::Update(const float& deltaTime)
	{
	}

	void Sprite::Draw()
	{
		glm::mat4 u_Model(1.0f);
		u_Model = glm::translate(u_Model, glm::vec3(Position.x, Position.y, 0.0f) * GlobalScaleFactor);
		u_Model = glm::rotate(u_Model, glm::radians(Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		u_Model = glm::scale(u_Model, glm::vec3(Scale.x, Scale.y, 1.0f) * GlobalScaleFactor);

		m_ShaderProgram->SetUniformMat4("u_Model", u_Model);
		m_ShaderProgram->SetUniform1i("u_SpriteTexture", 0);
		m_ShaderProgram->SetUniformVec4("u_Tint", Tint);
		m_Texture->Bind(0);

		Renderer::Draw(*m_VertexArray, *m_ShaderProgram, Primitive::Triangles, 6);

		m_Texture->Unbind();
	}

	void Sprite::DisplayGUI()
	{
		if (!EditorOpen)
			return;

		ImGui::PushID(this);

		ImGui::Begin((m_Name + "##" + std::to_string(ImGui::GetID(this))).c_str(), &EditorOpen, ImGuiWindowFlags_NoSavedSettings);

		ImGui::Text("Texture: ");
		ImGui::SameLine();
		ImGui::Image((ImTextureID)m_Texture->GetRendererID(), ImVec2(100, 100), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 1));

		m_FileBrowser.DisplayGUI([this](const std::string& targetFile)
			{
				m_Texture.reset();

				m_Texture = Renderer::CreateTexture();

				Image img = Image::LoadFromFile(targetFile);

				if (img.m_Comp == 1)
					Image::GrayScaleToRGB(img);

				m_Texture->SetImage(img);
			}
		);

		if (ImGui::Button("Load Texture"))
			m_FileBrowser.OpenWindow();

		ImGui::Spacing();

		ImGui::Text("Position: ");
		ImGui::SameLine();
		ImGui::DragFloat2("##Position: ", &Position.x, 0.01f);

		ImGui::Text("Scale: ");
		ImGui::SameLine();
		ImGui::DragFloat2("##Scale: ", &Scale.x, 0.01f);

		ImGui::Text("Rotate: ");
		ImGui::SameLine();
		ImGui::DragFloat("##Rotate: ", &Rotation);
		Rotation = std::clamp(Rotation, 0.0f, 360.0f);

		ImGui::Text("Tint: ");
		ImGui::SameLine();
		ImGui::ColorEdit4("##Tint: ", &Tint.r);
		

		ImGui::End();

		ImGui::PopID();
	}

}