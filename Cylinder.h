/*======================================================================
* COSC363 Computer Graphics
* Assignment 2: Ray Tracer
* File name: Cylinder.h
* Author: Adam Ross
*
* The Cylinder class
* This is a subclass of Object, and hence implements the
* methods intersect() and normal().
*=======================================================================
*/

#ifndef H_CYLINDER
#define H_CYLINDER

#include <glm/glm.hpp>
#include "SceneObject.h"

class Cylinder : public SceneObject
{
private:
    glm::vec3 o;
    float r;
    float h;


public:
    Cylinder ( glm::vec3 o, float r, float h, glm::vec3 col )
        : o ( o ), r ( r ), h ( h )
    {
        color = col;
    };
    float intersect ( glm::vec3 posn, glm::vec3 dir );
    glm::vec3 normal ( glm::vec3 pt );
};
#endif //!H_CYLINDER
