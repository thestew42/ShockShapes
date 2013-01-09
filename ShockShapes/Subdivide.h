/** @file Subdivide.h
 * 
 * @brief Geometry filter that subdivides polygons
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

#ifndef _SUBDIVIDE_
#define _SUBDIVIDE_

#include <vector>

#include "GeometryFilter.h"
#include "Geometry.h"

/**
 * @brief Structure used to keep track of which midpoints have been computed
 */
typedef struct {
	int v1, v2;
	int vmid;
} Midpoint;

/**
 * @brief Structure used to keep track of which interpolated normals have been calculated
 */
typedef struct {
	int n1, n2;
	int nmid;
} NormalInt;

/**
 * @brief Subdivides polygons of a mesh
 * @details TODO
 */
class Subdivide : public GeometryFilter {
private:
	int levels;

	/**
	 * Subdivides the object once
	 * @param g The geometric object to subdivide
	 */
	void subd(Geometry *g);

public:
	Subdivide(int ilevels);			/**< Constructs a filter that will subdivide ilevels times. */

	~Subdivide();					/**< Destructor. */

	/**
	 * Subdivides the object levels times
	 * @param g The object to apply the filter to
	 */
	virtual void run(Geometry *g);
};

#endif
