/*======================================================================
* COSC363 Computer Graphics
* Assignment 2: Ray Tracer
* File name: RayTracer.cpp
* Author: Adam Ross
*=======================================================================
*/

#include <iostream>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include "Sphere.h"
#include "SceneObject.h"
#include "Ray.h"
#include "Plane.h"
#include "TextureBMP.h"
#include "Tetrahedron.h"
#include "Cylinder.h"
#include <GL/glut.h>

using namespace std;

const float PARAMS = 20.0;
const float EDIST = 40.0;
const int NUMDIV = 500;
const int MAX_STEPS = 5;
const float MIN_COORD = -PARAMS * 0.5;
const float MAX_COORD =  PARAMS * 0.5;
const float PHONG = 10;

vector<SceneObject*> sceneObjects;  // global list containing pointers to objects in the scene
TextureBMP textureMirror;
TextureBMP textureMoss;
TextureBMP textureDeathStar;

float x = 25, y = 20, z = -150; 	// plane co-ordinates
bool aa = true; // turns anti-aliasing on/off


/* Computes the colour value obtained by tracing a ray and finding
 * its closest point of intersection with objects in the scene.
*/
glm::vec3 trace( Ray ray, int step )
{
    glm::vec3 backgroundCol ( 0 );
    glm::vec3 ambientTerm ( 0.2 );
    glm::vec3 lightOne ( 24, -19, -49 );
    glm::vec3 lightTwo ( -24, -19, -49 );
    glm::vec3 specularColour ( 1, 1, 1 );
    ray.closestPt ( sceneObjects );

    if ( ray.xindex == -1 )
    {
        return backgroundCol;
    }
    glm::vec3 colTex;

    if ( ray.xindex == 3 )
    {
        float texcoordt = ( ray.xpt.x - -x ) / ( x - -x );
        float texcoords = ( ray.xpt.z - -x ) / ( z - -x );
        colTex += ( textureMoss.getColorAt ( texcoords, texcoordt ) ) * 0.5f;
    }

    if ( ray.xindex == 5 )
    {
        float texcoords = ( ray.xpt.x - -x ) / ( z - -x );
        float texcoordt = ( ray.xpt.z - -y ) / ( y - -y );
        colTex += ( textureMirror.getColorAt ( texcoords, texcoordt ) ) * 0.5f;
    }

    if ( ray.xindex == 6 )
    {
        float texcoords = ( ray.xpt.z - -x ) / ( z - -x );
        float texcoordt = ( ray.xpt.y - -y + 10 ) / ( y - -y + 10 );
        colTex += ( textureMirror.getColorAt ( texcoords, texcoordt ) ) * 0.5f;
    }

    if ( ray.xindex == 7 )
    {
        float texcoords = ( ray.xpt.z - z ) / ( -x - z );
        float texcoordt = ( ray.xpt.y - -y ) / ( y - -y );
        colTex += ( textureMirror.getColorAt ( texcoords, texcoordt ) ) * 0.5f;
    }
    glm::vec3 colSum;
    glm::vec3 colOne = sceneObjects [ ray.xindex ] -> getColor ();
    glm::vec3 colTwo = sceneObjects [ ray.xindex ] -> getColor ();
    glm::vec3 normalVector = sceneObjects [ ray.xindex ] -> normal ( ray.xpt );

    if ( ray.xindex == 2 )
    {
        float texcoords = 0.5 + ( asin ( normalVector.x ) / M_PI );
        float texcoordt = 0.5 + ( asin ( normalVector.y ) / M_PI );
        colTex += textureDeathStar.getColorAt ( texcoords, texcoordt );
    }
    glm::vec3 lightVectorOne = lightOne - ray.xpt;
    glm::vec3 lightVectorTwo = lightTwo - ray.xpt;
    float lightDistOne = glm::length ( lightVectorOne );
    float lightDistTwo = glm::length ( lightVectorTwo );
    lightVectorOne = glm::normalize ( lightVectorOne );
    lightVectorTwo = glm::normalize ( lightVectorTwo );
    float lDotnOne = glm::dot ( lightVectorOne, normalVector );
    float lDotnTwo = glm::dot ( lightVectorTwo, normalVector );
    Ray shadowOne ( ray.xpt, lightVectorOne );
    Ray shadowTwo ( ray.xpt, lightVectorTwo );
    shadowOne.closestPt ( sceneObjects );
    shadowTwo.closestPt ( sceneObjects );
    glm::vec3 reflVectorOne = glm::reflect ( -lightVectorOne, normalVector );
    glm::vec3 reflVectorTwo = glm::reflect ( -lightVectorTwo, normalVector );
    float rDotvOne = glm::dot ( reflVectorOne, -ray.dir ); // normalVector );
    float rDotvTwo = glm::dot ( reflVectorTwo, -ray.dir ); // normalVector );
    float specularTermOne;
    float specularTermTwo;

    if ( lDotnOne <= 0 || ( shadowOne.xindex > -1 && shadowOne.xdist < lightDistOne ) )
    {
        colOne = ambientTerm * colOne;
    }
    else
    {
        if ( rDotvOne < 0 )
        {
            specularTermOne = 0;
        }
        else
        {
            specularTermOne = pow ( rDotvOne, PHONG );
        }
        colOne = specularTermOne * specularColour + ( ( ambientTerm + lDotnOne ) * colOne ) + colTex;
    }

    if ( lDotnTwo <= 0 || ( shadowTwo.xindex > -1 && shadowTwo.xdist < lightDistTwo ) )
    {
        colTwo = ambientTerm * colTwo;
    }
    else
    {
        if ( rDotvTwo < 0 )
        {
            specularTermTwo = 0;
        }
        else
        {
            specularTermTwo = pow ( rDotvTwo, PHONG );
        }
        colTwo = specularTermTwo * specularColour + ( ( ambientTerm + lDotnTwo ) * colTwo ) + colTex;
    }
    colSum += ( colOne + colTwo ) * 0.5f;

    if ( ( ray.xindex == 0 || ( ray.xindex > 4 && ray.xindex < 8 ) ) && step < MAX_STEPS )
    {
        glm::vec3 reflectedDir = glm::reflect ( ray.dir, normalVector );
        Ray reflectedRay ( ray.xpt, reflectedDir );
        glm::vec3 reflectedCol = trace ( reflectedRay, step + 1 );
        colSum += ( 0.8f * reflectedCol );
    }

    if ( ray.xindex == 1 && step < MAX_STEPS )
    {
        float eta = 1.003;
        glm::vec3 g = glm::refract ( ray.dir, normalVector, 1.0f / eta );
        Ray refrRay1 ( ray.xpt, g );
        refrRay1.closestPt ( sceneObjects );
        glm::vec3 m = sceneObjects [ refrRay1.xindex ] -> normal ( refrRay1.xpt );
        glm::vec3 h = glm::refract ( g, -m, eta );
        Ray refrRay2 ( refrRay1.xpt, h );
        glm::vec3 refractedCol = trace ( refrRay2, step + 1 );
        colSum += refractedCol * ( glm::vec3 ( 0.99 ) );
    }
    return colSum;
}


void antiAliasing ( glm::vec3 eye, float cellXY, float xp, float yp )
{
    glm::vec3 col;
    glm::vec3 dirA ( xp + 0.25 * cellXY, yp + 0.25 * cellXY, -EDIST );
    Ray rayA = Ray ( eye, dirA );
    rayA.normalize ();
    glm::vec3 colA = trace ( rayA, 1 );
    glm::vec3 dirB ( xp + 0.25 * cellXY, yp + 0.75 * cellXY, -EDIST );
    Ray rayB = Ray ( eye, dirB );
    rayB.normalize ();
    glm::vec3 colB = trace ( rayB, 1 );
    glm::vec3 dirC ( xp + 0.75 * cellXY, yp + 0.25 * cellXY, -EDIST );
    Ray rayC = Ray ( eye, dirC );
    rayC.normalize ();
    glm::vec3 colC = trace ( rayC, 1 );
    glm::vec3 dirD ( xp + 0.75 * cellXY, yp + 0.75 * cellXY, -EDIST );
    Ray rayD = Ray ( eye, dirD );
    rayD.normalize ();
    glm::vec3 colD = trace ( rayD, 1 );
    col.r = ( colA.r + colB.r + colC.r + colD.r ) / 4;
    col.g = ( colA.g + colB.g + colC.g + colD.g ) / 4;
    col.b = ( colA.b + colB.b + colC.b + colD.b ) / 4;
    glColor3f ( col.r, col.g, col.b );
}


/* In a ray tracing application, this function displays the ray
 * traced image by drawing each cell as a quad.
*/
void display ()
{
    float xp, yp, cellXY = ( MAX_COORD-MIN_COORD ) / NUMDIV;
    glm::vec3 eye ( 0., 0., 0. );
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glMatrixMode ( GL_MODELVIEW );
    glLoadIdentity ();
    glBegin ( GL_QUADS );

    for ( int i = 0; i < NUMDIV; i++ )
    {
        xp = MIN_COORD + i * cellXY;

        for ( int j = 0; j < NUMDIV; j++ )
        {
            yp = MIN_COORD + j * cellXY;
            if ( aa )
            {
                antiAliasing ( eye, cellXY, xp, yp );
            }
            else
            {
                glm::vec3 dir ( xp + 0.5 * cellXY, yp + 0.5 * cellXY, -EDIST );
                Ray ray = Ray ( eye, dir );
                ray.normalize ();
                glm::vec3 col = trace ( ray, 1 );
                glColor3f ( col.r, col.g, col.b );
            }
            glVertex2f ( xp, yp );
            glVertex2f ( xp + cellXY, yp );
            glVertex2f ( xp + cellXY, yp + cellXY );
            glVertex2f ( xp, yp + cellXY );
        }
    }
    glEnd ();
    glFlush ();
}


void drawCylinder ()
{
    glm::vec3 colCol = glm::vec3 ( 255.0, 255.0, 240.0 );
    Cylinder *col19 = new Cylinder ( glm::vec3 ( 19.5, -16, -145), 2.0f, 34.0f, colCol );
    sceneObjects.push_back ( col19 );
    Cylinder *col20 = new Cylinder ( glm::vec3 ( 19.5, -18, -145 ), 3.0f, 2.0f, colCol );
    sceneObjects.push_back ( col20 );
    Cylinder *col21 = new Cylinder ( glm::vec3 ( 19.5, 16, -145 ), 3.0f, 2.0f, colCol );
    sceneObjects.push_back ( col21 );
    Cylinder *col22 = new Cylinder ( glm::vec3 ( 19.5, -20, -145 ), 4.0f, 2.0f, colCol );
    sceneObjects.push_back ( col22 );
    Cylinder *col23 = new Cylinder ( glm::vec3 ( 19.5, 18, -145 ), 4.0f, 2.0f, colCol );
    sceneObjects.push_back ( col23 );

    Cylinder *col24 = new Cylinder ( glm::vec3 ( -19.5, -16, -145 ), 2.0f, 34.0f, colCol );
    sceneObjects.push_back ( col24 );
    Cylinder *col25 = new Cylinder ( glm::vec3 ( -19.5, -18, -145 ), 3.0f, 2.0f, colCol );
    sceneObjects.push_back ( col25 );
    Cylinder *col26 = new Cylinder ( glm::vec3 ( -19.5, 16, -145 ), 3.0f, 2.0f, colCol );
    sceneObjects.push_back ( col26 );
    Cylinder *col27 = new Cylinder ( glm::vec3 ( -19.5, -20, -145 ), 4.0f, 2.0f, colCol );
    sceneObjects.push_back ( col27 );
    Cylinder *col28 = new Cylinder ( glm::vec3 ( -19.5, 18, -145 ), 4.0f, 2.0f, colCol );
    sceneObjects.push_back ( col28 );
}


void drawTetrahedron ()
{
    Tetrahedron *side15 = new Tetrahedron ( glm::vec3 ( -15, 5, -100 ), glm::vec3 ( -5, 5, -100 ), glm::vec3 ( -10, 15, -100 ), glm::vec3 ( 255.0, 8.0, 0.0 ) );
    sceneObjects.push_back ( side15 );
    Tetrahedron *side16 = new Tetrahedron ( glm::vec3 ( -10, 5, -90 ), glm::vec3 ( -5, 5, -100 ), glm::vec3 ( -10, 15, -100 ), glm::vec3 ( 64.3, 0.0, 0.0 ) );
    sceneObjects.push_back ( side16 );
    Tetrahedron *side17 = new Tetrahedron ( glm::vec3 ( -15, 5, -100 ), glm::vec3 ( -10, 5, -90 ), glm::vec3 ( -10, 15, -100 ), glm::vec3 ( 0.8, 1.0, 0.0 ) );
    sceneObjects.push_back ( side17 );
    Tetrahedron *side18 = new Tetrahedron ( glm::vec3 ( -15, 5, -100 ), glm::vec3 ( -5, 5, -100 ), glm::vec3 ( -10, 5, -90 ), glm::vec3 ( 0.54, 0.81, 0.94 ) );
    sceneObjects.push_back ( side18 );
}


void drawBox ()
{
    float minX = -17.5, maxX = -10, minY = -20, maxY = -12.5, minZ = -87.5, maxZ = -95;
    glm::vec3 boxCol = glm::vec3 ( 0.8, 0.4, 0.1 ); //Orange
    Plane *boxTop9 = new Plane ( glm::vec3 ( minX, maxY, maxZ ), glm::vec3 ( minX, maxY, minZ ), glm::vec3 ( maxX, maxY, minZ ), glm::vec3 ( maxX, maxY, maxZ ), boxCol );
    sceneObjects.push_back ( boxTop9 );
    Plane *boxBottom10 = new Plane ( glm::vec3 ( minX, minY, minZ ), glm::vec3 ( minX, minY, maxZ ), glm::vec3 ( maxX, minY, maxZ ), glm::vec3 ( maxX, minY, minZ ), boxCol );
    sceneObjects.push_back ( boxBottom10 );
    Plane *boxRightSide11 = new Plane ( glm::vec3 ( maxX, maxY, minZ ), glm::vec3 ( maxX, minY, minZ ), glm::vec3 ( maxX, minY, maxZ ), glm::vec3 ( maxX, maxY, maxZ ), boxCol );
    sceneObjects.push_back ( boxRightSide11 );
    Plane *boxLeftSide12 = new Plane ( glm::vec3 ( minX, maxY, maxZ ), glm::vec3 ( minX, minY, maxZ ), glm::vec3 ( minX, minY, minZ ), glm::vec3 ( minX, maxY, minZ ), boxCol );
    sceneObjects.push_back ( boxLeftSide12 );
    Plane *boxFront13 = new Plane ( glm::vec3 ( minX, minY, minZ ), glm::vec3 ( maxX, minY, minZ ), glm::vec3 ( maxX, maxY, minZ ), glm::vec3 ( minX, maxY, minZ ), boxCol );
    sceneObjects.push_back ( boxFront13 );
    Plane *boxBack14 = new Plane ( glm::vec3 ( minX, maxY, maxZ ), glm::vec3 ( maxX, maxY, maxZ ), glm::vec3 ( maxX, minY, maxZ ), glm::vec3 ( minX, minY, maxZ ), boxCol );
    sceneObjects.push_back ( boxBack14 );
}


void drawPlanes ()
{
    glm::vec3 planeCol = glm::vec3 ( 0.5, 0.5, 0 );
    Plane *planeFloor4 = new Plane ( glm::vec3 ( -x, -y, -x ), glm::vec3 ( x, -y, -x ), glm::vec3 ( x, -y, z ), glm::vec3 ( -x, -y, z ), planeCol );
    sceneObjects.push_back ( planeFloor4 );
    Plane *planeBackWall5 = new Plane ( glm::vec3 ( -x, -y, z ), glm::vec3 ( x, -y, z ), glm::vec3 ( x, y, z ), glm::vec3 ( -x, y, z ), planeCol );
    sceneObjects.push_back ( planeBackWall5 );
    Plane *planeCeiling6 = new Plane ( glm::vec3 ( -x, y, z ), glm::vec3 ( x, y, z ), glm::vec3 ( x, y, -x ), glm::vec3 ( -x, y, -x ), planeCol );
    sceneObjects.push_back ( planeCeiling6 );
    Plane *planeCeiling7 = new Plane ( glm::vec3 ( -x, -y, -x ), glm::vec3 ( -x, -y, z ), glm::vec3 ( -x, y, z ), glm::vec3 ( -x, y, -x ), planeCol );
    sceneObjects.push_back ( planeCeiling7 );
    Plane *planeCeiling8 = new Plane ( glm::vec3 ( x, -y, z ), glm::vec3 ( x, y, z ), glm::vec3 ( x, y, -x ), glm::vec3 ( x, -y, -x ), planeCol );
    sceneObjects.push_back ( planeCeiling8 );
}


void drawSpheres ()
{
    Sphere *sphere1 = new Sphere ( glm::vec3 ( 0, -2.5, -115.0 ), 17.5, glm::vec3 ( 0.471, 0.318, 0.613 ) );
    sceneObjects.push_back ( sphere1 );
    Sphere *sphere2 = new Sphere ( glm::vec3 ( 9, -12.5, -85.0 ), 7.0, glm::vec3 ( 0.1, 0.8, 0.4 ) );
    sceneObjects.push_back ( sphere2 );
    Sphere *sphere3 = new Sphere ( glm::vec3 ( 12, 8.0, -90.0 ), 4.0, glm::vec3 ( 0.5, 0.5, 0.5 ) );
    sceneObjects.push_back ( sphere3 );
}


/* Specifically, it creates scene objects and adds them to the list
 * of scene objects. It also initializes the OpenGL orthographc
 * projection matrix for drawing the the ray traced image.
 */
//----------------------------------------------------------------------------------
void initialize ()
{
    textureMirror = TextureBMP ( ( char* const ) "mirror.bmp" );
    textureMoss = TextureBMP ( ( char* const ) "Moss.bmp" );
    textureDeathStar = TextureBMP ( ( char* const ) "ds.bmp" );
    glMatrixMode ( GL_PROJECTION );
    gluOrtho2D ( MIN_COORD, MAX_COORD, MIN_COORD, MAX_COORD );
    glClearColor ( 0, 0, 0, 1 );
    drawSpheres ();
    drawPlanes ();
    drawBox ();
    drawTetrahedron ();
    drawCylinder ();
}


int main ( int argc, char *argv[] )
{
    glutInit ( &argc, argv );
    glutInitDisplayMode ( GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize ( 500, 500 );
    glutInitWindowPosition ( 650, 250 );
    glutCreateWindow ( "COSC363 Assignment 2: Ray Tracer" );
    initialize ();
    glutDisplayFunc ( display );
    glutMainLoop ();
    return 0;
}
