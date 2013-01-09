/** @file NormalFilter.cpp
 * 
 * @brief Recalculates normals for a mesh
 * 
 * Copyright 2013, Stewart Hall
 *
 * Licensed under The MIT License
 * Redistributions of files must retain the above copyright notice.
 *
 * @author		Stewart Hall (www.stewartghall.com)
 * @date		1/2/2013
 * @copyright	Copyright 2013, Stewart Hall
 * @license		MIT License (http://www.opensource.org/licenses/mit-license.php)
 */

#include <math.h>
#include <vector>

#include "NormalFilter.h"

//Constructor
NormalFilter::NormalFilter(normal_method imethod)
{
	method = imethod;
	soften_all = true;
	threshold = 2.0f;
}

//Destructor
NormalFilter::~NormalFilter()
{
	
}

//Sets up soften threshold
void NormalFilter::enableSoftenThreshold(float t)
{
	soften_all = false;
	threshold = t;
}

//Disables soften threshold
void NormalFilter::disableSoftenThreshold()
{
	soften_all = true;
}

//Change the generation method
void NormalFilter::changeMethod(normal_method m)
{
	method = m;
}

//Re-generate normals for a mesh
void NormalFilter::run(Geometry *g)
{
	Vector3D* v1;
	Vector3D* v2;
	Vector3D* v3;
	Triangle* current;
	Triangle current_mod;
	Vector3D edge1, edge2;
	Vector3D normal;

	int n1;
	int count;
	float magnitude;
	float dot;
	float inv_threshold = 1.0f - threshold;

	int num_triangles = g->getNumTriangles();
	if(method == NM_HARDEN) {
		//Calculate per face normals for each triangle
		for(int i = 0; i < num_triangles; i++) {
			//Get the triangle
			current = g->getTriangle(i);

			//Get vertices
			v1 = g->getVertex(current->vertices[0]);
			v2 = g->getVertex(current->vertices[1]);
			v3 = g->getVertex(current->vertices[2]);

			//Calculate edge vectors of the triangle
			edge1.x = v2->x - v1->x;
			edge1.y = v2->y - v1->y;
			edge1.z = v2->z - v1->z;

			edge2.x = v3->x - v1->x;
			edge2.y = v3->y - v1->y;
			edge2.z = v3->z - v1->z;

			//Calculate normal from cross product
			normal.x = edge1.y * edge2.z - edge1.z * edge2.y;
			normal.y = edge1.z * edge2.x - edge1.x * edge2.z;
			normal.z = edge1.x * edge2.y - edge1.y * edge2.x;

			//Normalize the resulting normal
			magnitude = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
			magnitude = 1.0f / magnitude;
			normal.x *= magnitude;
			normal.y *= magnitude;
			normal.z *= magnitude;

			//Add normal to geometry buffer
			n1 = g->addNormal(normal);

			//Modify triangle and replace in mesh
			current_mod.vertices[0] = current->vertices[0];
			current_mod.vertices[1] = current->vertices[1];
			current_mod.vertices[2] = current->vertices[2];

			current_mod.uvs[0] = current->uvs[0];
			current_mod.uvs[1] = current->uvs[1];
			current_mod.uvs[2] = current->uvs[2];

			current_mod.normals[0] = n1;
			current_mod.normals[1] = n1;
			current_mod.normals[2] = n1;

			g->setTriangle(i, current_mod);
		}
	} else {
		//Calculate all per-face normals
		Vector3D *face_normals = new Vector3D[g->getNumTriangles()];
		for(int i = 0; i < num_triangles; i++) {
			//Get the triangle
			current = g->getTriangle(i);

			//Get vertices
			v1 = g->getVertex(current->vertices[0]);
			v2 = g->getVertex(current->vertices[1]);
			v3 = g->getVertex(current->vertices[2]);

			//Calculate edge vectors of the triangle
			edge1.x = v2->x - v1->x;
			edge1.y = v2->y - v1->y;
			edge1.z = v2->z - v1->z;

			edge2.x = v3->x - v1->x;
			edge2.y = v3->y - v1->y;
			edge2.z = v3->z - v1->z;

			//Calculate normal from cross product
			normal.x = edge1.y * edge2.z - edge1.z * edge2.y;
			normal.y = edge1.z * edge2.x - edge1.x * edge2.z;
			normal.z = edge1.x * edge2.y - edge1.y * edge2.x;

			//Normalize the resulting normal
			magnitude = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
			magnitude = 1.0f / magnitude;
			normal.x *= magnitude;
			normal.y *= magnitude;
			normal.z *= magnitude;

			//Add normal to geometry buffer
			face_normals[i] = normal;
		}

		//Iterate through each vertex and compute its normal(s)
		std::vector<int> neighbors;
		float inverse_divisor;
		int num_neighbors;
		int num_vertices = g->getNumVertices();
		if(soften_all) {
			//Iterate through each vertex and calculate its normal
			for(int i = 0; i < num_vertices; i++) {
				//Find all triangles that use this vertex
				for(int j = 0; j < num_triangles; j++) {
					current = g->getTriangle(j);

					if(current->vertices[0] == i || current->vertices[1] == i || current->vertices[2] == i)
						neighbors.push_back(j);
				}
				
				//Average normals of all triangles with this vertex
				normal.x = normal.y = normal.z = 0.0f;
				num_neighbors = neighbors.size();
				for(int j = 0; j < num_neighbors; j++) {
					normal.x += face_normals[neighbors[j]].x;
					normal.y += face_normals[neighbors[j]].y;
					normal.z += face_normals[neighbors[j]].z;
				}

				inverse_divisor = 1.0f / (float)num_neighbors;
				normal.x *= inverse_divisor;
				normal.y *= inverse_divisor;
				normal.z *= inverse_divisor;

				//Add this new normal to the geometry's buffer
				n1 = g->addNormal(normal);

				//Modify each triangle
				for(int j = 0; j < num_neighbors; j++) {
					current = g->getTriangle(neighbors[j]);

					current_mod.vertices[0] = current->vertices[0];
					current_mod.vertices[1] = current->vertices[1];
					current_mod.vertices[2] = current->vertices[2];

					current_mod.uvs[0] = current->uvs[0];
					current_mod.uvs[1] = current->uvs[1];
					current_mod.uvs[2] = current->uvs[2];

					if(current->vertices[0] == i) {
						current_mod.normals[0] = n1;
						current_mod.normals[1] = current->normals[1];
						current_mod.normals[2] = current->normals[2];
					} else if(current->vertices[1] == i) {
						current_mod.normals[0] = current->normals[0];
						current_mod.normals[1] = n1;
						current_mod.normals[2] = current->normals[2];
					} else {
						current_mod.normals[0] = current->normals[0];
						current_mod.normals[1] = current->normals[1];
						current_mod.normals[2] = n1;
					}

					g->setTriangle(neighbors[j], current_mod);
				}

				//Clear the neighbors list
				neighbors.clear();
			}
		} else {
			std::vector<int> n_normals;
			//Iterate through each vertex and calculate its normal
			for(int i = 0; i < num_vertices; i++) {
				//Find all triangles that use this vertex
				for(int j = 0; j < num_triangles; j++) {
					current = g->getTriangle(j);

					if(current->vertices[0] == i || current->vertices[1] == i || current->vertices[2] == i) {
						neighbors.push_back(j);
						n_normals.push_back(-1);
					}
				}

				num_neighbors = neighbors.size();

				//Modify each triangle
				for(int j = 0; j < num_neighbors; j++) {
					current = g->getTriangle(neighbors[j]);

					//Combine normals of all neighbors facing in a similar direction
					normal.x = normal.y = normal.z = 0.0f;
					count = 0;
					n1 = -1;
					for(int k = 0; k < num_neighbors; k++) {
						//Make sure this triangle is facing in a similar direction
						dot = face_normals[neighbors[k]].x * face_normals[neighbors[j]].x +
							face_normals[neighbors[k]].y * face_normals[neighbors[j]].y +
							face_normals[neighbors[k]].z * face_normals[neighbors[j]].z;

						if(dot < inv_threshold)
							continue;

						//Check for already generated normal
						if(n_normals[k] != -1) {
							n1 = n_normals[k];
							break;
						}
						
						normal.x += face_normals[neighbors[k]].x;
						normal.y += face_normals[neighbors[k]].y;
						normal.z += face_normals[neighbors[k]].z;

						count++;
					}

					//Average the normal and add to geometry
					if(n1 == -1) {
						inverse_divisor = 1.0f / (float)count;
						normal.x *= inverse_divisor;
						normal.y *= inverse_divisor;
						normal.z *= inverse_divisor;

						n1 = g->addNormal(normal);
						n_normals[j] = n1;
					}

					//Modify the triangle
					current_mod.vertices[0] = current->vertices[0];
					current_mod.vertices[1] = current->vertices[1];
					current_mod.vertices[2] = current->vertices[2];

					current_mod.uvs[0] = current->uvs[0];
					current_mod.uvs[1] = current->uvs[1];
					current_mod.uvs[2] = current->uvs[2];

					if(current->vertices[0] == i) {
						current_mod.normals[0] = n1;
						current_mod.normals[1] = current->normals[1];
						current_mod.normals[2] = current->normals[2];
					} else if(current->vertices[1] == i) {
						current_mod.normals[0] = current->normals[0];
						current_mod.normals[1] = n1;
						current_mod.normals[2] = current->normals[2];
					} else {
						current_mod.normals[0] = current->normals[0];
						current_mod.normals[1] = current->normals[1];
						current_mod.normals[2] = n1;
					}

					g->setTriangle(neighbors[j], current_mod);
				}

				//Clear the neighbors list
				neighbors.clear();
				n_normals.clear();
			}
		}

		delete[] face_normals;
	}

	return;
}
