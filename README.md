# COSC363 Ray Tracing Scene

C++ OpenGL rendered ray tracing scene containing diverse geometric objects and global illumination features:

* multiple light sources
* ambient, diffuse and specular lighting
* shadows
* anti-aliasing
* refractions 
* reflective sphere, side walls and ceiling with mirror textures 
* textured planar surface (moldy floor)
* textured non-planar object (death star)
* cylinder primitives (two Roman columns, or eight including reflections)
* cube object 
* tetrahedron object 
* transparent object (green sphere)

# Instructions

## Requirements

* OpenGL
* GLUT
* GLM
* excluded files

## Build

```bash
g++ -o RayTracingScene RayTracer.cpp Ray.cpp SceneObject.cpp Sphere.cpp Plane.cpp TextureBMP.cpp Tetrahedron.cpp Cylinder.cpp -lm -lGL -lGLU -lglut
```

## Run

```bash
./RayTracingScene
```
