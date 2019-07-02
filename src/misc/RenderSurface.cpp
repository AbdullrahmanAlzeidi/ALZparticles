#include <pch.h>

#include "RenderSurface.h"

namespace ALZ {

	static ShaderProgram* ImageShader = nullptr;
	static bool ImageShaderInitilized = false;

	RenderSurface::RenderSurface()
	{
		m_FrameBuffer = Renderer::CreateFrameBuffer();

		m_FrameBuffer->Bind();

		m_Texture = Renderer::CreateTexture();

		m_Size = Window::WindowSize;
		
		m_Texture->SetImage(m_Size, 3);

		m_Texture->SetDefaultTextureSettings();
		m_Texture->Unbind();

		m_FrameBuffer->SetActiveTexture(*m_Texture);

		m_VertexArray = Renderer::CreateVertexArray();
		m_VertexArray->Bind();

		// vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		float quadVertices[] = { 
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};

		m_VertexBuffer = Renderer::CreateVertexBuffer(quadVertices, sizeof(quadVertices));
		m_VertexBuffer->SetLayout({ ShaderVariableType::Vec2, ShaderVariableType::Vec2 });

		m_FrameBuffer->Unbind();

		if (ImageShaderInitilized == false)
		{
			ImageShader = Renderer::CreateShaderProgram("shaders/Image.vert", "shaders/Image.frag").release();
			ImageShaderInitilized = true;
		}
	}

	void RenderSurface::Render()
	{
		ImageShader->SetUniform1i("screenTexture", 0);
		m_Texture->Bind();
		Renderer::Draw(*m_VertexArray, *ImageShader, Primitive::Triangles, 6);
	}

	void RenderSurface::Render(ShaderProgram& shader)
	{
		m_Texture->Bind();
		Renderer::Draw(*m_VertexArray, shader, Primitive::Triangles, 6);
	}

	void RenderSurface::RenderToScreen()
	{
		//nullptr means we are copying to the default render buffer (which is the one being displayed)
		m_FrameBuffer->Blit(nullptr, m_Size, m_Size);

		Renderer::SetViewportSize(glm::ivec2(0.0), glm::ivec2(Window::WindowSize.x, Window::WindowSize.y));
	}

	void RenderSurface::SetSize(const glm::vec2 & size)
	{
		m_Size = size;
		m_FrameBuffer->Bind();
		
		//delete old trexture
		m_Texture.reset();
	
		//create a new one with the window size
		m_Texture = Renderer::CreateTexture();
		m_Texture->SetImage(size, 3);
	
		m_Texture->SetDefaultTextureSettings();
		m_Texture->Unbind();
	
		m_FrameBuffer->SetActiveTexture(*m_Texture);
		m_FrameBuffer->Unbind();
	}
}