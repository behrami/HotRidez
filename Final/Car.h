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



class car
{
public:
    car();
    car(point3D inPoint,char inShape,float xScale,float yScale,float zScale,char inMaterial,boundingBox inBoundingBox,bool isSelected);
    float size;
    point3D location;
    float scaleX;
    float scaleY;
    float scaleZ;
    char material;
    char shape;
    boundingBox box;
    bool selected;
    
    
    
    
    
};
#endif /* defined(__C___Tutorial__Particle__) */
