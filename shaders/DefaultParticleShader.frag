#version 400

out vec4 FragColor;

uniform sampler2D baseImage;
in vec2 UV;
in vec4 finalColor;

void main()
{
    FragColor = vec4(texture(baseImage, UV).rgb + finalColor.xyz, finalColor.a);
	
	//FragColor = vec4(0.5, 0.3, 0.2,1.0);
}
