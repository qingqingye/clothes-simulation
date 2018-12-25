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


#ifndef _OBJECT_
#define _OBJECT_


#include <iostream>
#include <array>


#include <iomanip>
#include <sstream>
#include <algorithm>
#include "../core/common.h"
#include "../math/vec3.h"

namespace ClothSimulation
{
	class Object
	{
	private:
		bool movable; // can the object move or not ? used to pin parts of the cloth
		int x;
		int y;
		float mass; // the mass of the object (is always 1 in this example)
		Vec3 pos; // the current position of the object in 3D space
		Vec3 old_pos; // the position of the object in the previous time step, used as part of the verlet numerical integration scheme
		Vec3 acceleration; // a vector representing the current acceleration of the object
		Vec3 accumulated_normal; // an accumulated normal (i.e. non normalized), used for OpenGL soft shading
		Vec3 velocity;
	public:
		Object(Vec3 pos,int x, int y);

		Object() {}

		void addForce(Vec3 f);

		Vec3 Velocity() { return velocity; }

		void changeVelocity(Vec3 v) { velocity = v; }

		/* This is one of the important methods, where the time is progressed a single step size (TIME_STEPSIZE)
		The method is called by Cloth.time_step()
		Given the equation "force = mass * acceleration" the next position is found through verlet integration*/
		void timeStep();
		
		int X() { return x; }
		int Y() { return y; }
		Vec3& getPos() { return pos; }

		Vec3& getOldpos() { return old_pos; }

		Vec3& Acceleration() { return acceleration; }

		void resetAcceleration() { acceleration = Vec3(0, 0, 0); }

		void offsetPos(const Vec3 v) { if (movable) pos += v; }

		void makeUnmovable() { movable = false; }

		void addToNormal(Vec3 normal);

		Vec3& getNormal() { return accumulated_normal; } // notice, the normal is not unit length

		void resetNormal() { accumulated_normal = Vec3(0, 0, 0); }

	};
}

#endif