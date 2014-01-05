//
//  _2DMathLib.cpp

//  Created by Jason Paolasini on 2013-09-16.
//  Copyright (c) 2013 Jason Paolasini. All rights reserved.
//

#include "3DMathLib.h"
#include <math.h>

//Point Constructor

point3D::point3D(){
    
}

point3D::point3D(float inX,float inY,float inZ){
    x = inX;
    y = inY;
    z = inZ;
    
}

//vector Constructor
vec3D::vec3D(){
    
}

vec3D::vec3D(float inX, float inY,float inZ){
    
    x=inX;
    y=inY;
    z=inZ;
    
}

plane3D::plane3D(){
    
}


plane3D::plane3D(vec3D inPlaneNorm, point3D inPlaneOrigin){
   
    norm=inPlaneNorm;
    origin = inPlaneOrigin;
    
    
    
    
}


boundingBox::boundingBox(){
    
}

boundingBox::boundingBox(plane3D inFront,plane3D inBack, plane3D inLeft,plane3D inRight, plane3D inTop,plane3D inBottom){
    front = inFront;
    back = inBack;
    left = inLeft;
    right = inRight;
    top = inTop;
    bottom = inBottom;
    
}



// Colour

colour::colour()
{
    
}


colour::colour(float inR, float inG, float inB){
    
    r = inR;
    g = inG;
    b = inB;

}


//Finds the magnitude of a 3D vector

float vec3D::magnitude(){
    float i  = this->x;
    float j  = this->y;
    float k  = this->z;
    
    return sqrtf(powf(i,2)+powf(j, 2)+powf(k,2));
    
    
}

ray::ray(){
    
    
}

ray::ray(point3D inp0, vec3D inp1){
    
    p0 = inp0;
    p1 = inp1;
    
    
    
}


//adds vectors

vec3D vectorAddition(vec3D vec1,vec3D vec2){
    float totalX = vec1.x+vec2.x;
    float totalY = vec1.y+vec2.y;
    float totalZ = vec1.z+vec2.z;
    
    vec3D finalVec(totalX,totalY,totalZ);
    
    return finalVec;
    
}

//Finds the distance between 2 Points

float distance(point3D pt1,point3D pt2){
    return sqrtf(powf((pt2.x-pt1.x), 2)+powf((pt2.y-pt1.y), 2)+powf((pt2.z-pt2.z),2));
}



//Fast Distance Finds the fistance that is relati e, no sqrt fn.


float fastDistance(point3D pt1,point3D pt2){
    return powf((pt2.x-pt1.x), 2)+powf((pt2.y-pt1.y), 2)+powf((pt2.z-pt2.z),2);
}


//Normalize vector

vec3D vec3D::normalize(){
    
    vec3D normal = *new vec3D(x/magnitude(),y/magnitude(),z/magnitude());
    
    return normal;
}


//Scalar Multiply

vec3D vec3D::scalarMultiply(float s){
    
    return vec3D(x*s,y*s,z*s);
}


point3D point3D::scalarMultiply(float s){
    
    return point3D(x*s,y*s,z*s);
}


float vec3D::dot(vec3D vec1){
    float dP = (this->x)*(vec1.x)+(this->y)*(vec1.y)+(this->z)*(vec1.z);
    return dP;
}

float point3D::dot(vec3D vec1){
    float dP = (this->x)*(vec1.x)+(this->y)*(vec1.y)+(this->z)*(vec1.z);
    return dP;
}
    
//Create vector from 2 Points


vec3D createVector(point3D pt1,point3D pt2){
    return vec3D(pt2.x-pt1.x,pt2.y-pt1.y,pt2.z-pt1.z);
}


//shifts a point by the value of a vector that is added to the point, returns a new point

point3D point3D::shift(vec3D vector){
    return point3D(this->x+vector.x,this->y+vector.y,this->z+vector.z);
}

point3D point3D::subtract(point3D point2){
    return point3D(this->x-point2.x,this->y-point2.y,this->z-point2.z);
}
