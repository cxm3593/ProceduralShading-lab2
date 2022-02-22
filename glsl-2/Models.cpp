//
//  Models.cpp
//
//  Assignment-specific code for objects.  This module will typically be
//  heavily modified to fit the needs of each assignment.
//
//  Created by Warren R. Carithers on 2021/09/02.
//  Based on earlier versions created by Joe Geigel and Warren R. Carithers
//  Copyright 2021 Rochester Institute of Technology. All rights reserved.
//
//  This file should not be modified by students.
//

#include <iostream>
#include <cmath>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#include "Models.h"

#include "ObjectLoader.h"

using namespace std;

//
// PRIVATE GLOBALS
//

// object loader data
static objl::Loader cubeData;
static objl::Loader cylinderData;
static objl::Loader sphereData;

// names of our object files
static const char *cubeFile = "cube.obj";
static const char *cylinderFile = "cylinder.obj";
static const char *sphereFile = "sphere.obj";

//
// PUBLIC GLOBALS
//

// object names (must match the sequence in Models.h)
const char *objects[ N_SHAPES ] = {
    "Cube", "Cylinder", "Sphere"
};

//
// PRIVATE FUNCTIONS
//

///
/// makeObject - internal function to create an object
///
/// @param object - the object to be created
/// @param which  - which model we want within that data
/// @param C      - the Canvas to use when drawing
///
static void makeObject( objl::Loader object, int which, Canvas &C )
{
    objl::Mesh mesh = object.LoadedMeshes[which];

    for( int i = 0; i < mesh.Indices.size(); i += 3 ) {
        int i1 = mesh.Indices[i];
        int i2 = mesh.Indices[i+1];
        int i3 = mesh.Indices[i+2];

        Vertex v1 = { mesh.Vertices[i1].Position.X,
                      mesh.Vertices[i1].Position.Y,
                      mesh.Vertices[i1].Position.Z };

        Vertex v2 = { mesh.Vertices[i2].Position.X,
                      mesh.Vertices[i2].Position.Y,
                      mesh.Vertices[i2].Position.Z };

        Vertex v3 = { mesh.Vertices[i3].Position.X,
                      mesh.Vertices[i3].Position.Y,
                      mesh.Vertices[i3].Position.Z };

#if defined(USE_CALCULATED_NORMALS)
        // let addTriangle() calculate normals
        C.addTriangle( v1, v2, v3 );
#else
        // use normals from the .obj file
        Normal n1 = { mesh.Vertices[i1].Normal.X,
                      mesh.Vertices[i1].Normal.Y,
                      mesh.Vertices[i1].Normal.Z };

        Normal n2 = { mesh.Vertices[i2].Normal.X,
                      mesh.Vertices[i2].Normal.Y,
                      mesh.Vertices[i2].Normal.Z };

        Normal n3 = { mesh.Vertices[i3].Normal.X,
                      mesh.Vertices[i3].Normal.Y,
                      mesh.Vertices[i3].Normal.Z };

        C.addTriangleWithNorms( v1, n1, v2, n2, v3, n3 );
#endif

#if defined(USE_OBJ_TEXCOORDS)
        TexCoord t1 = { mesh.Vertices[i1].TextureCoordinate.X,
                        mesh.Vertices[i1].TextureCoordinate.Y };

        TexCoord t2 = { mesh.Vertices[i2].TextureCoordinate.X,
                        mesh.Vertices[i2].TextureCoordinate.Y };

        TexCoord t3 = { mesh.Vertices[i3].TextureCoordinate.X,
                        mesh.Vertices[i3].TextureCoordinate.Y };

        C.addTextureCoords( t1, t2, t3 );
#endif
    }
}

//
// PUBLIC FUNCTIONS
//

///
/// createObject - load data for an object and create it in the Canvas
///
/// @param object - which object to use
/// @param C      - the Canvas to draw into
void createObject( Shape object, Canvas &C )
{
    switch( object ) {

    case Cube:
        if( !cubeData.LoadFile( cubeFile ) ) {
            cerr << "Error loading " << cubeFile << endl;
        } else {
            makeObject( cubeData, 0, C );
        }
        break;

    case Cylinder:
        if( !cylinderData.LoadFile( cylinderFile ) ) {
            cerr << "Error loading " << cylinderFile << endl;
        } else {
            makeObject( cylinderData, 0, C );
        }
        break;

    case Sphere:
        if( !sphereData.LoadFile( sphereFile ) ) {
            cerr << "Error loading " << sphereFile << endl;
        } else {
            makeObject( sphereData, 0, C );
        }
        break;

    default:
        cerr << "ERROR unknown object " << object << " - ignoring" << endl;
    }
}
