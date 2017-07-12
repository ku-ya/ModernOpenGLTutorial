#version 120

varying vec2 texCoord0;
varying vec3 normal0;

uniform sampler2D sampler;
uniform vec3 lightDirection;

void main()
{
	gl_FragColor = texture2D(sampler, texCoord0) * clamp(dot(-lightDirection, normal0), 0.0, 1.0);
	gl_FragColor = vec4(1.0f, 0.0f,1.0f, 1.0f) * clamp(dot(-lightDirection, normal0), 0.0, 1.0);
}
