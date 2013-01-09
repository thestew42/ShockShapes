/** @file GeometryFilter.h
 * 
 * @brief Defines interface for objects that modify geometry
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

#ifndef _GEOMETRYFILTER_
#define _GEOMETRYFILTER_

#include <string>
#include <stdlib.h>

class Geometry;

/**
 * @brief Class used to express randomized parameters
 */
class Parameter {
public:
	/**
	 * The constraints on this parameter
	 */
	float min, max;

	float range;

	Parameter();		/**< Default constructor. */

	/**
	 * Creates a parameter with a fixed value
	 * @param value The fixed value of the parameter
	 */
	Parameter(float value);

	/**
	 * Creates a parameter in the given range
	 * @param imin The minimum value
	 * @param imax The maximum value
	 */
	Parameter(float imin, float imax);

	~Parameter();		/**< Destructor. */

	/**
	 * Samples the parameter for a single value
	 * @return A value in the parameter's range
	 */
	float sample();
};

/**
 * @brief Base class for all objects that modify (filter) geometry
 * @details Stores parameters which can be set up with
 * accessors. Child classes implement the run method which takes a
 * geometric object and modifies the mesh.
 */
class GeometryFilter {
protected:
	std::string name;					/**< Human recognizeable name of the filter. */

public:
	GeometryFilter();					/**< Default constructor. */
	GeometryFilter(const char *iname);	/**< Constructor which names the filter. */

	~GeometryFilter();					/**< Destructor. */

	/**
	 * Method implemented by each filter which modifies the geometry
	 * @param g The object to apply the filter to
	 */
	virtual void run(Geometry *g);
};

#endif
