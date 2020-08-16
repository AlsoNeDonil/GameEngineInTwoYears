#include <chrono>
#include "EditorLayer.h"
#include "Engine/Core/EntryPoint.h"

EditorLayer::EditorLayer():
	Layer("Layer2D"), m_CameraController((float)1280 / 720, true)
{
	ENGINE_PROFILE_FUNCTION();
}

void EditorLayer::OnAttach()
{
	ENGINE_PROFILE_FUNCTION();
	
	m_CheckerboardTexture = Engine::Texture2D::Create("assets/textures/clown.png");

	Engine::FramebufferSpecification fbSpec;

	fbSpec.Width = 1280;
	fbSpec.Height = 720;

	m_Framebuffer = Engine::Framebuffer::Create(fbSpec);
	m_Framebuffer->Bind();
}

void EditorLayer::OnDetach()
{
	ENGINE_PROFILE_FUNCTION();
}

void EditorLayer::OnUpdate(Engine::Timestep ts)
{
	ENGINE_PROFILE_FUNCTION();

	{
		ENGINE_PROFILE_SCOPE("EditorLayer::OnUpdate");
		m_CameraController.OnUpdate(ts);
	}

	Engine::Renderer2D::ResetStats();
	{
		m_Framebuffer->Bind();
		ENGINE_PROFILE_SCOPE("EditorLayer::Renderer prep");
		Engine::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		Engine::RenderCommand::Clear();
	}

	{
		ENGINE_PROFILE_SCOPE("EditorLayer::Renderer draw");
		Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Engine::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, {0.2f, 0.5f, 0.8f, 1.0f});
		Engine::Renderer2D::DrawQuad({ 0.0f, -2.0f }, { 1.0f, 1.0f }, { 0.5f, 0.2f, 0.8f, 1.0f });

		Engine::Renderer2D::DrawQuad({ 2.0f, 0.0f }, { 1.0f, 1.0f }, m_CheckerboardTexture);
		Engine::Renderer2D::DrawQuad({ 2.0f, -2.0f }, { 1.0f, 1.0f }, m_CheckerboardTexture);

		Engine::Renderer2D::EndScene();
		m_Framebuffer->Unbind();
	}
}

void EditorLayer::OnImGuiRender()
{
	ENGINE_PROFILE_FUNCTION();
	
	// Note: Switch this to true to enable dockspace
	static bool dockingEnabled = true;
	if (dockingEnabled)
	{
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("Exit")) Engine::Application::Get().Close();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::Begin("Settings");

		auto stats = Engine::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ 1280, 720.0f }, ImVec2{ 0, 1 }, ImVec2{1, 0});
		ImGui::End();

		ImGui::End();
	}

	else
	{
		ImGui::Begin("Settings");

		auto stats = Engine::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

		uint32_t textureID = m_CheckerboardTexture->GetRendererID();
		ImGui::Image((void*)textureID, ImVec2{ 1280, 720.0f }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
	}
}

void EditorLayer::OnEvent(Engine::Event& e)
{
	ENGINE_PROFILE_FUNCTION();
	m_CameraController.OnEvent(e);
}