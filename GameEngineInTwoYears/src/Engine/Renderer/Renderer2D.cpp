#include <enpch.h>
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Engine
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();

		s_Data->QuadVertexArray = VertexArray::Create();

		float SquareVertices[] = {  -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
									-0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
									 0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
									 0.5f, -0.5f, 0.0f,  1.0f, 0.0f };

		Ref<VertexBuffer> squareVB;
		squareVB = VertexBuffer::Create(SquareVertices, sizeof(SquareVertices));
		squareVB->SetLayout({
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float2, "a_TexCoord"}
			});
		s_Data->QuadVertexArray->AddVertexBuffer(squareVB);

		uint32_t SquareIndices[] = { 0, 1, 2, 2, 0, 3 };
		Engine::Ref<IndexBuffer> squareIB;
		squareIB = IndexBuffer::Create(SquareIndices, sizeof(SquareIndices) / sizeof(uint32_t));
		s_Data->QuadVertexArray->SetIndexBuffer(squareIB);

		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_Texture", 0);

	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float tiles = 1.0f)
	{
		s_Data->TextureShader->SetFloat4("u_Color", color);
		s_Data->TextureShader->SetFloat("u_TileMult", tiles);
		s_Data->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * /* rotation */ glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetMat4("u_Transform", transform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}


	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture, float tiles = 1.0f)
	{
		s_Data->TextureShader->SetFloat4("u_Color", glm::vec4(1.0f));
		s_Data->TextureShader->SetFloat("u_TileMult", tiles);
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * /* rotation */ glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetMat4("u_Transform", transform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color, float tiles)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color, tiles);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color, float tiles)
	{
		s_Data->TextureShader->SetFloat4("u_Color", color);
		s_Data->TextureShader->SetFloat("u_TileMult", tiles);
		s_Data->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetMat4("u_Transform", transform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture>& texture, float tiles)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tiles);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture>& texture, float tiles)
	{
		s_Data->TextureShader->SetFloat4("u_Color", glm::vec4(1.0f));
		s_Data->TextureShader->SetFloat("u_TileMult", tiles);
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetMat4("u_Transform", transform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float tiles = 1.0f)
	{
		DrawQuad({ position.x, position.y, 1.0f }, size, color, tiles);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture, float tiles = 1.0f)
	{
		DrawQuad({ position.x, position.y, 1.0f}, size, texture, tiles);
	}
}