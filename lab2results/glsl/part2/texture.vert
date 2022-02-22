#version 150

// Texture shading vertex shader
//
// Does all vertex transformation, and set up relevant
// shading vectors to be passed to the fragment shader.
//
// @author  RIT CS Department
// @author  YOUR_NAME_HERE
//

// INCOMING DATA

// Data from the application

// Vertex position (in model space)
in vec4 vPosition;

// Normal vector at vertex (in model space)
in vec3 vNormal;

// Texture coordinate
in vec2 vTexCoord;

// Transformations
uniform mat4 viewMat;  // view (camera)
uniform mat4 projMat;  // projection

uniform mat4 modelMat; // composite

// Light position is given in world space
uniform vec4 lightPosition;

// OUTGOING DATA

// Vectors to "attach" to vertex and get sent to fragment shader
// Vectors and points will be passed in "eye" space
out vec3 lPos;
out vec3 vPos;
out vec3 vNorm;

// Texture coordinate
//out vec2 texCoord;

void main()
{
    // create the modelview matrix
    mat4 modelViewMat = viewMat * modelMat;

    // All vectors need to be converted to "eye" space
    // All vectors should also be normalized
    vec4 vertexInEye = modelViewMat * vPosition;
    vec4 lightInEye = viewMat * lightPosition;

    // "Correct" way to transform normals.  The normal matrix is the
    // inverse transpose of the upper left 3x3 submatrix of the modelView
    // matrix (i.e., does not include translations).  THIS IS EXPENSIVE
    // TO COMPUTE because of the inverse(), and should really be done in
    // the application, not here.
    mat3 normMat = inverse( transpose( mat3(modelViewMat) ) );
    vec4 normalInEye = vec4( normMat * vNormal, 0.0 );

    // pass our vertex data to the fragment shader
    lPos = lightInEye.xyz;
    vPos = vertexInEye.xyz;
    vNorm = normalInEye.xyz;

    // pass the texture coordinates
    //texCoord = vTexCoord;

    // send the vertex position into clip space
    gl_Position =  projMat * modelViewMat * vPosition;
}
