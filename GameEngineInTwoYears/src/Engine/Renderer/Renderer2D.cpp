#include <enpch.h>
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"


namespace Engine
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> FlatColorShader;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();

		s_Data->QuadVertexArray = VertexArray::Create();

		float SquareVertices[] = {  -0.5f, -0.5f, 0.0f,
									-0.5f,  0.5f, 0.0f,
									 0.5f,  0.5f, 0.0f,
									 0.5f, -0.5f, 0.0f };

		Ref<VertexBuffer> squareVB;
		squareVB = VertexBuffer::Create(SquareVertices, sizeof(SquareVertices));
		squareVB->SetLayout({
			{ShaderDataType::Float3, "a_Position"}
			});
		s_Data->QuadVertexArray->AddVertexBuffer(squareVB);

		uint32_t SquareIndices[] = { 0, 1, 2, 2, 0, 3 };
		Engine::Ref<IndexBuffer> squareIB;
		squareIB = IndexBuffer::Create(SquareIndices, sizeof(SquareIndices) / sizeof(uint32_t));
		s_Data->QuadVertexArray->SetIndexBuffer(squareIB);

		s_Data->FlatColorShader = Shader::Create("assets/shaders/Square.glsl");
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 1.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->SetFloat4("u_Color", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * /* rotation */ glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->FlatColorShader->SetMat4("u_Transform", transform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}
}