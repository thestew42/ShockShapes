/** @file Scene.h
 * 
 * @brief Defines behavior of the base node of a file which contains all other nodes
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

#ifndef _SCENE_
#define _SCENE_

#include <vector>
#include <string>
#include <ctime>

#include "pugixml.hpp"
#include "Geometry.h"
#include "Group.h"

/**
 * @brief Manages scene assets and file I/O for scenes
 * Initializes file output of the scene and calls other output methods
 * Stores objects in the scene
 */
class Scene {
private:
	/**
	 * List of visible geometric objects in the scene
	 */
	std::vector<Geometry*> objects;

	std::string name;

	float units_per_meter;

public:
	Scene();					/**< Default empty scene constructor. */
	Scene(const char *sname);	/**< Constructor that names the scene. */

	~Scene();					/**< Destructor. */

	/**
	 * Adds a geometric object to the scene
	 * @param g Pointer to an object that is any child of Geometry
	 * @return Returns the id number that can be used to query the object
	 */
	int addObject(Geometry *g);

	/**
	 * Generates the scene and all objects contained
	 * @param seed Number to seed the random number generator with
	 */
	void generate(int seed);

	/**
	 * Writes the scene to a COLLADA file with the supplied name
	 * @param filename The local file name to use for writing
	 * @return Returns 0 if no errors occur
	 */
	int save(const char *filename);

	/**
	 * Loads the scene from a COLLADA file with the supplied name
	 * @param filename The local file to load from
	 * @return Returns 0 if no errors occur
	 */
	int load(const char *filename);

	/**
	 * Load a node object from a COLLADA file
	 * @param node The xml COLLADA node to process
	 * @param parent Pointer to the group to add any children to or NULL for top level
	 */
	void loadNode(pugi::xml_node node, Group *parent);

	/**
	 * Finds a geometric object by id
	 * @param id The string to search for
	 * @param A pointer to the geometry object or NULL
	 */
	Geometry *findObject(const char *id);

	/**
	 * Clears a scene of all data
	 */
	void clear();
};

#endif
