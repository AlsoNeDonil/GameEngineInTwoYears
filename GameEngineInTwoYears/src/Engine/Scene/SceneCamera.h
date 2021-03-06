#include "Engine/Renderer/Camera.h"

namespace Engine
{
	class SceneCamera : public Camera
	{
	public:
		friend class SceneSerializer;

		enum ProjectionType { Perspective = 0, Orthographic = 1 };
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);
		void SetPerspective(float verticalFOV, float nearClip, float farClip);

		void SetViewportSize(uint32_t width, uint32_t height);

		ProjectionType GetProjectionType() const { return m_ProjectionType; }
		void SetProjectionType(ProjectionType type) { m_ProjectionType = type; RecalculateProjection(); }

		//Perspective
		float GetPerspectiveVerticalFOV() const { return m_PerspectiveFOV; }
		void SetPerspectiveVerticalFOV(float verticalFOV) { m_PerspectiveFOV = verticalFOV; RecalculateProjection(); }

		float GetPerspectiveNear() const { return m_PerspectiveNear; }
		void SetPerspectiveNear(float nearClip) { m_PerspectiveNear = nearClip; RecalculateProjection(); }

		float GetPerspectiveFar() const { return m_PerspectiveFar; }
		void SetPerspectiveFar(float farClip) { m_PerspectiveFar = farClip; RecalculateProjection(); }

		//Orhographic
		float GetOrthographicSize() const { return m_OrthographicSize; }
		void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection(); }

		float GetOrthographicNear() const { return m_OrthographicNear; }
		void SetOrthographicNear(float nearClip) { m_OrthographicNear = nearClip; RecalculateProjection(); }

		float GetOrthographicFar() const { return m_OrthographicFar; }
		void SetOrthographicFar(float farClip) { m_OrthographicFar = farClip; RecalculateProjection(); }
	private:
		void RecalculateProjection();
	private:
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;

		float m_PerspectiveFOV = glm::radians(45.0f);
		float m_PerspectiveNear = 0.1f; float m_PerspectiveFar = 1000.0f;

		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f; float m_OrthographicFar  = 1.0f;

		float m_AspectRatio = 0.0f;
	};
}