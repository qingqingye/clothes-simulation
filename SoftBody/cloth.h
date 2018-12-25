/*
Orchid source code is Copyright(c) 2018
Li Wei.
This file is part of Orchid.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:
- Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifdef _MSC_VER
#pragma once
#endif

#ifndef _CLOTH_
#define _CLOTH_


#include <iostream>
#include <array>


#include <iomanip>
#include <sstream>
#include <algorithm>
#include "../core/common.h"
#include "../math/vec3.h"
#include "../object/object.h"
#include "../constrain/constrain.h"

namespace ClothSimulation
{

	class Cloth
	{
	private:

		int num_objects_width; // number of objects in "width" direction
		int num_objects_height; // number of objects in "height" direction
								// total number of objects is num_objects_width*num_objects_height

		std::vector<Object> objects; // all objects that are part of this cloth
		std::vector<Constrain> Constrains; // all constraints between objects as part of this cloth

		Object* getObject(int x, int y);
		void makeConstrain(Object *p1, Object *p2);


		/* A private method used by drawShaded() and addWindForcesForTriangle() to retrieve the
		normal vector of the triangle defined by the position of the objects p1, p2, and p3.
		The magnitude of the normal vector is equal to the area of the parallelogram defined by p1, p2 and p3
		*/
		Vec3 calcTriangleNormal(Object *p1, Object *p2, Object *p3);

		/* A private method used by windForce() to calcualte the wind force for a single triangle
		defined by p1,p2,p3*/
		void addWindForcesForTriangle(Object *p1, Object *p2, Object *p3, const Vec3 direction);

		/* A private method used by drawShaded(), that draws a single triangle p1,p2,p3 with a color*/
		void drawTriangle(Object *p1, Object *p2, Object *p3, const Vec3 color);

	public:

		/* This is a important constructor for the entire system of objects and constraints*/
		Cloth(float width, float height, int num_objects_width, int num_objects_height,int mo);

		/* drawing the cloth as a smooth shaded (and colored according to column) OpenGL triangular mesh
		Called from the display() method
		The cloth is seen as consisting of triangles for four objects in the grid as follows:

		(x,y)   *--* (x+1,y)
		| /|
		|/ |
		(x,y+1) *--* (x+1,y+1)

		*/
		void drawShaded(GLuint tex1);

		/* this is an important methods where the time is progressed one time step for the entire cloth.
		This includes calling satisfyConstraint() for every constraint, and calling timeStep() for all objects
		*/
		void timeStep();

		/* used to add gravity (or any other arbitrary vector) to all objects*/
		void addForce(const Vec3 direction);

		/* used to add wind forces to all objects, is added for each triangle since the final force is proportional to the triangle area as seen from the wind direction*/
		void windForce(const Vec3 direction);

		/* used to detect and resolve the collision of the cloth with the ball.
		This is based on a very simples scheme where the position of each object is simply compared to the sphere and corrected.
		This also means that the sphere can "slip through" if the ball is small enough compared to the distance in the grid bewteen objects
		*/
		void ballCollision(const Vec3 center, const float radius);
		//void drawPlane(GLuint tex1);
		void cubeCollision(const Vec3 center, const float edge, int frac);
		//void doFrame();
		void selfCollision();
	};


}



#endif