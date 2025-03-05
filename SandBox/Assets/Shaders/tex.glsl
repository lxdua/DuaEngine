#type vertex
#version 330 core

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec4 a_color;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec3 v_pos;
out vec4 v_color;
			
void main()
{
	v_pos = a_pos;
	v_color = a_color;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_pos, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;
in vec3 v_pos;
in vec4 v_color;

void main()
{
	color = v_color;
}