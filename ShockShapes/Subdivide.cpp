/** @file Subdivide.cpp
 * 
 * @brief Geometry filter that subdivides polygons
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

#include "Subdivide.h"

//Constructor
Subdivide::Subdivide(int ilevels)
:GeometryFilter("Subdivide")
{
	levels = ilevels;
}

//Destructor
Subdivide::~Subdivide()
{

}

//Perform one iteration of subdivision
void Subdivide::subd(Geometry *g)
{
	std::vector<Midpoint> midpoints;
	std::vector<NormalInt> norm_ints;

	int mid1, mid2, mid3;
	int nint1, nint2, nint3;
	Triangle *current;
	Vector3D *v1;
	Vector3D *v2;
	Vector3D midpoint;
	Vector3D interpolated_normal;
	Vector2D uv1, uv2, uv3;
	Triangle temp;

	//Iterate through each triangle
	int num_triangles = g->getNumTriangles();
	for(int i = 0; i < num_triangles; i++) {
		mid1 = -1; mid2 = -1; mid3 = -1;
		nint1 = -1; nint2 = -1; nint3 = -1;
		current = g->getTriangle(i);

		//Check for existing midpoints
		int num_midpoints = midpoints.size();
		for(int j = 0; j < num_midpoints; j++) {
			if(current->vertices[0] == midpoints[j].v1) {
				if(current->vertices[1] == midpoints[j].v2)
					mid1 = midpoints[j].vmid;
				else if(current->vertices[2] == midpoints[j].v2)
					mid3 = midpoints[j].vmid;
			} else if(current->vertices[1] == midpoints[j].v1) {
				if(current->vertices[0] == midpoints[j].v2)
					mid1 = midpoints[j].vmid;
				else if(current->vertices[2] == midpoints[j].v2)
					mid2 = midpoints[j].vmid;
			} else if(current->vertices[2] == midpoints[j].v1) {
				if(current->vertices[0] == midpoints[j].v2)
					mid3 = midpoints[j].vmid;
				else if(current->vertices[1] == midpoints[j].v2)
					mid2 = midpoints[j].vmid;
			}
		}

		//Calculate remaining midpoints
		if(mid1 == -1) {
			Midpoint m;
			m.v1 = current->vertices[0];
			m.v2 = current->vertices[1];

			v1 = g->getVertex(m.v1);
			v2 = g->getVertex(m.v2);
			midpoint.x = (v1->x + v2->x) * 0.5f;
			midpoint.y = (v1->y + v2->y) * 0.5f;
			midpoint.z = (v1->z + v2->z) * 0.5f;

			mid1 = m.vmid = g->addVertex(midpoint);
			midpoints.push_back(m);
		}

		if(mid2 == -1) {
			Midpoint m;
			m.v1 = current->vertices[1];
			m.v2 = current->vertices[2];

			v1 = g->getVertex(m.v1);
			v2 = g->getVertex(m.v2);
			midpoint.x = (v1->x + v2->x) * 0.5f;
			midpoint.y = (v1->y + v2->y) * 0.5f;
			midpoint.z = (v1->z + v2->z) * 0.5f;

			mid2 = m.vmid = g->addVertex(midpoint);
			midpoints.push_back(m);
		}

		if(mid3 == -1) {
			Midpoint m;
			m.v1 = current->vertices[2];
			m.v2 = current->vertices[0];

			v1 = g->getVertex(m.v1);
			v2 = g->getVertex(m.v2);
			midpoint.x = (v1->x + v2->x) * 0.5f;
			midpoint.y = (v1->y + v2->y) * 0.5f;
			midpoint.z = (v1->z + v2->z) * 0.5f;

			mid3 = m.vmid = g->addVertex(midpoint);
			midpoints.push_back(m);
		}

		//Don't interpolate normals if they are the same
		if(current->normals[0] == current->normals[1])
			nint1 = current->normals[0];

		if(current->normals[1] == current->normals[2])
			nint2 = current->normals[1];

		if(current->normals[2] == current->normals[0])
			nint3 = current->normals[2];

		//Check for pre-calculated interpolations of normals
		if(nint1 == -1 || nint2 == -1 || nint3 == -1) {
			int num_norm_ints = norm_ints.size();
			for(int j = 0; j < num_norm_ints; j++) {
				if(current->normals[0] == norm_ints[j].n1) {
					if(current->normals[1] == norm_ints[j].n2)
						nint1 = norm_ints[j].nmid;
					else if(current->normals[2] == norm_ints[j].n2)
						nint3 = norm_ints[j].nmid;
				} else if(current->normals[1] == norm_ints[j].n1) {
					if(current->normals[0] == norm_ints[j].n2)
						nint1 = norm_ints[j].nmid;
					else if(current->normals[2] == norm_ints[j].n2)
						nint2 = norm_ints[j].nmid;
				} else if(current->normals[2] == norm_ints[j].n1) {
					if(current->normals[0] == norm_ints[j].n2)
						nint3 = norm_ints[j].nmid;
					else if(current->normals[1] == norm_ints[j].n2)
						nint2 = norm_ints[j].nmid;
				}
			}
		}

		//Calculate new normal interpolations
		if(nint1 == -1) {
			NormalInt ni;
			ni.n1 = current->normals[0];
			ni.n2 = current->normals[1];

			v1 = g->getNormal(ni.n1);
			v2 = g->getNormal(ni.n2);

			interpolated_normal.x = (v1->x + v2->x) * 0.5f;
			interpolated_normal.y = (v1->y + v2->y) * 0.5f;
			interpolated_normal.z = (v1->x + v2->z) * 0.5f;

			nint1 = ni.nmid = g->addNormal(interpolated_normal);
			norm_ints.push_back(ni);
		}

		if(nint2 == -1) {
			NormalInt ni;
			ni.n1 = current->normals[1];
			ni.n2 = current->normals[2];

			v1 = g->getNormal(ni.n1);
			v2 = g->getNormal(ni.n2);

			interpolated_normal.x = (v1->x + v2->x) * 0.5f;
			interpolated_normal.y = (v1->y + v2->y) * 0.5f;
			interpolated_normal.z = (v1->x + v2->z) * 0.5f;

			nint2 = ni.nmid = g->addNormal(interpolated_normal);
			norm_ints.push_back(ni);
		}

		if(nint3 == -1) {
			NormalInt ni;
			ni.n1 = current->normals[2];
			ni.n2 = current->normals[0];

			v1 = g->getNormal(ni.n1);
			v2 = g->getNormal(ni.n2);

			interpolated_normal.x = (v1->x + v2->x) * 0.5f;
			interpolated_normal.y = (v1->y + v2->y) * 0.5f;
			interpolated_normal.z = (v1->x + v2->z) * 0.5f;

			nint3 = ni.nmid = g->addNormal(interpolated_normal);
			norm_ints.push_back(ni);
		}

		//Interpolate uvs
		uv1.u = (current->uvs[0].u + current->uvs[1].u) * 0.5f;
		uv1.v = (current->uvs[0].v + current->uvs[1].v) * 0.5f;

		uv2.u = (current->uvs[1].u + current->uvs[2].u) * 0.5f;
		uv2.v = (current->uvs[1].v + current->uvs[2].v) * 0.5f;

		uv3.u = (current->uvs[2].u + current->uvs[0].u) * 0.5f;
		uv3.v = (current->uvs[2].v + current->uvs[0].v) * 0.5f;

		//Tesselate the triangle
		//Triangle 1
		temp.vertices[0] = current->vertices[0];
		temp.normals[0] = current->normals[0];
		temp.uvs[0] = current->uvs[0];

		temp.vertices[1] = mid1;
		temp.normals[1] = nint1;
		temp.uvs[1] = uv1;

		temp.vertices[2] = mid3;
		temp.normals[2] = nint3;
		temp.uvs[2] = uv3;

		g->addTriangle(temp);
		current = g->getTriangle(i);

		//Triangle 2
		temp.vertices[0] = mid1;
		temp.normals[0] = nint1;
		temp.uvs[0] = uv1;

		temp.vertices[1] = mid2;
		temp.normals[1] = nint2;
		temp.uvs[1] = uv2;

		temp.vertices[2] = mid3;
		temp.normals[2] = nint3;
		temp.uvs[2] = uv3;

		g->addTriangle(temp);
		current = g->getTriangle(i);

		//Triangle 3
		temp.vertices[0] = mid1;
		temp.normals[0] = nint1;
		temp.uvs[0] = uv1;

		temp.vertices[1] = current->vertices[1];
		temp.normals[1] = current->normals[1];
		temp.uvs[1] = current->uvs[1];

		temp.vertices[2] = mid2;
		temp.normals[2] = nint2;
		temp.uvs[2] = uv2;

		g->addTriangle(temp);
		current = g->getTriangle(i);

		//Triangle 4
		temp.vertices[0] = mid3;
		temp.normals[0] = nint3;
		temp.uvs[0] = uv3;

		temp.vertices[1] = mid2;
		temp.normals[1] = nint2;
		temp.uvs[1] = uv2;

		temp.vertices[2] = current->vertices[2];
		temp.normals[2] = current->normals[2];
		temp.uvs[2] = current->uvs[2];

		g->setTriangle(i, temp);
	}

	return;
}

//Run the filter on specified object
void Subdivide::run(Geometry *g)
{
	//Run the subdivision algorithm levels times
	for(int i = 0; i < levels; i++)
		subd(g);

	return;
}
