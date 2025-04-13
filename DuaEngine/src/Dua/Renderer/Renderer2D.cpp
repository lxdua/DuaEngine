#include "duapch.h"
#include "Renderer2D.h"

#include "RenderCommand.h"
#include "VertexArray.h"
#include "Shader.h"

#include "glm/gtc/matrix_transform.hpp"

namespace Dua {

	static glm::mat4 CalculateTransform(const glm::vec3& pos, const glm::vec2& size, float rot)
	{
		return glm::translate(glm::mat4(1.0f), pos)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rot), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
	}

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec2 UV;
		float TexIndex;
		glm::vec4 Modulate;
	};

	struct Renderer2DStorage
	{
		static const uint32_t MaxQuads = 10000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;

		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;

		std::unordered_map<Texture2D*, uint32_t> TextureIndexMap;///
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
			{ ShaderDataType::Float, "a_TexIndex" },
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

		s_Data->WhiteTexture = Texture2D::Create("Assets/Textures/White.png");

		int samplers[32];
		for (int i = 0; i < s_Data->MaxTextureSlots; i++)
		{
			samplers[i] = i;
		}

		s_Data->TextureShader = Shader::Create("Assets/Shaders/Texture.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetIntArray("u_Textures", samplers, s_Data->MaxTextureSlots);

		s_Data->TextureSlots[0] = s_Data->WhiteTexture;
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data->QuadIndexCount = 0;
		s_Data->QuadVertexBufferPtr = s_Data->QuadVertexBufferBase;

		s_Data->TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		uint32_t dataSize = (uint8_t*)s_Data->QuadVertexBufferPtr - (uint8_t*)s_Data->QuadVertexBufferBase;
		s_Data->QuadVertexBuffer->SetData(s_Data->QuadVertexBufferBase, dataSize);
		Flush();
	}

	void Renderer2D::Flush()
	{
		for (uint32_t i = 0; i < s_Data->TextureSlotIndex; i++)
		{
			s_Data->TextureSlots[i]->Bind(i);
		}
		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray, s_Data->QuadIndexCount);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, glm::vec4 modulate)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, rotation, texture, modulate);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, glm::vec4 modulate)
	{
		glm::mat4 transform = CalculateTransform(position, size, rotation);
		DrawQuad(transform, texture, modulate);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, glm::vec4 modulate)
	{
		float textureIndex = 0.0f;
		auto it = s_Data->TextureIndexMap.find(texture.get());
		if (it != s_Data->TextureIndexMap.end())
		{
			textureIndex = static_cast<float>(it->second);
		}
		else
		{
			if (s_Data->TextureSlotIndex >= Renderer2DStorage::MaxTextureSlots)
			{
				FlushAndReset();
			}
			textureIndex = static_cast<float>(s_Data->TextureSlotIndex);
			s_Data->TextureSlots[s_Data->TextureSlotIndex] = texture;
			s_Data->TextureIndexMap[texture.get()] = s_Data->TextureSlotIndex;
			s_Data->TextureSlotIndex++;
		}

		constexpr glm::vec4 modelPositions[4] = {
			{0.0f, 0.0f, 0.0f, 1.0f},  // 左下
			{1.0f, 0.0f, 0.0f, 1.0f},  // 右下
			{1.0f, 1.0f, 0.0f, 1.0f},  // 右上
			{0.0f, 1.0f, 0.0f, 1.0f}   // 左上
		};

		// 写入顶点数据（通过变换矩阵传递给着色器）
		for (int i = 0; i < 4; ++i)
		{
			s_Data->QuadVertexBufferPtr->Position = transform * modelPositions[i];
			s_Data->QuadVertexBufferPtr->UV = { (i == 0 || i == 3) ? 0.0f : 1.0f, (i < 2) ? 0.0f : 1.0f };
			s_Data->QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data->QuadVertexBufferPtr->Modulate = modulate;
			s_Data->QuadVertexBufferPtr++;
		}
		s_Data->QuadIndexCount += 6;

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_Transform", glm::mat4(1.0f));
	}

	void Renderer2D::FlushAndReset()
	{
		EndScene();

		s_Data->QuadIndexCount = 0;
		s_Data->QuadVertexBufferPtr = s_Data->QuadVertexBufferBase;

		s_Data->TextureSlotIndex = 1;
	}

}