#type vertex
#version 330 core

layout(location = 0) in vec3 a_pos;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;	

out vec3 v_pos;
			
void main()
{
	v_pos = a_pos;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_pos, 1.0);
}



#type fragment
#version 330 core

layout(location = 0) out vec4 COLOR;
in vec3 v_pos;

uniform vec4 u_Color;

void main()
{
	COLOR = u_Color;
}