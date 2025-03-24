#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_UV;
layout(location = 2) in vec4 a_Modulate;

uniform mat4 ViewProjection;
uniform mat4 TRANSFORM;

out vec2 v_UV;
out vec4 v_Modulate;
			
void main()
{
	v_UV = a_UV;
	v_Modulate = a_Modulate;
	gl_Position = ViewProjection * TRANSFORM * vec4(a_Position, 1.0);
}


#type fragment
#version 330 core

out vec4 COLOR;

in vec2 v_UV;
in vec4 v_Modulate;

uniform sampler2D TEXTURE;

void main()
{
	COLOR = texture(TEXTURE, v_UV) * v_Modulate;
}