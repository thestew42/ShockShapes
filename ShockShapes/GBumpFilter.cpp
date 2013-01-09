/** @file GBumpFilter.cpp
 * 
 * @brief Geometry filter makes the surface of an object rough
 * 
 * Copyright 2013, Stewart Hall
 *
 * Licensed under The MIT License
 * Redistributions of files must retain the above copyright notice.
 *
 * @author		Stewart Hall (www.stewartghall.com)
 * @date		1/1/2013
 * @copyright	Copyright 2013, Stewart Hall
 * @license		MIT License (http://www.opensource.org/licenses/mit-license.php)
 */

#include "GBumpFilter.h"

//Constructor
GBumpFilter::GBumpFilter(Parameter imagnitude, bool iperturb_once)
:direction()
{
	magnitude = imagnitude;
	perturb_once = iperturb_once;

	direction_constrain = false;
	tolerance = 0.0f;
}

//Destructor
GBumpFilter::~GBumpFilter()
{

}

//Set up a direction constraint
void GBumpFilter::setupDirectionConstraint(Vector3D idirection, float itolerance)
{
	direction = idirection;
	tolerance = itolerance;
	direction_constrain = true;
}

//Disable direction constraint
void GBumpFilter::disableDirectionConstraint()
{
	direction_constrain = false;
}

//Run the filter
void GBumpFilter::run(Geometry *g)
{
	Triangle *current;
	Vector3D *vertex;
	Vector3D *normal;
	float itol = 1.0f - tolerance;
	float agreement, scalar;
	if(perturb_once) {
		//Set up list to keep track of which vertices have been perturbed
		int num_vertices = g->getNumVertices();
		int *visited = new int[num_vertices];
		for(int i = 0; i < num_vertices; i++)
			visited[i] = 0;

		//Iterate through each triangle and perturb vertices
		int num_triangles = g->getNumTriangles();
		if(direction_constrain) {
			for(int i = 0; i < num_triangles; i++) {
				current = g->getTriangle(i);
				for(int j = 0; j < 3; j++) {
					//Skip perturbed vertices
					if(visited[current->vertices[j]])
						continue;

					//Check direction factor
					normal = g->getNormal(current->normals[j]);
					agreement = direction.x * normal->x + direction.y * normal->y + direction.z * normal->z;
					if(agreement < itol)
						continue;

					//Perturb the vertex
					vertex = g->getVertex(current->vertices[j]);
					scalar = magnitude.sample();
					vertex->x += (normal->x * scalar);
					vertex->y += (normal->y * scalar);
					vertex->z += (normal->z * scalar);

					//Mark as visited
					visited[current->vertices[j]] = 1;
				}
			}
		} else {
			for(int i = 0; i < num_triangles; i++) {
				current = g->getTriangle(i);
				for(int j = 0; j < 3; j++) {
					//Skip perturbed vertices
					if(visited[current->vertices[j]])
						continue;

					//Perturb the vertex
					vertex = g->getVertex(current->vertices[j]);
					normal = g->getNormal(current->normals[j]);
					scalar = magnitude.sample();
					vertex->x += (normal->x * scalar);
					vertex->y += (normal->y * scalar);
					vertex->z += (normal->z * scalar);

					//Mark as visited
					visited[current->vertices[j]] = 1;
				}
			}
		}

		delete[] visited;
	} else {
		//Iterate through each triangle and perturb vertices
		int num_triangles = g->getNumTriangles();
		if(direction_constrain) {
			for(int i = 0; i < num_triangles; i++) {
				current = g->getTriangle(i);
				for(int j = 0; j < 3; j++) {
					normal = g->getNormal(current->normals[j]);
					agreement = direction.x * normal->x + direction.y * normal->y + direction.z * normal->z;
					if(agreement < itol)
						continue;

					//Perturb the vertex
					vertex = g->getVertex(current->vertices[j]);
					scalar = magnitude.sample();
					vertex->x += (normal->x * scalar);
					vertex->y += (normal->y * scalar);
					vertex->z += (normal->z * scalar);
				}
			}
		} else {
			for(int i = 0; i < num_triangles; i++) {
				current = g->getTriangle(i);
				for(int j = 0; j < 3; j++) {
					//Perturb the vertex
					vertex = g->getVertex(current->vertices[j]);
					normal = g->getNormal(current->normals[j]);
					scalar = magnitude.sample();
					vertex->x += (normal->x * scalar);
					vertex->y += (normal->y * scalar);
					vertex->z += (normal->z * scalar);
				}
			}
		}
	}

	return;
}
