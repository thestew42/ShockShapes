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
#include "Instance.h"

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

		if(objects[i]->isVisible() && objects[i]->saveInstance(vscene_node, &id, NULL))
			return 1;
	}

	//Write to the file
	if(!doc.save_file(filename))
		return 1;

	return 0;
}

//Load a scene from a COLLADA file
int Scene::load(const char *filename)
{
	pugi::xml_document load_file;
	pugi::xml_parse_result result = load_file.load_file(filename);

	//Exit if the file could not be parsed
	if(result.status != pugi::xml_parse_status::status_ok)
	{
		return 1;
	}

	//Get the base node of the file
	if(load_file.child("COLLADA"))
	{
		pugi::xml_node root = load_file.child("COLLADA");

		//Get the scene node
		if(!(root.child("scene")))
			return 1;

		pugi::xml_node scene_node = root.child("scene");

		//Get the name of the visual scene to load
		if(!(scene_node.child("instance_visual_scene")))
			return 1;

		pugi::xml_node instance_scene_node = scene_node.child("instance_visual_scene");

		//Trim off the # character at the beginning of the url
		const char* scene_url = instance_scene_node.attribute("url").as_string();
		if(scene_url[0] == '#')
			scene_url++;
		else
			return 1;

		//Read each child node
		for(pugi::xml_node top_node = root.first_child(); top_node; top_node = top_node.next_sibling())
		{
			if(!strcmp(top_node.name(), "asset"))
			{
				//TODO: Process metadata
			}
			else if(!strcmp(top_node.name(), "library_geometries"))
			{
				for(pugi::xml_node geom_node = top_node.first_child(); geom_node; geom_node = geom_node.next_sibling())
				{
					if(!strcmp(geom_node.name(), "geometry"))
					{
						Geometry *g = new Geometry();
						if(!g->readGeometry(geom_node))
						{
							g->setVisibility(false);
							addObject(g);
						}
						else
						{
							delete g;
						}
					}
				}
			}
			else if(!strcmp(top_node.name(), "library_visual_scenes"))
			{
				//Only load the scene pointed to by instance_visual_scene
				for(pugi::xml_node vscene_node = top_node.first_child(); vscene_node; vscene_node = vscene_node.next_sibling())
				{
					if(!strcmp(vscene_node.name(), "visual_scene") && !strcmp(vscene_node.attribute("id").as_string(), scene_url))
					{
						//Iterate over each node in the scene
						for(pugi::xml_node node = vscene_node.first_child(); node; node = node.next_sibling())
						{
							//Add each geometry instance
							if(!strcmp(node.name(), "node"))
							{
								loadNode(node, NULL);
							}
						}
					}
				}
			}
		}
	}

	return 0;
}

void Scene::loadNode(pugi::xml_node node, Group *parent)
{
	for(pugi::xml_node node_child = node.first_child(); node_child; node_child = node_child.next_sibling())
	{
		if(!strcmp(node_child.name(), "instance_geometry"))
		{
			Geometry *base_geom = findObject(node_child.attribute("url").as_string());

			if(base_geom)
			{
				Geometry *g = new Instance(base_geom);

				if(node.child("matrix"))
				{
					//Transform the object if necessary
					Transform *t = g->getTransform();
					Matrix m;

					//Read matrix from COLLADA node
					pugi::xml_node matrix_node = node.child("matrix");
					std::string text = matrix_node.text().get();
					std::istringstream iss(text);

					iss >> m.r0[0]; iss >> m.r0[1]; iss >> m.r0[2]; iss >> m.r0[3];
					iss >> m.r1[0]; iss >> m.r1[1]; iss >> m.r1[2]; iss >> m.r1[3];
					iss >> m.r2[0]; iss >> m.r2[1]; iss >> m.r2[2]; iss >> m.r2[3];
					iss >> m.r3[0]; iss >> m.r3[1]; iss >> m.r3[2]; iss >> m.r3[3];

					t->setMatrix(m);
				}

				if(parent != NULL)
				{
					parent->addObject(g);
				}
				else
				{
					addObject(g);
				}
			}
		}
		else if(!strcmp(node_child.name(), "node"))
		{
			Group *geom_group = new Group();

			if(node.child("matrix"))
			{
				//Transform the object if necessary
				Transform *t = geom_group->getTransform();
				Matrix m;

				//Read matrix from COLLADA node
				pugi::xml_node matrix_node = node.child("matrix");
				std::string text = matrix_node.text().get();
				std::istringstream iss(text);

				iss >> m.r0[0]; iss >> m.r0[1]; iss >> m.r0[2]; iss >> m.r0[3];
				iss >> m.r1[0]; iss >> m.r1[1]; iss >> m.r1[2]; iss >> m.r1[3];
				iss >> m.r2[0]; iss >> m.r2[1]; iss >> m.r2[2]; iss >> m.r2[3];
				iss >> m.r3[0]; iss >> m.r3[1]; iss >> m.r3[2]; iss >> m.r3[3];

				t->setMatrix(m);
			}

			loadNode(node_child, geom_group);

			addObject(geom_group);
		}
	}
}

//Clear a scene
void Scene::clear()
{
	for(unsigned int i = 0; i < objects.size(); i++)
	{
		delete objects[i];
	}

	objects.clear();
}

//Find an object by id
Geometry *Scene::findObject(const char *id)
{
	//Remove # symbol if present in url
	if(id[0] == '#')
		id = &(id[1]);

	for(unsigned int i = 0; i < objects.size(); i++)
	{
		if(!strcmp(objects[i]->getUniqueId(), id))
			return objects[i];
	}

	return NULL;
}
