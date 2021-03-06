#include "UniformBuffer.h"

#include "RenderCommand.h"
#include "Renderer.h"

namespace Ainan
{
	void UniformBuffer::UpdateData(void* data, uint32_t packedDataSizeofBuffer)
	{
		RenderCommand cmd;
		cmd.Type = RenderCommandType::UpdateUniformBuffer;
		cmd.UpdateUniformBufferCmdDesc.Buffer = &Renderer::Rdata->UniformBuffers[Identifier];
		void* dataCpy = new uint8_t[packedDataSizeofBuffer];
		memcpy(dataCpy, data, packedDataSizeofBuffer);
		cmd.UpdateUniformBufferCmdDesc.Data = dataCpy;
		Renderer::PushCommand(cmd);
	}
}
