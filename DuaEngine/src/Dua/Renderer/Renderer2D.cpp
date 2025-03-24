#include "duapch.h"
#include "Renderer2D.h"

#include "RenderCommand.h"
#include "VertexArray.h"
#include "Shader.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Dua {

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec2 UV;
		glm::vec4 Modulate;
	};

	struct Renderer2DStorage
	{
		const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage;
		s_Data->QuadVertexArray = VertexArray::Create();

		s_Data->QuadVertexBuffer = VertexBuffer::Create(s_Data->MaxVertices * sizeof(QuadVertex));
		s_Data->QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Vec3, "a_Position" },
			{ ShaderDataType::Vec2, "a_UV" },
			{ ShaderDataType::Vec4, "a_Modulate" }
			});
		s_Data->QuadVertexArray->AddVertexBuffer(s_Data->QuadVertexBuffer);

		s_Data->QuadVertexBufferBase = new QuadVertex[s_Data->MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_Data->MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data->MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;
			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;
			offset += 4;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data->MaxIndices);
		s_Data->QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		s_Data->TextureShader = Shader::Create("Assets/Shaders/Texture.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("TEXTURE", 0);
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("ViewProjection", camera.GetViewProjectionMatrix());

		s_Data->QuadIndexCount = 0;
		s_Data->QuadVertexBufferPtr = s_Data->QuadVertexBufferBase;
	}

	void Renderer2D::EndScene()
	{
		uint32_t dataSize = (uint8_t*)s_Data->QuadVertexBufferPtr - (uint8_t*)s_Data->QuadVertexBufferBase;
		s_Data->QuadVertexBuffer->SetData(s_Data->QuadVertexBufferBase, dataSize);
		Flush();
	}

	void Renderer2D::Flush()
	{
		//RenderCommand::DrawIndexed(s_Data->QuadVertexArray, s_Data->QuadIndexCount);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, glm::vec4 modulate)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, rotation, texture, modulate);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, glm::vec4 modulate)
	{
		s_Data->QuadVertexBufferPtr->Position = position;
		s_Data->QuadVertexBufferPtr->UV = { 0.0f,0.0f };
		s_Data->QuadVertexBufferPtr->Modulate = modulate;
		s_Data->QuadVertexBufferPtr++;

		s_Data->QuadVertexBufferPtr->Position = { position.x + size.x, position.y, 0.0f };
		s_Data->QuadVertexBufferPtr->UV = { 1.0f,0.0f };
		s_Data->QuadVertexBufferPtr->Modulate = modulate;
		s_Data->QuadVertexBufferPtr++;

		s_Data->QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
		s_Data->QuadVertexBufferPtr->UV = { 1.0f,1.0f };
		s_Data->QuadVertexBufferPtr->Modulate = modulate;
		s_Data->QuadVertexBufferPtr++;

		s_Data->QuadVertexBufferPtr->Position = { position.x, position.y + size.y, 0.0f };
		s_Data->QuadVertexBufferPtr->UV = { 0.0f,1.0f };
		s_Data->QuadVertexBufferPtr->Modulate = modulate;
		s_Data->QuadVertexBufferPtr++;

		s_Data->QuadIndexCount += 6;

		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f,0.0f,1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("TRANSFORM", transform);
		//s_Data->TextureShader->SetVec4("MODULATE", modulate);

		texture->Bind();

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray, s_Data->QuadIndexCount);
	}

}