#include "constrain.h"
#include <stdlib.h>
#include <random>
#include <iostream>

namespace ClothSimulation
{

	Constrain::Constrain(Object *O1, Object *O2) : p1(O1), p2(O2)
	{
		Vec3 vec = p1->getPos() - p2->getPos();
		rest_distance = vec.norm();
	}

	void Constrain::satisfyConstrain() 
	{
		Vec3 p1_to_p2 = p2->getPos() - p1->getPos(); // vector from p1 to p2
		float current_distance = p1_to_p2.norm(); // current distance between p1 and p2
		if (abs(current_distance - rest_distance) > 0.0) {
			Vec3 correctionVector = p1_to_p2 * (1 - rest_distance / current_distance); // The offset vector that could moves p1 into a distance of rest_distance to p2
			Vec3 correctionVectorHalf = correctionVector * 0.5; // Lets make it half that length, so that we can move BOTH p1 and p2.
			p1->offsetPos(correctionVectorHalf); // correctionVectorHalf is pointing from p1 to p2, so the length should move p1 half the length needed to satisfy the constraint.
			p2->offsetPos(-correctionVectorHalf); // we must move p2 the negative direction of correctionVectorHalf since it points from p2 to p1, and not p1 to p2.	
			(*p1).changeVelocity((*p1).getPos() - (*p1).getOldpos());
			(*p2).changeVelocity((*p2).getPos() - (*p2).getOldpos());
			p1->changeVelocity((p1->Velocity() + p2->Velocity()) / 2);
			p2->changeVelocity((p1->Velocity() + p2->Velocity()) / 2);
		}
		
	}
}