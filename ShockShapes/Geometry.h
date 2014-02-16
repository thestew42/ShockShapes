/** @file Geometry.h
 * 
 * @brief Defines everything needed for geometric object manipulation and storage
 * 
 * Copyright 2012-2013, Stewart Hall
 *
 * Licensed under The MIT License
 * Redistributions of files must retain the above copyright notice.
 *
 * @author		Stewart Hall (www.stewartghall.com)
 * @date		12/29/2012
 * @copyright	Copyright 2012-2013, Stewart Hall
 * @license		MIT License (http://www.opensource.org/licenses/mit-license.php)
 */

#ifndef _GEOMETRY_
#define _GEOMETRY_

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>

#include "pugixml.hpp"
#include "CommonDefs.h"
#include "Transform.h"
#include "GeometryFilter.h"
#include "CSource.h"

/**
 * @brief Stores information about a single triangle
 */
typedef struct {
	int vertices[3];
	int normals[3];

	Vector2D uvs[3];
} Triangle;

/**
 * @brief Defines the different types of per vertex data
 */
enum vertex_data_type {
	VDT_POSITION,
	VDT_NORMAL,
	VDT_UV
};

class Scene;

/**
 * @brief Base class for all geometric objects
 * @details Stores all geometry information including vertices, faces, and
 * materials. Also stores generation parameters and geometry filters. Any
 * child classes should implement the generate method which builds an instance
 * of the geometric object.
 */
class Geometry
{
private:
	/**
	 * Vertex buffer storage
	 * An array of vertices referenced by triangles
	 */
	std::vector<Vector3D> vertices;

	/**
	 * Reference counters for the vertex buffer
	 * Keeps track of which vertices are in use so that garbage collection can be performed.
	 */
	std::vector<int> vbuffer_references;

	/**
	 * Normal buffer storage
	 * An array of normals referenced by triangles
	 */
	std::vector<Vector3D> normals;

	/**
	 * Reference counters for the normal buffer
	 */
	std::vector<int> nbuffer_references;

	/**
	 * Triangle buffer storage
	 * An array of triangles in the geometry.
	 */
	std::vector<Triangle> triangles;

	std::string name;		/**< Name of the object used when saving. */

	int id;					/**< ID number of this object. */

	std::string unique_id;	/**< Concatenation of name and id used for COLLADA unique id. */

	bool visible;			/**< If visible is false, this geometry is purely used as a base for instancing. */

	/**
	 * Writes a vertex data array of this geometry to a COLLADA source node
	 * @param root pugixml node to add the source node to
	 * @param data_type the type of per vertex data to create a source for
	 */
	void writeVertexData(pugi::xml_node root, vertex_data_type data_type);

	/**
	 * Writes the triangle data to a COLLADA triangles node
	 * @param root pugixml node to add the triangles to
	 */
	void writeTriangleData(pugi::xml_node root);

	/**
	 * Copies source data to vertex buffer
	 * @param source The source containing vertex positions
	 */
	void copyVertexData(CSource *source);

	/**
	 * Copies source data to normal buffer
	 * @param source The source containing normal vectors
	 */
	void copyNormalData(CSource *source);

protected:
	/**
	 * List of filters to apply to this object
	 */
	std::vector<GeometryFilter*> filters;

	Transform t;			/**< World space transform for this geometric object. */

public:
	Geometry();						/**< Constructs an empty geometry. */
	Geometry(const char *iname);	/**< Constructs object with different name. */

	~Geometry();					/**< Destructor. */

	/**
	 * Get the unique id string for this object
	 * @return The object's unique string ID
	 */
	const char *getUniqueId();

	/**
	 * Get the name of the object
	 * @return the object's name
	 */
	const char *getName();

	/**
	 * Adds a vertex to the buffer
	 * @param v The vertex to copy to the buffer.
	 * @return The index of the added vertex
	 */
	int addVertex(Vector3D v);

	/**
	 * Adds a triangle to the buffer
	 * @param t The triangle to copy to the buffer
	 * @return The index of the added triangle
	 */
	int addTriangle(Triangle t);

	/**
	 * Cleans up a model before saving
	 * Removes unused vertices from the buffer
	 */
	void cleanUp();

	/**
	 * Saves the geometry in COLLADA format to the file specified
	 * @param root Pointer to a pugixml node to write geometry in
	 * @return Returns 0 if no errors occur
	 */
	virtual int saveGeometry(pugi::xml_node root);

	/**
	 * Saves an instance node in COLLADA format to the scene
	 * @param root The scene element to add the node to
	 * @param id Unique suffix to place after the name of the node
	 * @param parent The transform on the parent object
	 * @return Returns 0 if no errors occur
	 */
	virtual int saveInstance(pugi::xml_node root, int *id, Transform *parent);

	/**
	 * Read mesh data from a COLLADA node into the object's buffers
	 * @param node The COLLADA geometry node to read the mesh from
	 * @return Returns 0 if no errors occur
	 */
	virtual int readGeometry(pugi::xml_node root);

	/**
	 * Generates the geometry with the class' parameters
	 * @param seed The value to seed the random number generator with
	 * @param scene The scene that this geometry will belong to
	 */
	virtual void generate(int seed, Scene *scene);

	/**
	 * Runs filters on the geometry
	 */
	virtual void filter();

	/**
	 * Adds a filter to the geometric object
	 * @param filter The filter to apply to the geometric object
	 */
	void addFilter(GeometryFilter *filter);

	/**
	 * Sets the triangle with the specified index
	 * @param id Index to copy to
	 * @param t Triangle to copy to buffer
	 */
	void setTriangle(int id, Triangle t);

	/**
	 * Gets the total number of triangles in the mesh
	 * @return The number of triangles in the mesh
	 */
	int getNumTriangles();

	/**
	 * Gets the total number of vertices
	 * @return The number of vertices
	 */
	int getNumVertices();

	/**
	 * Gets a triangle by index
	 * @param index The index of the triangle
	 * @return The triangle of the specified index
	 */
	Triangle *getTriangle(int index);

	/**
	 * Gets a vertex by index
	 * @param index The index of the vertex to get
	 * @return The vertex at that index
	 */
	Vector3D *getVertex(int index);

	/**
	 * Gets the total number of normals
	 * @return The number of vertices stored
	 */
	int getNumNormals();

	/**
	 * Gets a normal
	 * @param index The index of the normal to return
	 * @return A pointer to the normal at the specified index
	 */
	Vector3D *getNormal(int index);

	/**
	 * Adds a normal to the buffer
	 * @param n The normal to add
	 * @return The index of the added normal in the buffer
	 */
	int addNormal(Vector3D n);

	/**
	 * Gets the transform so the user can modify it
	 * @return A pointer to this object's transform
	 */
	Transform *getTransform();

	/**
	 * Clears the mesh data stored in the object
	 */
	void clearMesh();

	/**
	 * Clones the mesh data into another empty geometry object
	 * @param g The geometry to clone into. Must be an empty mesh.
	 */
	void cloneMesh(Geometry *g);

	/**
	 * Sets whether the object is visible or not
	 * @param visible
	 */
	void setVisibility(bool v);

	/**
	 * Gets the visibility status of the geometry
	 * @return True if visible
	 */
	bool isVisible();
};

#endif
