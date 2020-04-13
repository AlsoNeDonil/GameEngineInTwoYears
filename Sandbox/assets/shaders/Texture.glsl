#type vertex
#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec4 a_Color;
layout (location = 2) in vec2 a_TexCoord;
		
out vec2 v_TexCoord;
out vec4 v_Color;
		
uniform mat4 u_ViewProjection;

void main()
{
	v_TexCoord = a_TexCoord;
	v_Color = a_Color;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0f);
}		

#type fragment
#version 330 core

in vec2 v_TexCoord;
in vec4 v_Color;
out vec4 color;

uniform sampler2D u_Texture;
uniform vec4 u_Color;
uniform float u_TileMult;

void main()
{
	color = v_Color;
}		
		