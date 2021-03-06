#pragma once

#include "Texture.h"
#include "glm/glm.hpp"

namespace Engine
{
	class SubTexture2D
	{
	public:
		SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

		const glm::vec2* GetTextureCoords() const { return m_TexCoords; }
		const Ref<Texture2D> GetTexture() const { return m_Texture; }

		static Ref<SubTexture2D> GetFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize = {1.0f, 1.0f});
	private:
		Ref<Texture2D> m_Texture;
		glm::vec2 m_TexCoords[4];
	};
}