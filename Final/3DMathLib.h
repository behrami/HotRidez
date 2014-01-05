//
//  _2DMathLib.h
//
//  Created by Jason Paolasini on 2013-09-16.
//  Copyright (c) 2013 Jason Paolasini. All rights reserved.
//


#ifndef _H__3DMathLib_
#define _H__3DMathLib_



//creates a 3d vector

class vec3D{
    public:
        vec3D();
        vec3D(float inX, float inY,float inZ);
        float x;
        float y;
        float z;
        float magnitude();
        vec3D normalize();
        vec3D scalarMultiply(float s);
        float dot(vec3D);
    
    
};

//creates a 3d point

class point3D{
    public:
        point3D();
        point3D(float inX, float inY,float inZ);
        float x;
        float y;
        float z;
        point3D shift(vec3D vector);
        float dot(vec3D);
        point3D subtract(point3D point);
        point3D scalarMultiply(float s);
    
    
};

class plane3D{
public:
    plane3D();
    plane3D(vec3D inPlaneNorm, point3D inPlaneOrigin);
    vec3D norm;
    point3D origin;

    
};

//color attribute


class colour{
public:
    colour();
    colour(float inR,float inG,float inB);
    float r;
    float g;
    float b;
    
};


class boundingBox{
    public :
    boundingBox();
    boundingBox(plane3D inFront,plane3D inBack, plane3D inLeft,plane3D inRight, plane3D inTop,plane3D inBottom);
    plane3D front;
    plane3D back;
    plane3D left;
    plane3D right;
    plane3D top;
    plane3D bottom;
    

};

class ray{
public:
    ray();
    ray(point3D inp0, vec3D inp1);
    point3D p0;
    vec3D p1;

};
class _3DMathLib{
    
    _3DMathLib();
    
    
public:
    static float distance(point3D start,point3D end);
    

    
};


//The following are functions that were created for the MathLib



//Distance

float distance(point3D pt1,point3D pt2);


//Fast Distance
float fastDistance(point3D pt1,point3D pt2);


//Able to create a vector from 2 given points.

vec3D createVector(point3D pt1,point3D pt2);

//vec Addition

vec3D vectorAddition(vec3D vec1,vec3D vec2);



#endif /* defined(__C___Tutorial___DMathLib__) */

