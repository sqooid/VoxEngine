#type vertex
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec4 normal;

layout (std140) uniform camera 
{
	mat4 viewProj;
};

out vec4 outColor;
out vec4 outNormal;

void main()
{
	outColor = color;
	outNormal = normal;

	gl_Position = vec4(position.x, position.y, position.z, 1.0);
}

#type fragment
#version 330 core

in vec4 outColor;
in vec4 outNormal;

layout (location = 0) out vec4 fragColor;

void main()
{
	vec4 light = normalize(vec4(1.0, -1.0, -1.0, 0.0));
	fragColor = vec4(outColor) * dot(outNormal, light);
}