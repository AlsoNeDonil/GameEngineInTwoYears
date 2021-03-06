#pragma once
#include "Engine.h"
#include "Engine/Renderer/Animation2D.h"
#include <vector>

#include <box2d/box2d.h>

#include "Box.h"

namespace Sandbox
{

	class Sandbox2D : public Engine::Layer
	{
	public:
		Sandbox2D();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Engine::Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Engine::Event& event) override;
	private:
		Engine::OrthographicCameraController m_CameraController;
		Engine::Ref<Engine::Texture2D> m_BoxTexture;

		Engine::Scope<b2World> m_World;

		Box m_Boxes[500];
		uint32_t m_BoxesCount = 0;
		b2Body* m_Ground;
	};

}