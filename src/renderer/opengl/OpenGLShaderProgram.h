#include "renderer/ShaderProgram.h"

namespace Ainan {
	namespace OpenGL {

		class OpenGLShaderProgram : public ShaderProgram
		{
		public:
			OpenGLShaderProgram(const std::string& vertPath, const std::string& fragPath);
			OpenGLShaderProgram() { m_RendererID = 0; }
			static std::shared_ptr<OpenGLShaderProgram> CreateRaw(const std::string& vertSrc, const std::string& fragSrc);
			~OpenGLShaderProgram();

			void Bind() const override;
			void Unbind() const override;

			void SetUniform1i(const char* name, const int& value)                                  override;
			void SetUniform1f(const char* name, const float& value)                                override;
			void SetUniform1fs(const char* name, float* value, const int& count)                   override;
			void SetUniformVec2(const char* name, const glm::vec2& value)                          override;
			void SetUniformVec2s(const char* name, const glm::vec2* const value, const int& count) override;
			void SetUniformVec3(const char* name, const glm::vec3& value)                          override;
			void SetUniformVec3s(const char* name, const glm::vec3* const value, const int& count) override;
			void SetUniformVec4(const char* name, const glm::vec4& value)                          override;
			void SetUniformVec4s(const char* name, const glm::vec4* const value, const int& count) override;
			void SetUniformMat4(const char* name, const glm::mat4& value)                          override;
			void SetUniformMat4s(const char* name, const glm::mat4* const value, const int& count) override;

			int GetUniformLocation(const char* name) override;
			virtual int GetRendererID() const override;

		private:
			unsigned int m_RendererID;

			//this is to avoid getting uniform location repeatedly which is not very performant
			std::unordered_map<std::string, int> m_UniformLocationMap;

			// Inherited via ShaderProgram
			virtual void BindUniformBuffer(const char* name, uint32_t slot) override;
		};
	}
}