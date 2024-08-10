/*======================================================================
* COSC363 Computer Graphics
* Assignment 2: Ray Tracer
* File name: Tetrahedron.cpp
* Author: Adam Ross
*
* The Tetrahedron class
* This is a subclass of Object, and hence implements the
* methods intersect() and normal().
*=======================================================================
*/

#include "Tetrahedron.h"
#include <math.h>


/**
* Checks if a point pt is inside the current polygon
* Implement a point inclusion test using
* member variables a, b, c.
*/
bool Tetrahedron::isInside ( glm::vec3 pt )
{
    glm::vec3 crossA = glm::cross ( b - a, pt - a );
    glm::vec3 crossB = glm::cross ( c - b, pt - b );
    glm::vec3 crossC = glm::cross ( a - c, pt - c );
    glm::vec3 n = normal ( pt );
    float dotA = glm::dot ( crossA, n );
    float dotB = glm::dot ( crossB, n );
    float dotC = glm::dot ( crossC, n );

    if ( dotA >= 0 && dotB >= 0 && dotC >= 0 )
    {
        return true;
    }
    return false;
}


/**
* Tetrahedron's intersection method.  The input is a ray (pos, dir).
*/
float Tetrahedron::intersect ( glm::vec3 posn, glm::vec3 dir )
{
    glm::vec3 n = normal ( posn );
    glm::vec3 vdif = a - posn;
    float vdotn = glm::dot ( dir, n );

    if ( fabs ( vdotn ) < 1.e-4 )
    {
        return -1;
    }
    float t = glm::dot ( vdif, n ) / vdotn;

    if ( fabs ( t ) < 0.0001 )
    {
        return -1;
    }
    glm::vec3 q = posn + dir * t;

    if ( isInside ( q ) )
    {
        return t;
    }
    return -1;
}


/**
* Returns the unit normal vector at a given point.
* Compute the Tetrahedron's normal vector using
* member variables a, b, c.
* The parameter pt is a dummy variable and is not used.
*/
glm::vec3 Tetrahedron::normal ( glm::vec3 pt )
{
    glm::vec3 n = glm::cross ( b - a, c - a );
    return glm::normalize ( n );
}
