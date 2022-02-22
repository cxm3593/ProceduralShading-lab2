#version 150

// Texture shading fragment shader
//
// Uses shading information provided by the vertex shader
// to do Toon shading calculations.
//
// @author  RIT CS Department
// @author  Chengyi Ma

// INCOMING DATA

// Data from the application

// Light color
uniform vec4 lightColor;
uniform vec4 ambientLight;

// Reflectivity
uniform float specExp;
uniform vec3 kCoeff;

// Textures
uniform sampler2D material;

// Data from the vertex shader

// Light position
in vec3 lPos;

// Vertex position (in clip space)
in vec3 vPos;

// Vertex normal
in vec3 vNorm;

// Texture coordinates
//in vec2 texCoord;

// OUTGOING DATA

// The final fragment color
out vec4 fragColor;

float linearstep(float edge0, float edge1, float x)
{
    return  clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
}

void main()
{
    vec3 L = normalize( lPos - vPos );
    vec3 N = normalize( vNorm );


	float gradient_coordinate = linearstep(-1.01, 1.01, dot(N, L));
	vec4 color = texture( material, vec2(gradient_coordinate, 0.0));

    fragColor = color;
}
