#ifdef _MSC_VER
#pragma once
#endif

#ifndef _CONSTRAIN_
#define _CONSTRAIN_


#include <iostream>
#include <array>
#include <iomanip>
#include <sstream>
#include <algorithm> 
#include "../core/common.h"
#include "../math/vec3.h"
#include "../object/object.h"

namespace ClothSimulation
{
	class Constrain
	{
	private:
		float rest_distance; // the length between object p1 and p2 in rest configuration

	public:
		Object * p1, *p2; // the two objects that are connected through this Collision

		Constrain(Object *p1, Object *p2);

		/* This is one of the important methods, where a single constraint between two objects p1 and p2 is solved
		the method is called by Cloth.time_step() many times per frame*/
		void satisfyConstrain();
	};
}



#endif //!_CONSTRAIN_