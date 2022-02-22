//
//  Textures.cpp
//
//  Simple class for setting up texture mapping parameters.
//
//  Created by Warren R. Carithers 2021/11/11
//  Based on code created by Joe Geigel on 1/23/13.
//  Copyright 2021 Rochester Institute of Technology. All rights reserved.
//
//  Contributor:  Chengyi Ma
//

#include <iostream>

#include "Textures.h"

#include "Models.h"
#include "Lighting.h"
#include "Utils.h"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>

// this is here in case you are using SOIL;
// if you're not, it can be deleted.
#include <SOIL/SOIL.h>

using namespace std;

// reflective coefficients
static glm::vec3 k( 0.5, 0.7f, 1.0f );

// specular highlights (order depends upon the Shape type in Models.h)
static GLfloat specExp[N_SHAPES] = {
    50.0f,  // Cube
    20.0f,  // Cylinder
    80.0f   // Sphere
};

// Add any global definitions and/or variables you need here.

// texture units
static GLint texUnits[N_SHAPES] = {
    0,  // Cube
    1,  // Cylinder
    2  // Sphere
};

// Texture file names
static const char *textures[N_SHAPES] = {
    "blue-0000bb-9999ff.jpg",              // Cube
    "green-006600-88ee88.jpg",   // Cylinder
    "red-bb0000-ff6666.jpg"          // Sphere
};

// Texture IDs
GLuint tids[N_SHAPES];

///
/// Load an image into the current texture unit
///
/// @param file the image file to be loaded
/// @return the texture ID, or 0 on error
///
static GLuint load_texture( const char *file ) {
    GLuint tid;
    unsigned char *image;
    int width, height, channels;

    glGenTextures( 1, &tid );
    glBindTexture( GL_TEXTURE_2D, tid );

    checkErrors( "load_texture gen/bind" );

    image = SOIL_load_image( file, &width, &height, &channels, SOIL_LOAD_AUTO );

    checkErrors( "load_texture SOIL" );

    if( image == nullptr ) {
        cerr << "SOIL loading error for '" << file << "': '"
             << SOIL_last_result() << "'" << endl;
    }

    // invert in Y
    for( int j = 0; j*2 < height; ++j ) {
        int ix1 = j * width * channels;
        int ix2 = (height - 1 - j) * width * channels;
        for( int i = width * channels; i > 0; --i ) {
            unsigned char temp = image[ix1];
            image[ix1] = image[ix2];
            image[ix2] = temp;
            ++ix1;
            ++ix2;
        }
    }

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
            GL_UNSIGNED_BYTE, image );
    checkErrors( "load_texture glTexImage2D" );

    SOIL_free_image_data( image );
    checkErrors( "load_texture SOIL free" );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    checkErrors( "load_texture glTexParameteri" );

    return tid;
}

///
/// This function initializes all texture-related data structures for
/// the program.  This is where texture buffers should be created, where
/// texture images should be loaded, etc.
///
/// You will need to write this function, and maintain all of the values
/// needed to be sent to the various shaders.
///
void initTextures( void )
{
    for( int s = Cube; s < N_SHAPES; ++s ) {
        glActiveTexture( GL_TEXTURE0 + texUnits[s] );
        tids[s] = load_texture( textures[s] );
    }
}

///
/// This function sets up the appearance parameters for the object.
///
/// @param program        The ID of an OpenGL (GLSL) shader program to which
///                       parameter values are to be sent
/// @param obj            The type of object being drawn
///
void setTextures( GLuint program, Shape obj )
{
    // // Set the specular exponent for the object
    // GLint loc  = getUniformLoc( program, "specExp" );
    // if( loc >= 0 ) {
    //     glUniform1f( loc, specExp[obj] );
    // }
    // checkErrors( "setTextures 1" );

    // // Send down the reflective coefficients
    // loc = getUniformLoc( program, "kCoeff" );
    // if( loc >= 0 ) {
    //     glUniform3fv( loc, 1, glm::value_ptr(k) );
    // }
    // checkErrors( "setTextures 2" );

    GLint loc = getUniformLoc( program, "material" );
    GLint unit = texUnits[obj];
    if( loc >= 0 ) {
        glActiveTexture( GL_TEXTURE0 + unit );
        glBindTexture( GL_TEXTURE_2D, tids[obj]  );
        glUniform1i( loc, unit );
    }
    checkErrors( "setTextures 3" );
}
