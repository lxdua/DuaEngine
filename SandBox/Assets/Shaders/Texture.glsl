#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_UV;

uniform mat4 ViewProjection;
uniform mat4 TRANSFORM;

out vec2 UV;
			
void main()
{
	UV = a_UV;
	gl_Position = ViewProjection * TRANSFORM * vec4(a_Position, 1.0);
}


#type fragment
#version 330 core

out vec4 COLOR;

in vec2 UV;

uniform sampler2D TEXTURE;
uniform vec4 MODULATE;

void main()
{
	COLOR = texture(TEXTURE, UV) * MODULATE;
}