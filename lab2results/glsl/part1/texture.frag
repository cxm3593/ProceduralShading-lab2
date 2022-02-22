#version 150

// Texture shading fragment shader
//
// Uses shading information provided by the vertex shader
// to do Phong shading calculations.
//
// @author  RIT CS Department
// @author  YOUR_NAME_HERE

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
// Modified: opacity_texture
uniform sampler2D opacity_texture;


// Data from the vertex shader

// Light position
in vec3 lPos;

// Vertex position (in clip space)
in vec3 vPos;

// Vertex normal
in vec3 vNorm;

// Texture coordinates
in vec2 texCoord;

// OUTGOING DATA

// The final fragment color
out vec4 fragColor;


void main()
{
    // calculate lighting vectors
    vec3 L = normalize( lPos - vPos );
    vec3 N = normalize( vNorm );
    vec3 R = normalize( reflect(-L, N) );
    vec3 V = normalize( -(vPos) );

    // get the color data from the texture image
    vec4 matColor = texture( material, texCoord );

	// Modified get the opacity value from the opacity texture image
	vec4 opaTex_color = texture(opacity_texture, texCoord);
	float opacity = opaTex_color.r;

    // calculate components
    vec4 ambient = matColor * ambientLight;
    vec4 diffuse = matColor * lightColor * max(dot(N,L),0.0);
    float specDot = pow( max(dot(R,V),0.0), specExp );
    vec4 specular = matColor * lightColor * specDot;

    // calculate the final color
    vec4 color = (kCoeff.x * ambient) +
                 (kCoeff.y * diffuse) +
                 (kCoeff.z * specular);
	
	// Modified: add opacity
	color[3] = opacity;

    fragColor = color;
}
