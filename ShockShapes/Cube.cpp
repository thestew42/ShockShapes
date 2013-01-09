/** @file Cube.cpp
 * 
 * @brief Basic geometric object generator for rectangular prisms
 * 
 * Copyright 2012-2013, Stewart Hall
 *
 * Licensed under The MIT License
 * Redistributions of files must retain the above copyright notice.
 *
 * @author		Stewart Hall (www.stewartghall.com)
 * @date		12/31/2012
 * @copyright	Copyright 2012, Stewart Hall
 * @license		MIT License (http://www.opensource.org/licenses/mit-license.php)
 */

#include "Cube.h"

//Default constructor
Cube::Cube()
:Geometry("Cube")
{
	l = w = h = 1.0f;
}

//Cube constructor
Cube::Cube(float dimension)
:Geometry("Cube")
{
	l = w = h = dimension;
}

//Rectangular prism constructor
Cube::Cube(float x, float y, float z)
:Geometry("Cube")
{
	l = x;
	w = z;
	h = y;
}

//Generates the object's mesh
void Cube::generate(int seed, Scene *scene)
{
	Vector3D v;
	Triangle t1, t2;
	float hl = l * 0.5f;
	float hh = h * 0.5f;
	float hw = w * 0.5f;

	//Create vertex for each corner
	v.x = hl; v.y = hh; v.z = hw;
	addVertex(v);

	v.x = -hl;
	addVertex(v);

	v.z = -hw;
	addVertex(v);

	v.x = hl;
	addVertex(v);

	v.z = hw; v.y = -hh;
	addVertex(v);

	v.x = -hl;
	addVertex(v);

	v.z = -hw;
	addVertex(v);

	v.x = hl;
	addVertex(v);

	//Create a two triangles for each face
	//Top
	v.x = 0.0f; v.y = 1.0f; v.z = 0.0f;
	addNormal(v);
	t1.normals[0] = t1.normals[1] = t1.normals[2] = 0;
	t2.normals[0] = t2.normals[1] = t2.normals[2] = 0;

	t1.uvs[0].u = t1.uvs[0].v = t1.uvs[1].v = 1.0f;
	t1.uvs[1].u = t1.uvs[2].u = t1.uvs[2].v = 0.0f;
	t2.uvs[0].u = t2.uvs[0].v = t2.uvs[1].v = 0.0f;
	t2.uvs[1].u = t2.uvs[2].u = t2.uvs[2].v = 1.0f;

	t1.vertices[0] = 2; t1.vertices[1] = 1; t1.vertices[2] = 0;
	addTriangle(t1);

	t2.vertices[0] = 0; t2.vertices[1] = 3; t2.vertices[2] = 2;
	addTriangle(t2);

	//Front
	v.x = 0.0f; v.y = 0.0f; v.z = 1.0f;
	addNormal(v);
	t1.normals[0] = t1.normals[1] = t1.normals[2] = 1;
	t2.normals[0] = t2.normals[1] = t2.normals[2] = 1;

	t1.vertices[0] = 1; t1.vertices[1] = 5; t1.vertices[2] = 4;
	addTriangle(t1);

	t2.vertices[0] = 4; t2.vertices[1] = 0; t2.vertices[2] = 1;
	addTriangle(t2);

	//Bottom
	v.x = 0.0f; v.y = -1.0f; v.z = 0.0f;
	addNormal(v);
	t1.normals[0] = t1.normals[1] = t1.normals[2] = 2;
	t2.normals[0] = t2.normals[1] = t2.normals[2] = 2;

	t1.vertices[0] = 5; t1.vertices[1] = 6; t1.vertices[2] = 7;
	addTriangle(t1);

	t2.vertices[0] = 7; t2.vertices[1] = 4; t2.vertices[2] = 5;
	addTriangle(t2);

	//Back
	v.x = 0.0f; v.y = 0.0f; v.z = -1.0f;
	addNormal(v);
	t1.normals[0] = t1.normals[1] = t1.normals[2] = 3;
	t2.normals[0] = t2.normals[1] = t2.normals[2] = 3;

	t1.vertices[0] = 3; t1.vertices[1] = 7; t1.vertices[2] = 6;
	addTriangle(t1);

	t2.vertices[0] = 6; t2.vertices[1] = 2; t2.vertices[2] = 3;
	addTriangle(t2);

	//Left
	v.x = -1.0f; v.y = 0.0f; v.z = 0.0f;
	addNormal(v);
	t1.normals[0] = t1.normals[1] = t1.normals[2] = 4;
	t2.normals[0] = t2.normals[1] = t2.normals[2] = 4;

	t1.vertices[0] = 2; t1.vertices[1] = 6; t1.vertices[2] = 5;
	addTriangle(t1);

	t2.vertices[0] = 5; t2.vertices[1] = 1; t2.vertices[2] = 2;
	addTriangle(t2);

	//Right
	v.x = 1.0f; v.y = 0.0f; v.z = 0.0f;
	addNormal(v);
	t1.normals[0] = t1.normals[1] = t1.normals[2] = 5;
	t2.normals[0] = t2.normals[1] = t2.normals[2] = 5;

	t1.vertices[0] = 0; t1.vertices[1] = 4; t1.vertices[2] = 7;
	addTriangle(t1);

	t2.vertices[0] = 7; t2.vertices[1] = 3; t2.vertices[2] = 0;
	addTriangle(t2);

	return;
}
