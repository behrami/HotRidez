//
//  Particle.cpp
//  C++ Tutorial
//
//  Created by Jason Paolasini on 2013-09-29.
//  Copyright (c) 2013 Jason Paolasini. All rights reserved.
//

#include "Object.h"
#include "3DMathLib.h"


object::object(){
    
}
/*The particle class was created to keep track of the attributes each particle has in the system
 */

object::object(point3D inPoint,float inDirection,vec3D inSpeed,char inMaterial,boundingBox inBoundingBox,bool isSelected){
    location = inPoint;
    material = inMaterial;
    box = inBoundingBox;
    selected = isSelected;
    speed = inSpeed;
    direction = inDirection;
    
    
    
    
}

