/** @file Group.cpp
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

#include "Group.h"

//Default constructor
Group::Group()
:Geometry("Group"), objects()
{

}

//Named constructor
Group::Group(const char *name)
:Geometry(name), objects()
{

}

//Destructor
Group::~Group()
{

}

//Add an object to the group
void Group::addObject(Geometry *g)
{
	objects.push_back(g);
}

//Save the geometry of the group
int Group::saveGeometry(pugi::xml_node root)
{
	int num_objects = objects.size();
	int result;

	//Iterates through each sub-object and calls its save
	for(int i = 0; i < num_objects; i++) {
		if(result = objects[i]->saveGeometry(root))
			return result;
	}

	return 0;
}

//Save instances in the group
int Group::saveInstance(pugi::xml_node root, int *id, Transform *parent)
{
	int num_objects = objects.size();
	int result;

	//Compound parent transform into the group's transform
	Transform total_t(t);
	if(parent)
		total_t.combine(parent);

	//Iterates through each sub-object and calls its save
	for(int i = 0; i < num_objects; i++) {
		if(result = objects[i]->saveInstance(root, id, &total_t))
			return result;
	}

	return 0;
}

//Applies filters to each object in the group
void Group::filter()
{
	//Filter each object individually first
	int num_objects = objects.size();
	for(int i = 0; i < num_objects; i++)
		objects[i]->filter();

	//Filter all objects under the group filters
	int num_filters = filters.size();
	for(int i = 0; i < num_filters; i++)
		for(int j = 0; j < num_objects; j++)
			filters[i]->run(objects[j]);
}

//Generates all sub objects
void Group::generate(int seed, Scene *scene)
{
	int num_objects = objects.size();
	for(int i = 0; i < num_objects; i++)
		objects[i]->generate(seed, scene);
}
