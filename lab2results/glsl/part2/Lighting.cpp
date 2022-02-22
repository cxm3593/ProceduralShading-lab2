//
//  Lighting
//
//  Simple module for setting up the parameters for lighting and shading
//  for the Shading Assignment.
//
//  Created by Warren R. Carithers 2019/11/18.
//  Updated: 2021/11/07 by wrc
//  Copyright 2021 Rochester Institute of Technology.  All rights reserved.
//

#include "Lighting.h"

#include "Application.h"
#include "Models.h"
#include "Utils.h"

using namespace std;

// lighting data
GLfloat lightcolor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightpos[4]   = { 4.0f, 1.0f, 2.0f, 1.0f };
GLfloat amblight[4]   = { 0.5f, 0.5f, 0.5f, 1.0f };

///
/// This function sets up the lighting, material, and shading parameters
/// for the shaders.
///
/// @param program  The ID of an OpenGL (GLSL) shader program to which
///    parameter values are to be sent
/// @param object   Which object is currently being drawn
///
void setLighting( GLuint program, Shape object )
{
    GLint loc;

    // Lighting parameters
    // loc = getUniformLoc( program, "lightPosition" );
    // if( loc >= 0 ) {
    //     glUniform4fv( loc, 1, lightpos );
    // }

    // loc = getUniformLoc( program, "lightColor" );
    // if( loc >= 0 ) {
    //     glUniform4fv( loc, 1, lightcolor );
    // }

    // loc = getUniformLoc( program, "ambientLight" );
    // if( loc >= 0 ) {
    //     glUniform4fv( loc, 1, amblight );
    // }
}
