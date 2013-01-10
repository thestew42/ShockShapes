/** @file Geometry.cpp
 * 
 * @brief Basic functionality for geometric objects
 * 
 * Copyright 2012-2013, Stewart Hall
 *
 * Licensed under The MIT License
 * Redistributions of files must retain the above copyright notice.
 *
 * @author		Stewart Hall (www.stewartghall.com)
 * @date		12/30/2012
 * @copyright	Copyright 2012-2013, Stewart Hall
 * @license		MIT License (http://www.opensource.org/licenses/mit-license.php)
 */

#include "Geometry.h"

int current_id = 0; /**< Incrementing number used for unique IDs. */

//Geometry constructor creates empty mesh
Geometry::Geometry()
:vertices(), vbuffer_references(), normals(), nbuffer_references(), triangles(), name("Geometry"), unique_id(""), t(), filters()
{
	id = current_id++;

	//Build the unique id for this object
	std::ostringstream unique_id_stream;
	unique_id_stream << name << id;
	unique_id = unique_id_stream.str();
}

//Geometry constructor with different name
Geometry::Geometry(const char *iname)
:vertices(), vbuffer_references(), normals(), nbuffer_references(), triangles(), name(iname), unique_id(""), t(), filters()
{
	id = current_id++;

	//Build the unique id for this object
	std::ostringstream unique_id_stream;
	unique_id_stream << name << id;
	unique_id = unique_id_stream.str();
}

//Destructor
Geometry::~Geometry()
{
	
}

//Get the id
const char *Geometry::getUniqueId()
{
	return unique_id.c_str();
}

//Add a vertex to the buffer
int Geometry::addVertex(Vector3D v)
{
	int index = vertices.size();
	vertices.push_back(v);
	vbuffer_references.push_back(0);

	return index;
}

//Adds a normal to the buffer
int Geometry::addNormal(Vector3D n)
{
	int index = normals.size();
	normals.push_back(n);
	nbuffer_references.push_back(0);

	return index;
}

//Add a triangle to the buffer
int Geometry::addTriangle(Triangle t)
{
	int index = triangles.size();
	triangles.push_back(t);

	//Update the vertex reference counts
	vbuffer_references[t.vertices[0]]++;
	vbuffer_references[t.vertices[1]]++;
	vbuffer_references[t.vertices[2]]++;

	//Update the normal reference counts
	nbuffer_references[t.normals[0]]++;
	nbuffer_references[t.normals[1]]++;
	nbuffer_references[t.normals[2]]++;

	return index;
}

//Sets a triangle
void Geometry::setTriangle(int id, Triangle t)
{
	//Update the vertex reference counts
	vbuffer_references[triangles[id].vertices[0]]--;
	vbuffer_references[triangles[id].vertices[1]]--;
	vbuffer_references[triangles[id].vertices[2]]--;

	vbuffer_references[t.vertices[0]]++;
	vbuffer_references[t.vertices[1]]++;
	vbuffer_references[t.vertices[2]]++;

	//Update the normal reference counts
	nbuffer_references[triangles[id].normals[0]]--;
	nbuffer_references[triangles[id].normals[1]]--;
	nbuffer_references[triangles[id].normals[2]]--;

	nbuffer_references[t.normals[0]]++;
	nbuffer_references[t.normals[1]]++;
	nbuffer_references[t.normals[2]]++;

	triangles[id] = t;
}

//Gets the number of triangles in the mesh
int Geometry::getNumTriangles()
{
	return triangles.size();
}

//Gets the number of vertices in the mesh
int Geometry::getNumVertices()
{
	return vertices.size();
}

//Gets a triangle by index
Triangle *Geometry::getTriangle(int index)
{
	return &triangles[index];
}

//Gets a vertex by index
Vector3D *Geometry::getVertex(int index)
{
	return &vertices[index];
}

//Gets the number of normals
int Geometry::getNumNormals()
{
	return normals.size();
}

//Gets a normal from the buffer
Vector3D *Geometry::getNormal(int index)
{
	return &normals[index];
}

//Clean up the geometric object when done manipulating
void Geometry::cleanUp()
{
	int num_vertices = vbuffer_references.size();
	int num_triangles = triangles.size();
	
	//Find each vertex with 0 references
	for(int i = 0; i < num_vertices; i++) {
		if(vbuffer_references[i] == 0) {
			vbuffer_references.erase(vbuffer_references.begin() + i);
			vertices.erase(vertices.begin() + i);

			//Decrement vertex indices greater than i
			for(int j = 0; j < num_triangles; j++) {
				if(triangles[j].vertices[0] >= i)
					triangles[j].vertices[0]--;

				if(triangles[j].vertices[1] >= i)
					triangles[j].vertices[1]--;

				if(triangles[j].vertices[2] >= i)
					triangles[j].vertices[2]--;
			}
		}
	}

	int num_normals = nbuffer_references.size();
	
	//Find each vertex with 0 references
	for(int i = 0; i < num_normals; i++) {
		if(nbuffer_references[i] == 0) {
			nbuffer_references.erase(nbuffer_references.begin() + i);
			normals.erase(normals.begin() + i);

			//Decrement normal indices greater than i
			for(int j = 0; j < num_triangles; j++) {
				if(triangles[j].normals[0] >= i)
					triangles[j].normals[0]--;

				if(triangles[j].normals[1] >= i)
					triangles[j].normals[1]--;

				if(triangles[j].normals[2] >= i)
					triangles[j].normals[2]--;
			}
		}
	}
}

//Saves the instance of this object
int Geometry::saveInstance(pugi::xml_node root, int *id, Transform *parent)
{
	//Make sure node pointer is valid
	if(!root)
		return 1;

	std::ostringstream name_stream;
	name_stream << name << "-Inst-" << (*id);
	*id = *id + 1;

	pugi::xml_node node = root.append_child("node");
	node.append_attribute("name") = name_stream.str().c_str();

	//Compound parent transform into the group's transform
	Transform total_t(t);
	if(parent)
		total_t.combine(parent);

	//Add the transform element
	total_t.save(node);

	//Add the geometry instance element
	pugi::xml_node instance_node = node.append_child("instance_geometry");
	instance_node.append_attribute("url") = (std::string("#") + unique_id).c_str();

	return 0;
}

//Saves this geometric object into the specified COLLADA file
int Geometry::saveGeometry(pugi::xml_node root)
{
	//Make sure document pointer is valid
	if(!root)
		return 1;

	//Create the geometry node
	pugi::xml_node geom_node = root.append_child("geometry");
	geom_node.append_attribute("name") = name.c_str();
	geom_node.append_attribute("id") = unique_id.c_str();

	//Add mesh node to store triangle data
	pugi::xml_node mesh_node = geom_node.append_child("mesh");
	
	//Add the source node containing vertex positions
	writeVertexData(mesh_node, VDT_POSITION);

	//Add the source node containing vertex normals
	writeVertexData(mesh_node, VDT_NORMAL);

	//Add the source node containing vertex uv coordinates
	writeVertexData(mesh_node, VDT_UV);

	//Add the vertex node
	pugi::xml_node vertex_node = mesh_node.append_child("vertices");
	vertex_node.append_attribute("id") = (unique_id + "-Vtx").c_str();

	pugi::xml_node vinput_node = vertex_node.append_child("input");
	vinput_node.append_attribute("semantic") = "POSITION";
	vinput_node.append_attribute("source") = (std::string("#") + unique_id + "-Pos").c_str();

	//Add the triangles node
	writeTriangleData(mesh_node);

	return 0;
}

//Save the source node with vertices
void Geometry::writeVertexData(pugi::xml_node root, vertex_data_type data_type)
{
	//Determine the name of this source node
	std::string vsn_name;
	switch(data_type) {
	case VDT_POSITION:
		vsn_name = unique_id + "-Pos";
		break;
	case VDT_NORMAL:
		vsn_name = unique_id + "-Normal";
		break;
	case VDT_UV:
		vsn_name = unique_id + "-Tex";
		break;
	default:
		return;
	}

	//Determine the stride
	int stride = 2;
	if(data_type == VDT_POSITION || data_type == VDT_NORMAL)
		stride = 3;

	//Determine the count
	int count = 0;
	if(data_type == VDT_POSITION)
		count = vertices.size();
	else if(data_type == VDT_NORMAL)
		count = normals.size();
	else
		count = triangles.size() * 3;

	//Create the source node
	pugi::xml_node source_node = root.append_child("source");
	source_node.append_attribute("id") = vsn_name.c_str();

	std::string vsna_name = vsn_name + "-array";
	pugi::xml_node vfa_node = source_node.append_child("float_array");
	vfa_node.append_attribute("id") = vsna_name.c_str();
	vfa_node.append_attribute("count") = stride * count;

	//Create indentation for newlines
	char indent[20];
	int depth = vfa_node.depth() - 2;
	int idx;
	for(idx = 0; idx < depth; idx++)
		indent[idx] = '\t';
	indent[idx] = '\0';

	//Add each data value to the node
	std::ostringstream va_string_stream;
	va_string_stream << std::setiosflags(std::ios::showpoint) << std::endl << indent;
	if(data_type == VDT_POSITION) {
		//Write out per vertex information
		int num_vertices = vertices.size();
		for(int i = 0; i < num_vertices; i++) {
			va_string_stream << DEC_FORMAT << vertices[i].x << " ";
			va_string_stream << DEC_FORMAT << vertices[i].y << " ";
			va_string_stream << DEC_FORMAT << vertices[i].z << std::endl << indent;
		}
	} else if(data_type == VDT_NORMAL) {
		//Write out each normal in the buffer
		int num_normals = normals.size();
		for(int i = 0; i < num_normals; i++) {
			va_string_stream << DEC_FORMAT << normals[i].x << " ";
			va_string_stream << DEC_FORMAT << normals[i].y << " ";
			va_string_stream << DEC_FORMAT << normals[i].z << std::endl << indent;
		}
	} else {
		//Write out per triangle-vertex information
		int num_tris = triangles.size();
		
		switch(data_type){
		case VDT_UV:
			for(int i = 0; i < num_tris; i++) {
				for(int j = 0; j < 3; j++) {
					va_string_stream << DEC_FORMAT << triangles[i].uvs[j].u << " ";
					va_string_stream << DEC_FORMAT << triangles[i].uvs[j].v << std::endl << indent;
				}
			}
			break;
		}
	}

	//Build the string from the stream and copy to the node
	std::string va_string = va_string_stream.str();
	vfa_node.text() = va_string.c_str();

	//Add the technique node
	pugi::xml_node vtechnique_node = source_node.append_child("technique_common");
	pugi::xml_node vta_node = vtechnique_node.append_child("accessor");
	vta_node.append_attribute("source") = (std::string("#") + vsna_name).c_str();
	vta_node.append_attribute("count") = count;
	vta_node.append_attribute("stride") = stride;

	if(data_type == VDT_POSITION || data_type == VDT_NORMAL) {
		//3D points
		pugi::xml_node param1 = vta_node.append_child("param");
		param1.append_attribute("name") = "X";
		param1.append_attribute("type") = "float";
		pugi::xml_node param2 = vta_node.append_child("param");
		param2.append_attribute("name") = "Y";
		param2.append_attribute("type") = "float";
		pugi::xml_node param3 = vta_node.append_child("param");
		param3.append_attribute("name") = "Z";
		param3.append_attribute("type") = "float";
	} else {
		//2D points
		pugi::xml_node param1 = vta_node.append_child("param");
		param1.append_attribute("name") = "S";
		param1.append_attribute("type") = "float";
		pugi::xml_node param2 = vta_node.append_child("param");
		param2.append_attribute("name") = "T";
		param2.append_attribute("type") = "float";
	}

	return;
}

//Adds the triangles node to a COLLADA file
void Geometry::writeTriangleData(pugi::xml_node root)
{
	pugi::xml_node triangles_node = root.append_child("triangles");
	triangles_node.append_attribute("count") = triangles.size();

	//Specify the format of the data
	pugi::xml_node input1 = triangles_node.append_child("input");
	input1.append_attribute("semantic") = "VERTEX";
	input1.append_attribute("source") = (std::string("#") + unique_id + "-Vtx").c_str();
	input1.append_attribute("offset") = 0;

	pugi::xml_node input2 = triangles_node.append_child("input");
	input2.append_attribute("semantic") = "NORMAL";
	input2.append_attribute("source") = (std::string("#") + unique_id + "-Normal").c_str();
	input2.append_attribute("offset") = 1;

	pugi::xml_node input3 = triangles_node.append_child("input");
	input3.append_attribute("semantic") = "TEXCOORD";
	input3.append_attribute("source") = (std::string("#") + unique_id + "-Tex").c_str();
	input3.append_attribute("offset") = 2;

	//Add the primitive node
	pugi::xml_node p_node = triangles_node.append_child("p");

	//Determine indent
	char indent[20];
	int depth = p_node.depth() - 1;
	int idx;
	for(idx = 0; idx < depth; idx++)
		indent[idx] = '\t';
	indent[idx] = '\0';

	//Add indices for each triangle to the node
	std::ostringstream tri_stream;
	tri_stream << std::endl << indent;
	int num_tris = triangles.size();
	int last_tri = num_tris - 1;
	int index = 0;
	for(int i = 0; i < num_tris; i++) {
		for(int j = 0; j < 3; j++) {
			tri_stream << triangles[i].vertices[j] << " ";
			tri_stream << triangles[i].normals[j] << " ";
			tri_stream << index;

			if(j != 2)
				tri_stream << "  ";

			index++;
		}

		if(i == last_tri)
			indent[depth - 1] = '\0';

		tri_stream << std::endl << indent;
	}

	//Build string from stream and add to the node
	std::string tri_string = tri_stream.str();
	p_node.text() = tri_string.c_str();

	return;
}

//Generation function does nothing for default
void Geometry::generate(int seed, Scene *scene)
{

}

//Get the object's transform
Transform *Geometry::getTransform()
{
	return &t;
}

//Add a filter to the object
void Geometry::addFilter(GeometryFilter *filter)
{
	filters.push_back(filter);
}

//Apply each filter to the object in order
void Geometry::filter()
{
	int num_filters = filters.size();
	for(int i = 0; i < num_filters; i++)
		filters[i]->run(this);
}

//Clears mesh data in the geometry
void Geometry::clearMesh()
{
	vertices.clear();
	vbuffer_references.clear();
	normals.clear();
	nbuffer_references.clear();
	triangles.clear();
}

//Clones the mesh data into another geometry
void Geometry::cloneMesh(Geometry *g)
{
	int num_vertices = vertices.size();
	int num_normals = normals.size();
	int num_triangles = triangles.size();

	//Clone each vertex
	for(int i = 0; i < num_vertices; i++)
		g->addVertex(vertices[i]);

	//Clone each normal
	for(int i = 0; i < num_normals; i++)
		g->addNormal(normals[i]);

	//Clone each triangle
	for(int i = 0; i < num_triangles; i++)
		g->addTriangle(triangles[i]);
}
