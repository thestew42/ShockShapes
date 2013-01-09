/** @file Instance.cpp
 * 
 * @brief Object that is an instance of another object
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

#include "Instance.h"

//Constructor
Instance::Instance(Geometry *obj)
{
	original = obj;
}

//Destructor
Instance::~Instance()
{

}

//Saving geometry does nothing since this is just an instance
int Instance::saveGeometry(pugi::xml_node root)
{
	return 0;
}

//Saves an instance of the original object
int Instance::saveInstance(pugi::xml_node root, int *id, Transform *parent)
{
	//Make sure node pointer is valid
	if(!root)
		return 1;

	std::ostringstream name_stream;
	name_stream << original->getUniqueId() << "-Inst-" << (*id);
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
	instance_node.append_attribute("url") = (std::string("#") + original->getUniqueId()).c_str();

	return 0;
}

//Override filtering
void Instance::filter()
{

}
