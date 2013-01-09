/** @file GeometryFilter.cpp
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

#include "GeometryFilter.h"

//----------------Parameter----------------------------------------------------
//Default constructor
Parameter::Parameter()
{
	range = min = max = 0.0f;
}

//Constructor with fixed value
Parameter::Parameter(float value)
{
	min = max = value;
	range = 0.0f;
}

//Constructor with a range
Parameter::Parameter(float imin, float imax)
{
	min = imin;
	max = imax;
	range = max - min;
}

//Destructor
Parameter::~Parameter()
{

}

//Sample the parameter
float Parameter::sample()
{
	if(range == 0.0f)
		return min;

	//Get a scalar
	float scalar = (float)rand() / (float)RAND_MAX;
	
	return (min + (scalar * range));
}

//----------------GeometryFilter-----------------------------------------------
//Constructor
GeometryFilter::GeometryFilter()
:name("NullFilter")
{
	
}

//Named filter
GeometryFilter::GeometryFilter(const char *iname)
:name(iname)
{

}

//Destructor
GeometryFilter::~GeometryFilter()
{

}

//Run not implemented by base class
void GeometryFilter::run(Geometry *g)
{

}
