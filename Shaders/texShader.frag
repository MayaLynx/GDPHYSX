#version 330 core

uniform vec3 objectColor; // Color of object
uniform sampler2D tex0;

in vec2 texCoord;

out vec4 FragColor;

void main()
{
	vec4 pixelColor = texture(tex0, texCoord);
	if(pixelColor.a < 0.1)
	{
		discard;
	}

	if (tex0)
	{
		FragColor = vec4(pixelColor, 1.f);
	}
	else
	{
		FragColor = vec4(objectColor, 1.f);
	}
	
}