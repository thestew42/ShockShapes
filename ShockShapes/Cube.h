/** @file Cube.h
 * 
 * @brief Basic geometric object generator for rectangular prisms
 * 
 * Copyright 2012-2013, Stewart Hall
 *
 * Licensed under The MIT License
 * Redistributions of files must retain the above copyright notice.
 *
 * @author		Stewart Hall (www.stewartghall.com)
 * @date		12/31/2012
 * @copyright	Copyright 2012-2013, Stewart Hall
 * @license		MIT License (http://www.opensource.org/licenses/mit-license.php)
 */

#ifndef _CUBE_
#define _CUBE_

#include "Geometry.h"

/**
 * @brief Generates cubes and other rectangular prisms
 */
class Cube: public Geometry {
private:
	//Dimensions of the object
	float l, w, h;

public:
	Cube();								/**< Default constructor makes a unit cube. */
	Cube(float dimension);				/**< Makes a cube with the specified side length. */
	Cube(float x, float y, float z);	/**< Makes a rectangular prism with specified side lengths. */

	~Cube();							/**< Destructor. */

	/**
	 * Generates the cube's mesh
	 * @param seed The value to seed the random number generator with
	 * @param scene The scene that this geometry will belong to
	 */
	virtual void generate(int seed, Scene *scene);
};

#endif
