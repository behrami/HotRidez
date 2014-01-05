 //
//  Particle.h
//  C++ Tutorial
//
//  Created by Jason Paolasini on 2013-09-29.
//  Copyright (c) 2013 Jason Paolasini. All rights reserved.
//

#ifndef __C___Tutorial__Particle__
#define __C___Tutorial__Particle__

#include <iostream>
#include "3DMathLib.h"



class object
{
    public:
        object();
        object(point3D inPoint,float inDirection,vec3D inSpeed,char inMaterial,boundingBox inBoundingBox,bool isSelected);

        point3D location;
        float direction;
        vec3D speed;
        char material;
        char shape;
        boundingBox box;
        bool selected;  
    
    
    
    
    
};
#endif /* defined(__C___Tutorial__Particle__) */

