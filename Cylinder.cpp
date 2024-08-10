/*======================================================================
* COSC363 Computer Graphics
* Assignment 2: Ray Tracer
* File name: Cylinder.cpp
* Author: Adam Ross
*
* The Cylinder class
* This is a subclass of Object, and hence implements the
* methods intersect() and normal().
*=======================================================================
*/

#include "Cylinder.h"
#include <math.h>


/**
* Cylinder's intersection method.  The input is a ray (pos, dir).
*/
float Cylinder::intersect ( glm::vec3 posn, glm::vec3 dir )
{
    float a = pow ( dir.x, 2.0 ) + pow ( dir.z, 2.0 );
    float b = 2 * ( dir.x * ( posn.x - o.x ) + dir.z * ( posn.z - o.z ) );
    float c = pow ( posn.x - o.x, 2.0 ) + pow ( posn.z - o.z, 2.0 ) - pow ( r, 2.0 );
    float delta = b * b - a * c * 4.0;

    if ( fabs ( delta ) < 0.001 )
    {
        return -1.0;
    }
    if ( delta < 0.0 )
    {
         return -1.0;
    }
    float t1 = ( -b - sqrt ( delta ) ) / ( a * 2.0 );
    float t2 = ( -b + sqrt ( delta ) ) / ( a * 2.0 );

    if ( t1 < 0 && t2 < 0 )
    {
        return -1;
    }

    if ( t1 > 0 && t2 < 0 )
    {
        t2 = t1;
    }

    if ( fabs ( t1 ) > fabs ( t2 ) )
    {
        t2 = t1;
    }

    if ( ( ( posn.y + t2 * dir.y ) >= o.y ) && ( ( posn.y + t2 * dir.y ) <= o.y + h ) )
    {
        return t2;
    }
    if ( ( ( posn.y + t1 * dir.y ) >= o.y ) && ( ( posn.y + t1 * dir.y ) <= o.y + h ) )
    {
        return t1;
    }
    return -1.0;
}


/**
* Returns the unit normal vector at a given point.
* Assumption: The input point p lies on the sphere.
*/
glm::vec3 Cylinder::normal ( glm::vec3 p )
{
    glm::vec3 n ( ( p.x - o.x ) / r, 0, ( p.z - o.z ) / r );
    n = glm::normalize ( n );
    return n;
}
