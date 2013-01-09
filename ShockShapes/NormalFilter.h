/** @file NormalFilter.h
 * 
 * @brief Recalculates normals for a mesh
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

#ifndef _NORMALFILTER_
#define _NORMALFILTER_

#include "GeometryFilter.h"
#include "Geometry.h"

/**
 * @brief Defines the different methods of calculating normals
 */
enum normal_method {
	NM_HARDEN,		/**< Calculates face normals. */
	NM_SOFTEN		/**< Calculates per vertex normals based on several faces. */
};

/**
 * @brief Recalculates per-vertex normals for a mesh
 * @details TODO
 */
class NormalFilter : public GeometryFilter {
private:
	normal_method method;	/**< Method of calculation. */
	bool soften_all;		/**< Set to true when all edges should be softened if NM_SOFTEN is used. */

	/**
	 * @details If method is NM_SOFTEN and soften_all is set to false
	 * this threshold parameter determines which faces are softened based on
	 * the difference in their surface normals. When this parameter is close to
	 * 0, only faces with similar face normals are softened.
	 */
	float threshold;

public:
	NormalFilter(normal_method imethod);	/**< Creates a new filter based on the specified generation method. */

	~NormalFilter();							/**< Destructor. */

	/**
	 * Sets up a threshold for softening
	 * @param t The threshold for softening. Should be between 0 and 2
	 */
	void enableSoftenThreshold(float t);

	/**
	 * Disables soften threshold
	 */
	void disableSoftenThreshold();

	/**
	 * Changes the method of normal generation
	 * @param m Method to use
	 */
	void changeMethod(normal_method m);

	/**
	 * Regenerates normals
	 * @param g The object to apply the filter to
	 */
	virtual void run(Geometry *g);
};

#endif
