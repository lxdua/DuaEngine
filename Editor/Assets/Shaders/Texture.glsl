#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_UV;
layout(location = 2) in float a_TexIndex;
layout(location = 3) in vec4 a_Modulate;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec2 v_UV;
out float v_TexIndex;
out vec4 v_Modulate;
			
void main()
{
	v_UV = a_UV;
	v_TexIndex = a_TexIndex;
	v_Modulate = a_Modulate;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
	//gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}


#type fragment
#version 330 core

out vec4 COLOR;

in vec2 v_UV;
in float v_TexIndex;
in vec4 v_Modulate;

uniform sampler2D u_Textures[32];

void main()
{
	COLOR = texture(u_Textures[int(v_TexIndex)], v_UV) * v_Modulate;
}