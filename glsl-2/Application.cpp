//
//  Application.cpp
//
//  Assignment-specific code for this assignment.
//
//  Created by Warren R. Carithers on 2019/09/09.
//  Based on earlier versions created by Joe Geigel and Warren R. Carithers
//  Copyright 2019 Rochester Institute of Technology. All rights reserved.
//
//  This file should not be modified by students.
//

#include <cstdlib>
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

//
// GLEW and GLFW header files also pull in the OpenGL definitions
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Application.h"

#include "Buffers.h"
#include "Canvas.h"
#include "Lighting.h"
#include "Models.h"
#include "ShaderSetup.h"
#include "Textures.h"
#include "Types.h"
#include "Utils.h"
#include "Viewing.h"

using namespace std;

//
// PRIVATE GLOBALS
//

//
// Object information
//

// cube transformations
static glm::vec3 cube_scale(  0.5f, 1.0f, 0.5f );
static glm::vec3 cube_xlate( -1.5f, 0.0f, -1.5f );
static glm::vec3 cube_rotate( 0.0f, 25.0f, 0.0f );

// cylinder transformations
static glm::vec3 cyl_scale( 0.5f, 1.0f, 0.5f );
static glm::vec3 cyl_xlate( 0.0f, 0.0f, -3.5f );
static glm::vec3 cyl_rotate( 0.0f, 0.0f, 0.0f );

// sphere transformations
static glm::vec3 sph_scale( 0.5f, 0.5f, 0.5f );
static glm::vec3 sph_xlate( 1.5f, 0.0f, -0.5f );
static glm::vec3 sph_rotate( 0.0f, 0.0f, 0.0f );

// our Canvas
static Canvas *canvas;

// We need three vertex buffers for our shapes
static BufferSet cube, cylinder, sphere;

// shader program handle
static GLuint program;

// our VAO
static GLuint vao;

// do we need to redraw the objects?
static bool updateDisplay = true;

// names of our GLSL shader files
static const char *vshader = "texture.vert";
static const char *fshader = "texture.frag";

//
// PUBLIC GLOBALS
//

//
// Drawing-related variables
//

// dimensions of the drawing window
int w_width  = 600;
int w_height = 600;

// drawing window title
const char *w_title = (char *) "Lab 2";

// GL context we're using (we assume 3.2, for GLSL 1.50)
int gl_maj = 3;
int gl_min = 2;

// our GLFWwindow
GLFWwindow *w_window;

//
// PRIVATE FUNCTIONS
//

///
/// process the command-line arguments
///
/// @param argc   number of command-line arguments
/// @param argv   command-line argument strings
///
static void args( const int argc, char *argv[] ) {
    // nothing to do for this assignment!
    (void) argc;
    (void) argv;
}

///
/// createImage() - create vertex and element buffers for our shapes
///
/// @param C  - the Canvas to be used
///
static void createImage( Canvas &C )
{
    // clear any previous shape

    canvas->clear();            // clear any previous shape
    createObject( Cube, C );    // create the object
    cube.createBuffers( C );    // create its vertex and element buffers

    // ditto for the others
    canvas->clear();
    createObject( Cylinder, C );
    cylinder.createBuffers( C );

    canvas->clear();
    createObject( Sphere, C );
    sphere.createBuffers( C );
}

///
/// Event callback routines for this assignment
///

///
/// Handle keyboard input
///
/// @param window   GLFW window being used
/// @param key      code for the key that was pressed
/// @param scan     scan code of the key
/// @param action   type of event (press, release, etc.)
/// @param mods     modifiers in use (shift, etc.)
///
static void keyboard( GLFWwindow *window, int key, int scan,
                      int action, int mods )
{
    // only react to "press" events
    if( action != GLFW_PRESS ) {
        return;
    }

    switch( key ) {

    // termination
    case GLFW_KEY_ESCAPE:  // FALL THROUGH
    case GLFW_KEY_Q:
        glfwSetWindowShouldClose( window, 1 );
        break;

    // help message
    case GLFW_KEY_H:
        cout << "  Key(s)             Action" << endl;
        cout << "=========   =======================" << endl;
        cout << "ESC, q, Q   Terminate the program" << endl;
        cout << "  h, H      Print this message" << endl;
        break;

    default:
        cerr << "Unknown keycode " << key << " ignored" << endl;
        // return without updating the display
        return;
        // NOTREACHED

    }

    updateDisplay = true;
}

///
/// Display the current image
///
static void display( void )
{
    // clear the frame buffer
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // ensure we have selected the shader program
    glUseProgram( program );

    // set our titlebar (not needed for this assignment)
    // setTitle();

    // check for any errors to this point
    checkErrors( "display init" );

    // set up the common transformations
    setProjection( program );
    checkErrors( "display projection" );
    setCamera( program );
    checkErrors( "display camera" );

    //
    // First, the cube
    //

    // transformations for the cube
    setTransforms( program, cube_scale, cube_rotate, cube_xlate );
    checkErrors( "display cube xforms" );

    // set up illumination for the cube
    setLighting( program, Cube );
    checkErrors( "display cube lighting" );

    // materials
    setTextures( program, Cube );
    checkErrors( "display cube materials" );

    // bind the object buffers
    cube.selectBuffers( program, "vPosition", NULL, "vNormal", "vTexCoord" );
    checkErrors( "display cube select" );

    glDrawElements( GL_TRIANGLES, cube.numElements,
            GL_UNSIGNED_INT, (void *) 0 );
    checkErrors( "display cube draw" );

    //
    // Next, the cylinder
    //

    // transformations for the cylinder
    setTransforms( program, cyl_scale, cyl_scale, cyl_xlate );
    checkErrors( "display cylinder xforms" );

    setLighting( program, Cylinder );
    checkErrors( "display cylinder lighting" );

    setTextures( program, Cylinder );
    checkErrors( "display cylinder materials" );

    cylinder.selectBuffers( program, "vPosition", NULL, "vNormal", "vTexCoord" );
    checkErrors( "display cylinder select" );

    glDrawElements( GL_TRIANGLES, cylinder.numElements,
            GL_UNSIGNED_INT, (void *) 0 );
    checkErrors( "display cylinder draw" );

    //
    // Finally, the sphere
    //

    setTransforms( program, sph_scale, sph_scale, sph_xlate );
    checkErrors( "display sphere xforms" );

    setLighting( program, Sphere );
    checkErrors( "display sphere lighting" );

    setTextures( program, Sphere );
    checkErrors( "display sphere materials" );

    sphere.selectBuffers( program, "vPosition", NULL, "vNormal", "vTexCoord" );
    checkErrors( "display sphere select" );

    glDrawElements( GL_TRIANGLES, sphere.numElements,
            GL_UNSIGNED_INT, (void *) 0 );
    checkErrors( "display sphere draw" );

}

///
/// OpenGL initialization
///
/// @return boolean indicating status of initialization
///
static bool init( void )
{
    // Check the OpenGL version - we need at least OpenGL 3.2
    // in order to get GLSL 1.50
    if( gl_maj < 3 || (gl_maj == 3 && gl_min < 2) ) {
        cerr << "Caution: GL version may not allow GLSL 1.50+"
             << " code to compile!" << endl;
    }

    // Load shaders and use the resulting shader program
    ShaderError error;
    program = shaderSetup( vshader, fshader, &error );
    if( !program ) {
        cerr << "Error setting up shaders - "
             << errorString(error) << endl;
        return( false );
    }

#if DEBUG
    // this code can be enabled if you need to dump out the
    // shader variables for the shader

    cout << "Shader actives" << endl;
    cout << "----------" << endl << "Shader ID " << program << endl;
    dumpActives( program );
#endif

    glUseProgram( program );

    checkErrors( "init program" );

    // Create our Canvas
    canvas = new Canvas( w_width, w_height );

    if( canvas == NULL ) {
        cerr << "error - cannot create Canvas" << endl;
        return( false );
    }

    // need a VAO if we're using a core context;
    // doesn't hurt even if we're not using one
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );
    checkErrors( "init vao" );

    // OpenGL state initialization
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glDepthFunc( GL_LEQUAL );
    glClearDepth( 1.0f );
    checkErrors( "init setup" );

    // Create all our objects
    createImage( *canvas );
    checkErrors( "init image" );

    // initialize all texture-related things
    initTextures();

    // register our callbacks
    glfwSetKeyCallback( w_window, keyboard );
    checkErrors( "init callback" );

    return( true );
}

///
/// PUBLIC FUNCTIONS
///

///
/// Assignment-specific processing
///
void application( int argc, char *argv[] )
{
    // process command-line arguments
    args( argc, argv );
    
    // set up the objects and the scene
    if( !init() ) {
        return;
    }
    checkErrors( "after init" );

    // loop until it's time to quit
    while( !glfwWindowShouldClose(w_window) ) {
        display();
        glfwSwapBuffers( w_window );
        checkErrors( "event loop" );
        glfwWaitEvents();
    }
}
