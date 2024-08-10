/*======================================================================
* COSC363 Computer Graphics
* Assignment 2: Ray Tracer
* File name: Tetrahedron.h
* Author: Adam Ross
*
* The Tetrahedron class
* This is a subclass of Object, and hence implements the
* methods intersect() and normal().
*=======================================================================
*/
#ifndef H_TETRAHEDRON
#define H_TETRAHEDRON

#include <glm/glm.hpp>
#include "SceneObject.h"

class Tetrahedron : public SceneObject
{
private:
    glm::vec3 a, b, c;  //The three vertices

public:
    Tetrahedron ( void );
    Tetrahedron ( glm::vec3 pa, glm::vec3 pb, glm::vec3 pc, glm::vec3 col )
        : a ( pa ), b ( pb ), c ( pc )
    {
        color = col;
    };
    bool isInside ( glm::vec3 pt );
    float intersect ( glm::vec3 posn, glm::vec3 dir );
    glm::vec3 normal ( glm::vec3 pt );
};
#endif//!H_TETRAHEDRON
