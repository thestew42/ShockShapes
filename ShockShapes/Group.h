/** @file Group.h
 * 
 * @brief Stores a group of geometric objects which can be treated as a single object
 * 
 * Copyright 2013, Stewart Hall
 *
 * Licensed under The MIT License
 * Redistributions of files must retain the above copyright notice.
 *
 * @author		Stewart Hall (www.stewartghall.com)
 * @date		1/9/2013
 * @copyright	Copyright 2013, Stewart Hall
 * @license		MIT License (http://www.opensource.org/licenses/mit-license.php)
 */

#ifndef _GROUP_
#define _GROUP_

#include <vector>

#include "Geometry.h"

/**
 * @brief A group of other objects
 * @details Stores a group of geometric objects and configurations of these
 * which can be transformed as a single object
 */
class Group : public Geometry {
private:
	/**
	 * An array of the objects that make up this group
	 */
	std::vector<Geometry*> objects;

public:
	Group();					/**< Constructor for an empty group. */
	Group(const char* name);	/**< Constructor for an empty named group. */

	~Group();					/**< Destructor. */

	/**
	 * Saves the objects in COLLADA format to the file specified
	 * @param root Pointer to a pugixml node to write geometry in
	 * @return Returns 0 if no errors occur
	 */
	virtual int saveGeometry(pugi::xml_node root);

	/**
	 * Saves an instance node in COLLADA format to the scene
	 * @param root The scene element to add the node to
	 * @param id Unique suffix to place after the name of the node
	 * @return Returns 0 if no errors occur
	 */
	virtual int saveInstance(pugi::xml_node root, int *id, Transform *parent);

	/**
	 * Runs filters on each sub-object in the group
	 */
	virtual void filter();

	/**
	 * Adds a geometric object to this group
	 * @param g The object to add to this group
	 */
	void addObject(Geometry *g);

	/**
	 * Generates any sub-objects
	 * @param seed The value to seed the random number generator with
	 * @param scene The scene that this geometry will belong to
	 */
	virtual void generate(int seed, Scene *scene);
};

#endif
