/** @file Scene.cpp
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

#include "Scene.h"

//Default constructor
Scene::Scene()
:objects(), name("ShockShapes-Scene"), units_per_meter(1.0f)
{

}

//Named constructor
Scene::Scene(const char *sname)
:objects(), name(sname), units_per_meter(1.0f)
{

}

//Destructor
Scene::~Scene()
{

}

//Add a geometric object to the scene
int Scene::addObject(Geometry *g)
{
	int id = objects.size();
	objects.push_back(g);

	return id;
}

//Generate the scene
void Scene::generate(int seed)
{
	//Generate each object
	int num_objects = objects.size();
	for(int i = 0; i < num_objects; i++) {
		objects[i]->generate(seed, this);

		objects[i]->filter();
	}
}

//Save the scene to a COLLADA file
int Scene::save(const char *filename)
{
	char buf[128];
	time_t now;

	//Initialize the document root
	pugi::xml_document doc;
	pugi::xml_node collada_node = doc.append_child("COLLADA");
	collada_node.append_attribute("xmlns") = "http://www.collada.org/2008/03/COLLADASchema";
	collada_node.append_attribute("version") = "1.5.0";

	//Add an asset node
	pugi::xml_node asset_node = collada_node.append_child("asset");

	pugi::xml_node cont_node = asset_node.append_child("contributor");
	pugi::xml_node tool_node = cont_node.append_child("authoring_tool");
	tool_node.text() = VERSION_STRING;

	pugi::xml_node created_node = asset_node.append_child("created");
	pugi::xml_node modified_node = asset_node.append_child("modified");
	time(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", gmtime(&now));
	created_node.text() = buf;
	modified_node.text() = buf;

	pugi::xml_node unit_node = asset_node.append_child("unit");
	unit_node.append_attribute("meter") = 1.0f / units_per_meter;

	pugi::xml_node up_node = asset_node.append_child("up_axis");
	up_node.text() = "Y_UP";

	//Add the library of geometric objects and scenes
	pugi::xml_node lib_geometry = collada_node.append_child("library_geometries");
	pugi::xml_node lib_scenes = collada_node.append_child("library_visual_scenes");
	pugi::xml_node vscene_node = lib_scenes.append_child("visual_scene");
	vscene_node.append_attribute("id") = "DefaultScene";

	//Add the scene node
	pugi::xml_node scene_node = collada_node.append_child("scene");
	pugi::xml_node scene_inst_node = scene_node.append_child("instance_visual_scene");
	scene_inst_node.append_attribute("url") = "#DefaultScene";

	//Add each geometric object to the library and the scene
	int num_objects = objects.size();
	int id = 0;
	for(int i = 0; i < num_objects; i++) {
		if(objects[i]->saveGeometry(lib_geometry))
			return 1;

		if(objects[i]->saveInstance(vscene_node, &id, NULL))
			return 1;
	}

	//Write to the file
	if(!doc.save_file(filename))
		return 1;

	return 0;
}
