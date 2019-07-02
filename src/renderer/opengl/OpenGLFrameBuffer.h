#pragma once

#include "renderer/Texture.h"
#include "renderer/FrameBuffer.h"

namespace ALZ {
	namespace OpenGL {

		class OpenGLFrameBuffer : public FrameBuffer
		{
		public:
			OpenGLFrameBuffer();
			~OpenGLFrameBuffer();

			// Inherited via FrameBuffer
			virtual void Blit(FrameBuffer* otherBuffer, const glm::vec2& sourceSize, const glm::vec2& targetSize) override;

			virtual void Bind() const override;
			virtual void Unbind() const override;
			virtual unsigned int GetRendererID() const override { return m_RendererID; }

		private:
			unsigned int m_RendererID;

			// Inherited via FrameBuffer
			virtual void SetActiveTexture(const Texture& texture) override;
		};
	}
}