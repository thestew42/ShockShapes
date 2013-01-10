/** @file TiledGroup.cpp
 * 
 * @brief Builds and stores a group of tiled objects
 * 
 * Copyright 2013, Stewart Hall
 *
 * Licensed under The MIT License
 * Redistributions of files must retain the above copyright notice.
 *
 * @author		Stewart Hall (www.stewartghall.com)
 * @date		1/10/2013
 * @copyright	Copyright 2013, Stewart Hall
 * @license		MIT License (http://www.opensource.org/licenses/mit-license.php)
 */

#include "TiledGroup.h"
#include "Instance.h"

//Default constructor
TiledGroup::TiledGroup()
:Group("TiledGroup")
{
	base_object = NULL;

	num_distinct = 1;
	x_offset = 0.0f;

	tile_x = 0.0f;
	tile_z = 0.0f;

	group_x = 0.0f;
	group_z = 0.0f;
}

//Named constructor
TiledGroup::TiledGroup(const char *name)
:Group(name)
{
	base_object = NULL;

	num_distinct = 1;
	x_offset = 0.0f;

	tile_x = 0.0f;
	tile_z = 0.0f;

	group_x = 0.0f;
	group_z = 0.0f;
}

//Destructor
TiledGroup::~TiledGroup()
{

}

//Set up base object
void TiledGroup::setBaseObject(Geometry *g, float x, float z)
{
	base_object = g;
	tile_x = x;
	tile_z = z;
}

//Set properties for generation
void TiledGroup::setTiledProperties(int distinct, float x, float z, float x_off)
{
	num_distinct = distinct;
	group_x = x;
	group_z = z;
	x_offset = x_off;
}

//Generate the tiled surface
void TiledGroup::generate(int seed, Scene *scene)
{
	float tile_half_x = 0.5f * tile_x;
	float tile_half_z = 0.5f * tile_z;

	//Set up list of unique base objects
	std::vector<Geometry*> tiles(num_distinct);
	for(int i = 0; i < num_distinct; i++)
		tiles[i] = NULL;

	//Determine starting and ending location in each axis
	float x_start = -0.5f * group_x + tile_half_x;
	float z_start = -0.5f * group_z + tile_half_z;
	float x_end = 0.5f * group_x - tile_half_x;
	float z_end = 0.5f * group_z - tile_half_z;
	
	//Loop through rows of tiles and determine tile location
	float offset = 0.0f;
	float z_location = z_start;
	while(z_location < z_end) {
		float x_location = x_start;

		//Offset x
		if(offset >= tile_x)
			offset -= tile_x;
		x_location += offset;
		offset += x_offset;

		while(x_location < x_end) {
			//Determine which of the base objects to use
			int object_index = rand() % num_distinct;

			if(tiles[object_index] == NULL) {
				//Generate because this object is not present
				base_object->clearMesh();
				base_object->generate(seed, scene);
				base_object->filter();
				
				Geometry *tile_copy = new Geometry();
				base_object->cloneMesh(tile_copy);

				//Add the copy to this group
				tile_copy->getTransform()->setTranslation(x_location, 0.0f, z_location);
				addObject(tile_copy);

				//Save pointer to base object copy
				tiles[object_index] = tile_copy;
			} else {
				//Create an instance of the tile
				Instance *tile_inst = new Instance(tiles[object_index]);
				tile_inst->getTransform()->setTranslation(x_location, 0.0f, z_location);

				addObject(tile_inst);
			}

			//Increment to next tile
			x_location += tile_x;
		}

		//Increment to next row of tiles
		z_location += tile_z;
	}
}
