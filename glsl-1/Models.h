//
//  Models.h
//
//  Assignment-specific code for objects.
//
//  Created by Warren R. Carithers on 2021/09/02.
//  Based on earlier code created by Joe Geigel and Warren R. Carithers
//  Copyright 2021 Rochester Institute of Technology. All rights reserved.
//
//  This file should not be modified by students.
//

#ifndef MODELS_H_
#define MODELS_H_

#include "Canvas.h"

//
// Object selection
//
typedef
    enum shape_e {
        Cube = 0, Cylinder, Sphere
        // Sentinel gives us the number of shapes
        , N_SHAPES
    } Shape;

//
// PUBLIC GLOBALS
//

// object names
extern const char *objects[ N_SHAPES ];

//
// PUBLIC FUNCTIONS
//

///
/// createObject - create an object to be drawn
///
/// @param object - the object to be created
/// @param C      - the Canvas to use when drawing
///
void createObject( Shape object, Canvas &C );

#endif
