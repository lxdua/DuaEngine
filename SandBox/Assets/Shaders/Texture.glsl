#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_UV;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec2 UV;
			
void main()
{
	UV = a_UV;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}


#type fragment
#version 330 core

out vec4 COLOR;

in vec2 UV;

uniform sampler2D TEXTURE;

void main()
{
	COLOR = texture(TEXTURE, UV);
}