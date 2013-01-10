/** @file TiledGroup.h
 * 
 * @brief Builds and stores a group of tiled objects
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

#ifndef _TILEDGROUP_
#define _TILEDGROUP_

#include "Group.h"

/**
 * @brief Builds a group of tiled objects
 * @details Uses a base Geometry object to generate multiple
 * instances and tile them into a 2D plane. Can be used for walls,
 * floors, etc
 */
class TiledGroup : public Group {
private:
	/**
	 * The object to use for tiles
	 */
	Geometry *base_object;

	int num_distinct;				/**< Number of distinct versions of the base object to generate. */
	
	float tile_x;					/**< X dimension of the tile. Tile assumed to be centered at origin. */
	float tile_z;					/**< Z dimension of the tile. Tile assumed to be centered at origin. */

	float group_x;					/**< X dimension to generate the group with. */
	float group_z;					/**< Z dimension to generate the group with. */

	float x_offset;					/**< Amount to offset tiles by in x for each z step. */

public:
	TiledGroup();					/**< Default constructor. */
	TiledGroup(const char *name);	/**< Named constructor. */

	~TiledGroup();					/**< Destructor. */

	/**
	 * Sets the base object to use for tiles.
	 * @param g Pointer to the base object
	 * @param x The x dimension of the base object
	 * @param z The z dimension of the base object
	 */
	void setBaseObject(Geometry *g, float x, float z);

	/**
	 * Sets the properties of the tiled group
	 * @param distinct Number of unique versions of the base object to use
	 * @param x The x dimension of the group
	 * @param z The z dimension of the group
	 * @param x_off The offset in the x direction for each z step
	 */
	void setTiledProperties(int distinct, float x, float z, float x_off);

	/**
	 * Generates the tiled group
	 * @param seed The value to seed the random number generator with
	 * @param scene The scene that this geometry will belong to
	 */
	virtual void generate(int seed, Scene *scene);
};

#endif
