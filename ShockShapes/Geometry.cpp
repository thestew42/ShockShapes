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
#include "CSourceLib.h"

int current_id = 0; /**< Incrementing number used for unique IDs. */

//Geometry constructor creates empty mesh
Geometry::Geometry()
:vertices(), vbuffer_references(), normals(), nbuffer_references(), triangles(), name("Geometry"), unique_id(""), t(), filters()
{
	id = current_id++;
	visible = true;

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
	visible = true;

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

//Get the name
const char *Geometry::getName()
{
	return name.c_str();
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
	unsigned int num_vertices = vbuffer_references.size();
	unsigned int num_triangles = triangles.size();
	unsigned int num_normals = nbuffer_references.size();
	unsigned int original_num_vertices = num_vertices;
	unsigned int original_num_normals = num_normals;
	unsigned int original_index;

	std::vector<Vector3D> new_list;
	std::vector<int> new_references;

	std::vector<unsigned int> removed_vertices;
	std::vector<unsigned int> removed_normals;
	unsigned int *vertex_remap = new unsigned int[num_vertices];
	unsigned int *normal_remap = new unsigned int[num_normals];

	for(unsigned int i = 0; i < num_vertices; i++)
		vertex_remap[i] = i;

	for(unsigned int i = 0; i < num_normals; i++)
		normal_remap[i] = i;
	
	//Find each vertex with 0 references
	original_index = 0;
	for(unsigned int i = 0; i < num_vertices; i++) {
		if(vbuffer_references[i] == 0) {
			removed_vertices.push_back(original_index);
		}
		else
		{
			new_list.push_back(vertices[i]);
			new_references.push_back(vbuffer_references[i]);
		}

		original_index++;
	}

	vertices = new_list;
	vbuffer_references = new_references;
	new_list.clear();
	new_references.clear();
	
	//Find each vertex with 0 references
	original_index = 0;
	for(unsigned int i = 0; i < num_normals; i++) {
		if(nbuffer_references[i] == 0) {
			removed_normals.push_back(original_index);
		}
		else
		{
			new_list.push_back(normals[i]);
			new_references.push_back(nbuffer_references[i]);
		}

		original_index++;
	}

	normals = new_list;
	nbuffer_references = new_references;
	new_list.clear();
	new_references.clear();

	//Generate remap from list of removed vertices/normals
	int removed_index = 0;
	int current_offset = 0;
	for(unsigned int i = 0; i < original_num_vertices; i++)
	{
		vertex_remap[i] = current_offset;
		if(i < removed_vertices.size() && i == removed_vertices[removed_index])
		{
			removed_index++;
			current_offset++;
		}
	}

	removed_index = 0;
	current_offset = 0;
	for(unsigned int i = 0; i < original_num_normals; i++)
	{
		normal_remap[i] = current_offset;
		if(i < removed_normals.size() && i == removed_normals[removed_index])
		{
			removed_index++;
			current_offset++;
		}
	}

	//Update indices in each triangle
	for(unsigned int i = 0; i < num_triangles; i++)
	{
		triangles[i].normals[0] -= normal_remap[triangles[i].normals[0]];
		triangles[i].normals[1] -= normal_remap[triangles[i].normals[1]];
		triangles[i].normals[2] -= normal_remap[triangles[i].normals[2]];

		triangles[i].vertices[0] -= vertex_remap[triangles[i].vertices[0]];
		triangles[i].vertices[1] -= vertex_remap[triangles[i].vertices[1]];
		triangles[i].vertices[2] -= vertex_remap[triangles[i].vertices[2]];
	}

	delete vertex_remap;
	delete normal_remap;
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

//Read geometry data from COLLADA node
int Geometry::readGeometry(pugi::xml_node root)
{
	//Make sure node is valid
	if(!root)
		return 1;

	//Clear mesh of any data
	clearMesh();

	//Array of sources for mesh data
	CSourceLib sources;

	//Inputs for vertices and triangles
	CSource *positions = NULL;
	CSource *normals = NULL;
	CSource *tex1_coords = NULL;

	//Set to true if normals indexed the same as vertices
	bool vtxNormals = false;
	bool triNormals = false;
	//Set to true if tex coords indexed the same as vertices
	bool vtxTexCoords = false;
	bool triTexCoords = false;

	//Offsets in triangle data of each attribute
	unsigned int vtxOffset, normalOffset, texOffset;
	unsigned int numInputs;

	//Read name of the geometry
	if(root.attribute("name"))
	{
		pugi::xml_attribute name_attr = root.attribute("name");
		name = name_attr.value();
	}
	else
	{
		name = "Geometry";
	}

	if(root.attribute("id"))
	{
		pugi::xml_attribute id_attr = root.attribute("id");
		unique_id = id_attr.value();
	}
	else
	{
		id = current_id++;

		//Build the unique id for this object
		std::ostringstream unique_id_stream;
		unique_id_stream << name << id;
		unique_id = unique_id_stream.str();
	}

	//Get the mesh node
	pugi::xml_node mesh_node = root.child("mesh");
	if(!mesh_node)
		return 2;

	//Iterate over each child of the mesh and process if supported
	for(pugi::xml_node child = mesh_node.first_child(); child; child = child.next_sibling()) {
		//Currently not supported: lines, linestrips, polygons, polylist
		if(!strcmp(child.name(), "source")) {
			sources.addSource(child);
		} else if(!strcmp(child.name(), "vertices")) {
			//Find source id's for each input semetic
			for(pugi::xml_node v_input = child.first_child(); v_input; v_input = v_input.next_sibling()) {
				if(!strcmp(v_input.name(), "input")) {
					if(!strcmp(v_input.attribute("semantic").value(), "POSITION")) {
						positions = sources.getSource(v_input.attribute("source").value());
						copyVertexData(positions);
					} else if(!strcmp(v_input.attribute("semantic").value(), "NORMAL")) {
						normals = sources.getSource(v_input.attribute("source").value());
						copyNormalData(normals);
						vtxNormals = true;
					} else if(!strcmp(v_input.attribute("semantic").value(), "TEXCOORD")) {
						tex1_coords = sources.getSource(v_input.attribute("source").value());
						vtxTexCoords = true;
					}
				}
			}
		} else if(!strcmp(child.name(), "triangles")) {
			//Get per triangle inputs
			numInputs = 0;
			for(pugi::xml_node t_input = child.first_child(); t_input; t_input = t_input.next_sibling()) {
				if(!strcmp(t_input.name(), "input")) {
					numInputs++;
					if(!strcmp(t_input.attribute("semantic").value(), "VERTEX")) {
						vtxOffset = atoi(t_input.attribute("offset").value());
					} else if(!strcmp(t_input.attribute("semantic").value(), "NORMAL")) {
						normals = sources.getSource(t_input.attribute("source").value());
						copyNormalData(normals);
						triNormals = true;
						normalOffset = atoi(t_input.attribute("offset").value());
					} else if(!strcmp(t_input.attribute("semantic").value(), "TEXCOORD")) {
						tex1_coords = sources.getSource(t_input.attribute("source").value());
						triTexCoords = true;
						texOffset = atoi(t_input.attribute("offset").value());
					}
				}
			}

			//Buffer for each triangle
			int bufferSize = numInputs * 3;
			int *triBuffer = new int[bufferSize];

			//Read the primitive descriptors and add triangles to the mesh
			int numTriangles = atoi(child.attribute("count").value());
			if(child.child("p")) {
				std::string text = child.child("p").text().get();
				std::istringstream iss(text);

				for(int i = 0; i < numTriangles; i++) {
					//Read indices into buffer
					for(int j = 0; j < bufferSize; j++)
						iss >> triBuffer[j];
					
					//Construct the triangle from buffer data
					Triangle t;
					t.vertices[0] = triBuffer[vtxOffset];
					t.vertices[1] = triBuffer[numInputs + vtxOffset];
					t.vertices[2] = triBuffer[numInputs + numInputs + vtxOffset];

					//Normal indices if they are present
					if(vtxNormals) {
						t.normals[0] = triBuffer[vtxOffset];
						t.normals[1] = triBuffer[numInputs + vtxOffset];
						t.normals[2] = triBuffer[numInputs + numInputs + vtxOffset]; 
					} else if(triNormals) {
						t.normals[0] = triBuffer[normalOffset];
						t.normals[1] = triBuffer[numInputs + normalOffset];
						t.normals[2] = triBuffer[numInputs + numInputs + normalOffset];
					}

					//Copy texture coordinates from source if available
					if(vtxTexCoords) {
						Vector2D coord;
						int index;
						
						index = triBuffer[vtxOffset];
						coord.u = tex1_coords->accessFloatParameter(index, PT_S);
						coord.v = tex1_coords->accessFloatParameter(index, PT_T);
						t.uvs[0] = coord;

						index = triBuffer[numInputs + vtxOffset];
						coord.u = tex1_coords->accessFloatParameter(index, PT_S);
						coord.v = tex1_coords->accessFloatParameter(index, PT_T);
						t.uvs[1] = coord;

						index = triBuffer[numInputs + numInputs + vtxOffset];
						coord.u = tex1_coords->accessFloatParameter(index, PT_S);
						coord.v = tex1_coords->accessFloatParameter(index, PT_T);
						t.uvs[2] = coord;
					} else if(triTexCoords) {
						Vector2D coord;
						int index;
						
						index = triBuffer[texOffset];
						coord.u = tex1_coords->accessFloatParameter(index, PT_S);
						coord.v = tex1_coords->accessFloatParameter(index, PT_T);
						t.uvs[0] = coord;

						index = triBuffer[numInputs + texOffset];
						coord.u = tex1_coords->accessFloatParameter(index, PT_S);
						coord.v = tex1_coords->accessFloatParameter(index, PT_T);
						t.uvs[1] = coord;

						index = triBuffer[numInputs + numInputs + texOffset];
						coord.u = tex1_coords->accessFloatParameter(index, PT_S);
						coord.v = tex1_coords->accessFloatParameter(index, PT_T);
						t.uvs[2] = coord;
					}

					//Add triangle to mesh
					addTriangle(t);
				}
			}

			delete triBuffer;
		} else if(!strcmp(child.name(), "trifans")) {
			//TODO
		} else if(!strcmp(child.name(), "tristrips")) {
			//TODO
		}
    }

	if(vtxNormals && triNormals) {
		//ERROR
		return 1;
	}

	if(vtxTexCoords && triTexCoords) {
		//ERROR
		return 1;
	}

	return 0;
}

//Copy vertex data
void Geometry::copyVertexData(CSource *source)
{
	int nVertices = source->getNumElements();
	
	//Copy each vertex
	for(int i = 0; i < nVertices; i++)
	{
		Vector3D v;
		v.x = source->accessFloatParameter(i, PT_X);
		v.y = source->accessFloatParameter(i, PT_Y);
		v.z = source->accessFloatParameter(i, PT_Z);

		addVertex(v);
	}
}

//Copy normal data
void Geometry::copyNormalData(CSource *source)
{
	int nNormals = source->getNumElements();
	
	//Copy each vertex
	for(int i = 0; i < nNormals; i++)
	{
		Vector3D n;
		n.x = source->accessFloatParameter(i, PT_X);
		n.y = source->accessFloatParameter(i, PT_Y);
		n.z = source->accessFloatParameter(i, PT_Z);

		addNormal(n);
	}
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

//Set whether object is visible
void Geometry::setVisibility(bool v)
{
	visible = v;
}

//Gets visibility
bool Geometry::isVisible()
{
	return visible;
}

//Combine with another geometry
void Geometry::combineInto(Geometry *g, Matrix *parent_t)
{
	unsigned int g_num_vertices = g->getNumVertices();
	unsigned int g_num_normals = g->getNumNormals();

	unsigned int this_num_vertices = getNumVertices();
	unsigned int this_num_normals = getNumNormals();
	unsigned int this_num_triangles = getNumTriangles();

	//Compute total transform
	Matrix total_transform = t.m;
	if(parent_t != NULL)
		total_transform.multiply(parent_t);

	//Copy over vertices and normals
	Vector3D cur_vec;
	for(unsigned int i = 0; i < this_num_vertices; i++)
	{
		//Transform and add vertex
		cur_vec.x = vertices[i].x * total_transform.r0[0];
		cur_vec.x += vertices[i].y * total_transform.r0[1];
		cur_vec.x += vertices[i].z * total_transform.r0[2];
		cur_vec.x += total_transform.r3[0];
		
		cur_vec.y = vertices[i].x * total_transform.r1[0];
		cur_vec.y += vertices[i].y * total_transform.r1[1];
		cur_vec.y += vertices[i].z * total_transform.r1[2];
		cur_vec.y += total_transform.r3[1];

		cur_vec.z = vertices[i].x * total_transform.r2[0];
		cur_vec.z += vertices[i].y * total_transform.r2[1];
		cur_vec.z += vertices[i].z * total_transform.r2[2];
		cur_vec.z += total_transform.r3[2];

		g->addVertex(cur_vec);
	}

	for(unsigned int i = 0; i < this_num_normals; i++)
	{
		//Transform and add vertex
		cur_vec.x = normals[i].x * total_transform.r0[0];
		cur_vec.x += normals[i].y * total_transform.r0[1];
		cur_vec.x += normals[i].z * total_transform.r0[2];
		cur_vec.x += total_transform.r3[0];
		
		cur_vec.y = normals[i].x * total_transform.r1[0];
		cur_vec.y += normals[i].y * total_transform.r1[1];
		cur_vec.y += normals[i].z * total_transform.r1[2];
		cur_vec.y += total_transform.r3[1];

		cur_vec.z = normals[i].x * total_transform.r2[0];
		cur_vec.z += normals[i].y * total_transform.r2[1];
		cur_vec.z += normals[i].z * total_transform.r2[2];
		cur_vec.z += total_transform.r3[2];

		g->addNormal(cur_vec);
	}

	//Copy over triangles
	Triangle cur_tri;
	for(unsigned int i = 0; i < this_num_triangles; i++)
	{
		cur_tri = triangles[i];

		//Update indices to account for already present geometry
		cur_tri.normals[0] += g_num_normals;
		cur_tri.normals[1] += g_num_normals;
		cur_tri.normals[2] += g_num_normals;

		cur_tri.vertices[0] += g_num_vertices;
		cur_tri.vertices[1] += g_num_vertices;
		cur_tri.vertices[2] += g_num_vertices;

		g->addTriangle(cur_tri);
	}
}
