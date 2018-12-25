#include "object.h"
#include <stdlib.h>
#include <random>
#include <iostream>

namespace ClothSimulation
{
	Object::Object(Vec3 pos, int x, int y) :
		pos(pos),
		old_pos(pos),
		acceleration(Vec3(0, 0, 0)),
		mass(1),
		movable(true),
		accumulated_normal(Vec3(0, 0, 0)),
		x(x),
		y(y),
		velocity(pos -pos)
	{}

	
	void Object::addForce(Vec3 f)
	{
		acceleration += f / mass;
		//printf("moveable %d", movable1);
	}

	void Object::addToNormal(Vec3 normal)
	{
		accumulated_normal += normal.normalized();
	}
	
	void Object::timeStep()
	{
		
		if (movable)
		{
			//printf("%d", 1);
			Vec3 temp = pos;
			//printf("%f\n",pos)
			if ((pos.y > -1) || (pos - old_pos).norm() > 0.000001) {
				Vec3 t((pos - old_pos).x, (pos - old_pos).y, (pos - old_pos).z);
				/*printf("start!\n");
				printf(" %f %f %f \n",t.x,t.y,t.z);
				printf(" %f %f %f \n", velocity.x, velocity.y, velocity.z);*/
				pos = pos + velocity*(1.0 - DAMPING) + acceleration * TIME_STEPSIZE2;
			}
				//pos = pos + (pos - old_pos)*(1.0 - DAMPING) + acceleration * TIME_STEPSIZE2;
			old_pos = temp;
			velocity = pos - old_pos;
			acceleration = Vec3(0, 0, 0); // acceleration is reset since it HAS been translated into a change in position (and implicitely into velocity)	
		}
	}
}