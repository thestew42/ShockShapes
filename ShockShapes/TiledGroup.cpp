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
void TiledGroup::setTiledProperties(int distinct, float x, float z, float x_off, tile_end_method end_method)
{
	num_distinct = distinct;
	group_x = x;
	group_z = z;
	x_offset = x_off;
	tem = end_method;
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
	float x_left_bound = -0.5f * group_x;
	float x_right_bound = 0.5f * group_x;
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

		//Handle end tiles
		if(x_location > x_start) {
			//Determine needed width of end tile
			float needed_width = x_location - x_start;

			//Add partial tile
			Geometry *partial_tile;
			if((partial_tile = getPartialTile(needed_width, seed, scene)) != NULL) {
				partial_tile->getTransform()->setTranslation(x_left_bound + 0.5f * needed_width, 0.0f, z_location);
				addObject(partial_tile);
			}
		}

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

		//Handle end tile
		if((x_location - tile_half_x) < x_right_bound) {
			//Determine needed width of end tile
			float needed_width = x_right_bound - (x_location - tile_half_x);

			//Add partial tile
			Geometry *partial_tile;
			if((partial_tile = getPartialTile(needed_width, seed, scene)) != NULL) {
				partial_tile->getTransform()->setTranslation(x_right_bound - 0.5f * needed_width, 0.0f, z_location);
				addObject(partial_tile);
			}
		}

		//Increment to next row of tiles
		z_location += tile_z;
	}
}

//Retrieves a partial width tile
Geometry *TiledGroup::getPartialTile(float width, int seed, Scene *scene)
{
	if(tem == TEM_SCALE) {
		if(partial_tiles.size() == 0) {
			//Generate
			base_object->clearMesh();
			base_object->generate(seed, scene);
			base_object->filter();
					
			Geometry *tile_copy = new Geometry();
			base_object->cloneMesh(tile_copy);

			//Scale
			tile_copy->getTransform()->setScale(width / tile_x, 1.0f, 1.0f);

			//Store pointer and return the object
			partial_tiles.push_back(tile_copy);
			return tile_copy;
		} else {
			//Return instance
			Instance *tile_inst = new Instance(partial_tiles[0]);
			tile_inst->getTransform()->setScale(width / tile_x, 1.0f, 1.0f);

			return tile_inst;
		}
	} else {
		return NULL;
	}
}
