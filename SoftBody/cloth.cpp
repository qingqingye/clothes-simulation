#include "cloth.h"
#include <stdlib.h>
#include <random>
#include <iostream>
int time_counter = 0;

namespace ClothSimulation
{
	Object* Cloth::getObject(int x, int y)
	{
		return &objects[y*num_objects_width + x];
	}


	void Cloth::makeConstrain(Object *p1, Object *p2)
	{
		Constrains.push_back(Constrain(p1, p2));
	}


	Vec3 Cloth::calcTriangleNormal(Object *p1, Object *p2, Object *p3)
	{
		Vec3 pos1 = p1->getPos();
		Vec3 pos2 = p2->getPos();
		Vec3 pos3 = p3->getPos();

		Vec3 v1 = pos2 - pos1;
		Vec3 v2 = pos3 - pos1;

		return v1.cross(v2);
	}


	void Cloth::addWindForcesForTriangle(Object *p1, Object *p2, Object *p3, const Vec3 direction)
	{
		Vec3 normal = calcTriangleNormal(p1, p2, p3);
		Vec3 d = normal.normalized();
		Vec3 force = normal * (d.dot(direction));
		p1->addForce(force);
		p2->addForce(force);
		p3->addForce(force);
	}

	void Cloth::drawTriangle(Object *p1, Object *p2, Object *p3, const Vec3 color)
	{
		/*glColor3fv((GLfloat*)&color);

		glNormal3fv((GLfloat *) &(p1->getNormal().normalized()));
		glVertex3fv((GLfloat *) &(p1->getPos()));

		glNormal3fv((GLfloat *) &(p2->getNormal().normalized()));
		glVertex3fv((GLfloat *) &(p2->getPos()));

		glNormal3fv((GLfloat *) &(p3->getNormal().normalized()));
		glVertex3fv((GLfloat *) &(p3->getPos()));*/
		glTexCoord2f((p1->X()*1.0f) / num_objects_width, (p1->Y()*1.0f) / num_objects_height);
		glNormal3fv((GLfloat *) &(p1->getNormal().normalized()));
		glVertex3fv((GLfloat *) &(p1->getPos()));
		glTexCoord2f((p2->X()*1.0f) / num_objects_width, (p2->Y()*1.0f) / num_objects_height);
		glNormal3fv((GLfloat *) &(p2->getNormal().normalized()));
		glVertex3fv((GLfloat *) &(p2->getPos()));
		glTexCoord2f((p3->X()*1.0f) / num_objects_width, (p3->Y()*1.0f) / num_objects_height);
		glNormal3fv((GLfloat *) &(p3->getNormal().normalized()));
		glVertex3fv((GLfloat *) &(p3->getPos()));

	}


	Cloth::Cloth(float width, float height, int num_objects_width, int num_objects_height,int mo) :
		num_objects_width(num_objects_width),
		num_objects_height(num_objects_height)
	{
		objects.resize(num_objects_width*num_objects_height); //I am essentially using this vector as an array with room for num_objects_width*num_objects_height objects

															  // creating objects in a grid of objects from (0,0,0) to (width,-height,0)
		for (int x = 0; x<num_objects_width; x++)
		{
			for (int y = 0; y<num_objects_height; y++)
			{
				Vec3 pos = Vec3(width * (x / (float)num_objects_width), 0,
					height * (y / (float)num_objects_height));
				objects[y*num_objects_width + x] = Object(pos,x,y); // insert object in column x at y'th row
			}
		}

		// Connecting immediate neighbor objects with constraints (distance 1 and sqrt(2) in the grid)
		for (int x = 0; x<num_objects_width; x++)
		{
			for (int y = 0; y<num_objects_height; y++)
			{
				if (x<num_objects_width - 1) makeConstrain(getObject(x, y), getObject(x + 1, y));
				if (y<num_objects_height - 1) makeConstrain(getObject(x, y), getObject(x, y + 1));
				if (x<num_objects_width - 1 && y<num_objects_height - 1) makeConstrain(getObject(x, y), getObject(x + 1, y + 1));
				if (x<num_objects_width - 1 && y<num_objects_height - 1) makeConstrain(getObject(x + 1, y), getObject(x, y + 1));
			}
		}


		// Connecting secondary neighbors with constraints (distance 2 and sqrt(4) in the grid)
		for (int x = 0; x<num_objects_width; x++)
		{
			for (int y = 0; y<num_objects_height; y++)
			{
				if (x<num_objects_width - 2) makeConstrain(getObject(x, y), getObject(x + 2, y));
				if (y<num_objects_height - 2) makeConstrain(getObject(x, y), getObject(x, y + 2));
				if (x<num_objects_width - 2 && y<num_objects_height - 2) makeConstrain(getObject(x, y), getObject(x + 2, y + 2));
				if (x<num_objects_width - 2 && y<num_objects_height - 2) makeConstrain(getObject(x + 2, y), getObject(x, y + 2));
			}
		}

		// making the first line unmoveable;
		if (mo == 1) {
			for (int i = 0; i<26; i++)
			{
				//getObject(0 + i, 0)->offsetPos(Vec3(0.5, 0.0, 0.0)); // moving the object a bit towards the center, to make it hang more natural - because I like it ;)
				getObject(0 + i, 0)->makeUnmovable();

				//getObject(0 + i, 0)->offsetPos(Vec3(-0.5, 0.0, 0.0)); // moving the object a bit towards the center, to make it hang more natural - because I like it ;)
				getObject(num_objects_width - 1 - i, 0)->makeUnmovable();
			}
		}
		
	}


	void Cloth::drawShaded(GLuint tex1)
	{
		// reset normals (which where written to last frame)
		// Solve the shading problem.
		std::vector<Object>::iterator object;
		for (object = objects.begin(); object != objects.end(); object++)
		{
			(*object).resetNormal();
		}

		//create smooth per object normals by adding up all the (hard) triangle normals that each object is part of
		for (int x = 0; x<num_objects_width - 1; x++)
		{
			for (int y = 0; y<num_objects_height - 1; y++)
			{
				Vec3 normal = calcTriangleNormal(getObject(x + 1, y), getObject(x, y), getObject(x, y + 1));
				getObject(x + 1, y)->addToNormal(normal);
				getObject(x, y)->addToNormal(normal);
				getObject(x, y + 1)->addToNormal(normal);

				normal = calcTriangleNormal(getObject(x + 1, y + 1), getObject(x + 1, y), getObject(x, y + 1));
				getObject(x + 1, y + 1)->addToNormal(normal);
				getObject(x + 1, y)->addToNormal(normal);
				getObject(x, y + 1)->addToNormal(normal);
			}
		}
		glBindTexture(GL_TEXTURE_2D, tex1);
		glBegin(GL_TRIANGLES);
		for (int x = 0; x<num_objects_width - 1; x++)
		{
			for (int y = 0; y<num_objects_height - 1; y++)
			{
				Vec3 color(0.2f*(x*1.0f/num_objects_width), 0.4f*(x*1.0f / num_objects_width), 0.8f*(x*1.0f / num_objects_width));
				drawTriangle(getObject(x + 1, y), getObject(x, y), getObject(x, y + 1), color);
				drawTriangle(getObject(x + 1, y + 1), getObject(x + 1, y), getObject(x, y + 1), color);
			}
		}
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
		//drawPlane(tex1);
	}


	void Cloth::timeStep()
	{
		std::vector<Constrain>::iterator Collision;
		for (int i = 0; i<COLLISION_ITERATIONS; i++) // iterate over all Constrains several times
		{
			for (Collision = Constrains.begin(); Collision != Constrains.end(); Collision++)
			{
				(*Collision).satisfyConstrain(); // satisfy constraint.
			}
		}

		std::vector<Object>::iterator object;
		for (object = objects.begin(); object != objects.end(); object++)
		{
			(*object).timeStep(); // calculate the position of each object at the next time step.
		}
	}


	void Cloth::addForce(const Vec3 direction)
	{
		std::vector<Object>::iterator object;
		float height = 0;
		for (object = objects.begin(); object != objects.end(); object++){
			Vec3 pos = (*object).getPos();
			if (pos.y +5  > height) {
				height = pos.y +5;
			}
		}
		//printf("%f\n", height);
		bool movable = true;
		if (height <= 0.001f) {
			time_counter += 1;
			//if (time_counter >= 1000)
				//movable = false;
			
		}
		for (object = objects.begin(); object != objects.end(); object++)
		{
			(*object).addForce(direction); // add the forces to each object
		}

	}


	void Cloth::windForce(const Vec3 direction)
	{
		for (int x = 0; x<num_objects_width - 1; x++)
		{
			for (int y = 0; y<num_objects_height - 1; y++)
			{
				addWindForcesForTriangle(getObject(x + 1, y), getObject(x, y), getObject(x, y + 1), direction);
				addWindForcesForTriangle(getObject(x + 1, y + 1), getObject(x + 1, y), getObject(x, y + 1), direction);
			}
		}
	}
	

	void Cloth::ballCollision(const Vec3 center, const float radius)
	{
		std::vector<Object>::iterator object;
		for (object = objects.begin(); object != objects.end(); object++)
		{
			Vec3 v = (*object).getPos() - center;
			float l = v.norm();
			if (v.norm() < radius) // if the object is inside the ball
			{
				(*object).offsetPos(v.normalized()*(radius - l)); // project the object to the surface of the ball
				(*object).changeVelocity((*object).getPos() - (*object).getOldpos());
			}
		}
	}

	void Cloth::cubeCollision(const Vec3 center, const float edge, int frac)
	{
		std::vector<Object>::iterator object;
		for (object = objects.begin(); object != objects.end(); object++)
		{
			Vec3 v = (*object).getPos() - center;

			//printf("here\n");
			//printf("%f %f %f\n", (*object).getPos().x, (*object).getPos().y, (*object).getPos().z);
			//printf("%f %f %f\n", v.x, v.y, v.z);
			//printf("%f %f %f\n", center.x, center.y, center.z);
			if (abs(v.x) < (edge / 2) && abs(v.y) < (edge / 2) && abs(v.z) < (edge / 2)) // if the object is inside the c
			{
				(*object).offsetPos(v / v.Max()*(edge / 2) - v); // project the object to the surface of the ball
				(*object).changeVelocity((*object).getPos() - (*object).getOldpos());
			}
			if (frac == 1) {
				if ((((*object).getPos()).y == (center.y + (edge / 2))) &&
					(abs(((*object).getPos()).x - center.x) <= (edge / 2)) &&
					(abs(((*object).getPos()).z - center.z) <= (edge / 2)))
				{
					//printf("eeeeee\n");
					float fraction = 100;
					if (((*object).getPos() - (*object).getOldpos()).norm() > 0)
					{
						/*Vec3 t = (*object).getPos();
						printf("here\n");
						printf("before: %f %f %f \n", t.x, t.y, t.z);*/
						Vec3 temp = ((*object).getPos() - (*object).getOldpos());
						if (temp.x <= fraction) {
							temp.x = -temp.x;
						}
						else if (temp.x > fraction) {
							temp.x = -fraction * (temp.x / abs(temp.x));
						}
						if (temp.y <= fraction) {
							temp.y = -temp.y;
						}
						else if (temp.y > fraction) {
							temp.y = -fraction * (temp.y / abs(temp.y));
						}
						if (temp.z <= fraction) {
							temp.z = -temp.z;
						}
						else if (temp.z > 0.1) {
							temp.z = -fraction * (temp.z / abs(temp.z));
						}
						(*object).offsetPos(temp);
						(*object).changeVelocity((*object).getPos() - (*object).getOldpos());
						/*t = (*object).getPos();
						printf("after: %f %f %f \n", t.x, t.y, t.z);*/
					}

				}
			}

		}
	}
	
	void Cloth::selfCollision()
	{
		std::vector<Object>::iterator object;
		std::vector<Object>::iterator object1;
		for (object = objects.begin(); object != objects.end(); object++)
		{
			for (object1 = objects.begin(); object1 != objects.end(); object1++)
			{
				if (object == object1) {
					break;
				}
				/*if ((((*object).getPos() - (*object1).getPos()).dot((*object).getOldpos() - (*object1).getPos())) < 0) {
					(*object).offsetPos(((*object).getOldpos() - (*object).getPos())*1.2);
				}*/
				float distance = ((*object).getPos() - (*object1).getPos()).norm();
				if (distance < 0.12) {
					Vec3 displacement = (*object).getPos() - (*object1).getPos();
					displacement.normalized();
					(*object).offsetPos(displacement*(0.12 - distance) / 1.0f);
					(*object).changeVelocity((*object).getPos() - (*object).getOldpos());
					(*object1).offsetPos(-displacement * (0.12 - distance) / 1.0f);
					(*object1).changeVelocity((*object1).getPos() - (*object1).getOldpos());
				}
			}
		}
		/*std::vector<Object>::iterator object;
		for (object = objects.begin(); object != objects.end(); object++) {
			for (int x = 0; x<num_objects_width - 1; x++)
			{
				for (int y = 0; y<num_objects_height - 1; y++)
				{
					Vec3 normal = calcTriangleNormal(getObject(x + 1, y), getObject(x, y), getObject(x, y + 1));


					normal = calcTriangleNormal(getObject(x + 1, y + 1), getObject(x + 1, y), getObject(x, y + 1));

				}
			}
		}*/
		
	}
}
